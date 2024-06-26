//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#include "documentviewer.h"
#include <iostream>
#include <filesystem>
#include <QDebug>
#include <QProcess>
#include <QThread>
#include <string>
#include <signal.h>
#include <thread>
#include <klocalizedstring.h>

class ConversionThread : public QThread
{

public:

    int threadId;
    QProcess process;
    std::string path;
    std::string libreofficeCommand;
    bool isLibreofficeFlatpak;
    std::string tempDir;
    DocumentViewer *mainClass;
    // It is used to mitigate a problem that occurs when the PC has just been started and the waiting time (waitForFinished) of QProcess is not respected.
    //To try converting the document one more time.
    bool FirstLoading = true;

    void run() override
    {
        convertDocument(path,tempDir);
    }


    void convertDocument(std::string path,std::string tempDir){
        // used to discard the thread if it is no longer needed.
        threadId = mainClass->activeThreadId;
        mainClass->closeActiveConversionProcess();

        std::string result;
        std::string standardError;
        QStringList libreofficeArgs = QStringList() << QString::fromStdString("run") << QString::fromStdString("run org.libreoffice.LibreOffice") << QString::fromStdString("--headless")  << QString::fromStdString("--nolockcheck") << QString::fromStdString("--norestore")
                                                        << QString::fromStdString("--convert-to")   << QString::fromStdString("pdf") << QString::fromStdString(path) << QString::fromStdString("--outdir") << QString::fromStdString(tempDir);
        QProcess process;

        process.setProgram(QString::fromStdString(libreofficeCommand));

        if (isLibreofficeFlatpak){
            process.setArguments(QStringList() << QString::fromStdString("run")   << QString::fromStdString("org.libreoffice.LibreOffice") << libreofficeArgs );
        } else{
            process.setArguments(libreofficeArgs);

        }
        process.start();
        process.waitForStarted();
        mainClass->currentProcessPid = process.processId();
        mainClass->processIsRunning = true;
        bool exitStatus = process.waitForFinished(10000);
        mainClass->processIsRunning = false;

        //discard the thread if it is no longer needed.
        if ( threadId != mainClass->activeThreadId || mainClass->isActiveViewer == false){
            return;
        }
        if (exitStatus){
            //all Ok
        }
        else{
            if (FirstLoading){
                // It is used to mitigate a problem/bug that occurs when the PC has just been started and the waiting time (waitForFinished) of QProcess is not respected.
                //To try converting the document one more time.
                FirstLoading = false;
                qDebug() << QStringLiteral("retry FirstLoading");
                convertDocument(path,tempDir);
                return;
            }
            //timeout
            mainClass->loadingArrested(1);
            qDebug() << "error:file took to long to load";
            return;
        }

        result =  process.readAllStandardOutput().toStdString();
        standardError = process.readAllStandardError().toStdString();
        //removes the newline character because otherwise the html link that is inserted after it doesn't work...
        standardError.erase(std::remove(standardError.begin(), standardError.end(), '\n'), standardError.cend());

        //removes the warning part of the text (always present with libreoffice)
        size_t pos = standardError.find("Error");
        standardError.erase(0,pos);

        if ( result == ""){

            if (standardError == ""){
                //sometimes the standardOutput and error returns null if you quickly close and reopen Kview.
                //I think it's caused by flatpak.
                qDebug() << "Unknown error";

                //error message
                //mainClass->loadingArrested(2);

                //Reload the file.
                mainClass->reloadDocument();
                //auto reload?

            }
            else{
                //send libreoffice error
                //999: customError
                mainClass->loadingArrested(999,QString().fromStdString(standardError));
                qDebug() << "no results error:" << QString().fromStdString(standardError);
            }
            return;
        }


        // get the path
        unsigned first = result.find(">");
        unsigned last = result.find(" using filter");
        //+/-2 is for offset
        std::string pdfPath = result.substr (first +2 ,last-first-2);
        qDebug() << QString::fromStdString(pdfPath);

        //check if the path exists
        if (! std::filesystem::exists(pdfPath)){
            qDebug() << "new pdf path does not exist";
            mainClass->loadingArrested(3);
            return;
        }
        //load the new pdf
        mainClass->conversionFinished(pdfPath);
    }
};






DocumentViewer::DocumentViewer(QObject *parent)
    : QObject{parent}
{
    activeThreadId=0;
    libreofficeCommand = getLibreofficeCommand();
    tempDir = std::filesystem::temp_directory_path();

}


void DocumentViewer::loadFile(std::string filePath, std::string extension)
{
    closeActiveConversionProcess();
    setViewerDocument(QStringLiteral(""));
    showLoadingScreen();
    //saves the path of the current file in case it needs to be reloaded
    currentFilePath = filePath;
    //it is used to discard the thread if not longer needed
    isActiveViewer = true;
    //the id is used to discard the thread if not longer needed
    randomId = rand();
    //to make sure the new id is different
    while (activeThreadId == randomId ){
        randomId = rand();
    }
    activeThreadId = randomId;

    //Checks whether the file type requires conversion.
    if (extension == ".pdf"){
        QString path;
        path = QString::fromStdString("file://"+currentFilePath);
        setViewerDocument(path);
        setMessagePageVisible(false);
        setLoadingBarVisible(false);
    }
    else {
        convertDocument(filePath);
    }


}

void DocumentViewer::convertDocument(std::string path){
    //# shows a message and stops the function if libreoffice was not found.
    if (libreofficeCommand == ""){
        loadingArrested(4);
        qDebug() << "libreoffice not found";
        return;
    }

    //thread
    ConversionThread* t = new ConversionThread;
    t->libreofficeCommand = libreofficeCommand;
    t->mainClass = this;
    t->path = path;
    t->tempDir = tempDir;
    t->isLibreofficeFlatpak = isLibreofficeFlatpak;
    t->start();

}



void DocumentViewer::conversionFinished(std::string path){
    //calls the function with the path to the new pdf
    loadFile(path,".pdf");
}


void DocumentViewer::stopViewer()
{
    isActiveViewer = false;
    closeActiveConversionProcess();
}

void DocumentViewer::showLoadingScreen()
{
    setMessagePageVisible(true);
    setMessageHeaderText(QStringLiteral(""));
    setMessageDescriptionText(QStringLiteral(""));
    setLoadingBarVisible(true);
    setReloadButtonVisible(false);
    setHelpPageLinkVisible(false);

}

void DocumentViewer::reloadDocument()
{
    loadFile(currentFilePath,"");
}

void DocumentViewer::closeActiveConversionProcess(){


    if (processIsRunning == true){

        if (isLibreofficeFlatpak){
            // Unfortunately this is the only way I've found to close the Flatpak version of libreoffice.
            std::string command = R"(kill $( ps -aux | grep -F "/soffice.bin" | grep -F " --headless --nolockcheck --norestore --convert-to pdf" | grep -F " --outdir"| awk '{ print $2 }'))";
            QProcess process;
            process.setProgram(QStringLiteral("bash"));
            process.setArguments( QStringList() << QStringLiteral("-c") << QString().fromStdString(command.c_str()));
            process.start();
            process.waitForFinished();

        }else{
            kill(currentProcessPid,SIGTERM);
        }
        processIsRunning = false;
    }
}


std::string DocumentViewer::getLibreofficeCommand(){
    std::string command = "";

    if (! system("which libreoffice > /dev/null 2>&1")){
        // check if libreoffice is installed
        command = "libreoffice";
        isLibreofficeFlatpak=false;
    }
    else if (! system("which flatpak > /dev/null 2>&1")){
        //check if libreoffice is installed with flatpak
        QProcess process;
        process.start(QStringLiteral("bash"), QStringList() << QStringLiteral("-c") << QStringLiteral("flatpak list | grep -i libreoffice"));
        process.waitForFinished();
        if (process.exitCode() == 0){
            isLibreofficeFlatpak=true;
            command = "flatpak";
        }
    }
    return command;

}




void DocumentViewer::loadingArrested(int errorCode,QString customError){
    //code 1: file took too long to load
    //code 2: general error
    //code 3: flatpak error
    //code 4: libreoffice missing
    //code 999: libreoffice error message
    setLoadingBarVisible(false);
    setMessageHeaderText(i18n("Preview not available."));

    if (errorCode == 1){
        setMessageDescriptionText(i18n("The document took too long to load."));
        setReloadButtonVisible(true);
    }
    else if (errorCode == 2){
        //general message and reload option
        setMessageDescriptionText(i18n("Something went wrong."));
        setReloadButtonVisible(true);
    }
    else if (errorCode == 3){
        //new pdf not found/flatpak error
        setHelpPageLinkVisible(true);
        setMessageDescriptionText(i18n("File not found: Flatpak may not be configured correctly:"));
    }
    else if (errorCode == 4){
        //libreoffice not found
        setHelpPageLinkVisible(true);
        setMessageDescriptionText(i18n("Libreoffice is not installed:"));
    }
    else if (errorCode == 999){
        //libre error/general error: source could not be loaded
        setHelpPageLinkVisible(true);
        setMessageDescriptionText(customError + QStringLiteral(":"));
    }
}






QString DocumentViewer::viewerDocument() const
{
    return m_viewerDocument;
}

void DocumentViewer::setViewerDocument(const QString &newViewerDocument)
{
    if (m_viewerDocument == newViewerDocument)
        return;
    m_viewerDocument = newViewerDocument;
    Q_EMIT viewerDocumentChanged();
}


QString DocumentViewer::messageHeaderText() const
{
    return m_messageHeaderText;
}

void DocumentViewer::setMessageHeaderText(const QString &newMessageHeaderText)
{
    if (m_messageHeaderText == newMessageHeaderText)
        return;
    m_messageHeaderText = newMessageHeaderText;
    Q_EMIT messageHeaderTextChanged();
}

bool DocumentViewer::messagePageVisible() const
{
    return m_messagePageVisible;
}

void DocumentViewer::setMessagePageVisible(bool newMessagePageVisible)
{
    if (m_messagePageVisible == newMessagePageVisible)
        return;
    m_messagePageVisible = newMessagePageVisible;
    Q_EMIT messagePageVisibleChanged();
}

QString DocumentViewer::messageDescriptionText() const
{
    return m_messageDescriptionText;
}

void DocumentViewer::setMessageDescriptionText(const QString &newMessageDescriptionText)
{
    if (m_messageDescriptionText == newMessageDescriptionText)
        return;
    m_messageDescriptionText = newMessageDescriptionText;
    Q_EMIT messageDescriptionTextChanged();
}

bool DocumentViewer::reloadButtonVisible() const
{
    return m_reloadButtonVisible;
}

void DocumentViewer::setReloadButtonVisible(bool newReloadButtonVisible)
{
    if (m_reloadButtonVisible == newReloadButtonVisible)
        return;
    m_reloadButtonVisible = newReloadButtonVisible;
    Q_EMIT reloadButtonVisibleChanged();
}

bool DocumentViewer::loadingBarVisible() const
{
    return m_loadingBarVisible;
}

void DocumentViewer::setLoadingBarVisible(bool newLoadingBarVisible)
{
    if (m_loadingBarVisible == newLoadingBarVisible)
        return;
    m_loadingBarVisible = newLoadingBarVisible;
    Q_EMIT loadingBarVisibleChanged();
}


bool DocumentViewer::helpPageLinkVisible() const
{
    return m_helpPageLinkVisible;
}

void DocumentViewer::setHelpPageLinkVisible(bool newHelpPageLinkVisible)
{
    if (m_helpPageLinkVisible == newHelpPageLinkVisible)
        return;
    m_helpPageLinkVisible = newHelpPageLinkVisible;
    Q_EMIT helpPageLinkVisibleChanged();
}

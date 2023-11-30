//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#include "imageviewer.h"
#include <filesystem>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <signal.h>
#include <klocalizedstring.h>
std::vector<std::string> archiveExtensions{".kra"};




class PreviewExtractionThread : public QThread
{

public:

    int threadId;
    std::string path;
    std::string tempDir;
    ImageViewer *mainClass;

    void run() override
    {
        extractPreviewThread(path,tempDir);
    }


    void extractPreviewThread(std::string path,std::string tempDir){
        //used to discard the thread if no longer needed.
        threadId = mainClass->activeThreadId;
        mainClass->closeActiveSubProcess();


        if (! checkExecutable("unzip")){
            //not found
            mainClass->setErrorMessageText(i18n("unzip package not installed"));
            return;
        }

        //command to use to extract the image from the archive.
        std::string command = "unzip -o '" + path + "' mergedimage.png -d " + tempDir;
        QProcess process;
        //setsid is used to also kill the children of the subprocess.
        process.setProgram("setsid");
        process.setArguments( QStringList() << "bash" << "-c" <<  QString::fromStdString(command) );
        process.start();
        process.waitForStarted();
        mainClass->currentProcessPid = process.processId();
        mainClass->processIsRunning = true;
        bool exitStatus = process.waitForFinished(10000);
        mainClass->processIsRunning = false;
        std::string standardError = process.readAllStandardError().toStdString();

        //used to discard the thread if no longer needed.
        if ( threadId != mainClass->activeThreadId || mainClass->isActiveViewer == false){
            return;
        }

        if (exitStatus){
            //all Ok
            //load the extracted .png
            mainClass->loadFile(tempDir+"/mergedimage.png",".png");
            return;
        }

        if (standardError != ""){
            //sets an error message in the UI
             mainClass->setErrorMessageText(QString().fromStdString(standardError));
        }

        return;

    }
    bool checkExecutable(std::string exec){
        std::string command = "which " + exec +" > /dev/null 2>&1";



        if (! system(command.c_str())){
            return true;
        }
        else{

            return false;
        }
    }
};


ImageViewer::ImageViewer(QObject *parent)
    : QObject{parent}
{
    //folder in which to put the extracted preview.
    tempDir = std::filesystem::temp_directory_path();
    activeThreadId=0;
}

void ImageViewer::loadFile(std::string filePath, std::string extension)
{
    closeActiveSubProcess();
    setErrorMessageText(QString(""));
    setViewerImage("");
    //it is used to discard the thread if not longer needed
    isActiveViewer = true;
    //the id is used to discard the thread if not longer needed
    randomId = rand();
    //to make sure the new id is different
    while (activeThreadId == randomId ){
        randomId = rand();
    }
    activeThreadId = randomId;

    // Checks whether the file type requires extraction.
    if (std::find(std::begin(archiveExtensions), std::end(archiveExtensions), extension) != std::end(archiveExtensions)) {
        extractPreview(filePath);

    }
    else {
        QString path;
        path = QString::fromStdString("file://"+filePath);
        setViewerImage(path);
    }
}



void ImageViewer::extractPreview(std::string path)
{
    //uses a thread to avoid slowing down the application.
    PreviewExtractionThread* t = new PreviewExtractionThread;
    t->mainClass = this;
    t->path = path;
    t->tempDir = tempDir;
    t->start();
    return;

}




QString ImageViewer::viewerImage() const
{
    return m_viewerImage;
}

void ImageViewer::setViewerImage(const QString &newViewerImage)
{
    if (m_viewerImage == newViewerImage)
        return;
    m_viewerImage = newViewerImage;
    emit viewerImageChanged();
}

void ImageViewer::stopViewer()
{
    isActiveViewer = false;
    closeActiveSubProcess();
}



void ImageViewer::closeActiveSubProcess(){
    if (processIsRunning == true){
        killpg(currentProcessPid,SIGTERM);
        processIsRunning = false;
    }
}


QString ImageViewer::errorMessageText() const
{
    return m_errorMessageText;
}

void ImageViewer::setErrorMessageText(const QString &newErrorMessageText)
{
    if (m_errorMessageText == newErrorMessageText)
        return;
    m_errorMessageText = newErrorMessageText;
    emit errorMessageTextChanged();
}

//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#include "containerviewer.h"
#include "qdatetime.h"
#include "qfileinfo.h"
#include <filesystem>
#include <QDebug>
#include <QThread>
#include <QProcess>
#include <math.h>
#include <signal.h>
#include <sys/stat.h>
#include <tuple>
#include <klocalizedstring.h>
class ContainerVieverThread : public QThread
{

public:

    int threadId;
    std::string path;
    bool isArchive;
    std::string extension;
    ContainerViewer *mainClass;


    void run() override
    {

        //get last modified time
        const QFileInfo info(QString().fromStdString(path));
        const QDateTime lastModified = info.lastModified();
        Qt::DateFormat format = Qt::DateFormat(QLocale::ShortFormat);
        QString stringLastModified = lastModified.date().toString(format);
        //sets the info in the interface
        mainClass->setFileLastModified(stringLastModified);

        if (isArchive){
            mainClass->setFileIcon(QStringLiteral("application-zip"));
            loadArchive(path,extension);
        }
        else {
            mainClass->setFileIcon(QStringLiteral("folder-blue"));
            loadDir(path);
        }

    }
    void loadDir(std::string path){
        QStringList contentList;
        QStringList folderList;
        QStringList fileList;
        QString elementString;
        //used to discard the thread if no longer needed.
        threadId = mainClass->activeThreadId;

        for (const auto & entry : std::filesystem::directory_iterator(path)){
            if (entry.path().filename().generic_string()[0] == '.'){
                //if the name begins with "." (hidden folder), move to the next entry in the loop(so as not to count hidden folders).
                continue;
            }

            if (entry.is_directory()){
                //"!isDir!" is used to identify the folders in qml.
                elementString = QString().fromStdString("!isDir!" + entry.path().filename().generic_string());
                folderList.append(elementString);
            }
            else{
                elementString = QString().fromStdString(entry.path().filename().generic_string());
                fileList.append(elementString);
            }
        }
        //alphabetical order and folders first.
        folderList.sort();
        fileList.sort();
        contentList << folderList << fileList;

        if ( threadId != mainClass->activeThreadId || mainClass->isActiveViewer == false){
            //content no longer needed, exiting...
            return;
        }

        mainClass->setViewerElement(contentList);
        mainClass->setFileContentCount(QString::number(contentList.count()));
        return;


    }

    void loadArchive(std::string path,std::string extension){
        //used to discard the thread if no longer needed.
        threadId = mainClass->activeThreadId;
        mainClass->closeActiveSubProcess();

        //gets the size
        auto size = getFilesize(path);
        //convert to new size
        const auto[newSize, unit] = convertFileSize(getFilesize(path));
        //set
        mainClass->setFileSize(QString::number(newSize));
        mainClass->setFileSizeUnit(unit);

        //avoid opening the archive if it is too big.
        if (size > 1073741824){ // 1gib
            //file too big
            qDebug() << "error: file too big";
            mainClass->setErrorMessageText(i18n("Preview not available: File too big"));
            return;
        }

        std::string exec;
        std::string command;

        //choose the command based on the archive type.
        if (extension  ==  ".zip"){
            exec = "zipinfo";
            command = "zipinfo -1 '" + path +"'";
        }
        else if (extension  ==  ".gz" || extension  ==  ".xz"){
            exec = "tar";
            command = "tar --list -f '" + path +"'";
        }
        else if (extension  ==  ".rar"){
            exec = "unrar";
            command = "unrar l  '" + path +"' " + R"(| grep  -Fv '..D..' | cut -d' ' -f13-100 | grep -v "Date    Time   Name" | grep -v "                  " | grep -vi "Alexander Roshal" | sed '/^$/d' | sed 's/^[ \t]*//')";
        }

        //checks if the executable exists.
        if (! checkExecutable(exec)){
            //exec not found
            qDebug() << QString().fromStdString(exec +" package not installed");
            //set error message
            mainClass->setErrorMessageText(i18n("Package not installed:") + QStringLiteral(" ") + QString().fromStdString(exec));
            return;
        }

        //setsid is used to also close the children of the subprocess. not required in this case.
        QProcess process;
        process.setProgram(QStringLiteral("setsid"));
        process.setArguments( QStringList() << QStringLiteral("bash") << QStringLiteral("-c") <<  QString::fromStdString(command) );
        process.start();
        process.waitForStarted();
        mainClass->currentProcessPid = process.processId();
        mainClass->processIsRunning = true;
        bool exitStatus = process.waitForFinished(10000);
        mainClass->processIsRunning = false;
        std::string result =  process.readAllStandardOutput().toStdString();
        std::string standardError = process.readAllStandardError().toStdString();

        if ( threadId != mainClass->activeThreadId || mainClass->isActiveViewer == false){
            //content no longer needed, exiting...
            return;
        }

        if (! exitStatus){
            //failed
            qDebug() << "Failed with no error";
            qDebug() << "maybe the file took too long to load.";
            mainClass->setErrorMessageText(i18n("File took to long to load"));
            if (standardError != ""){
                mainClass->setErrorMessageText( i18n("Error:")+QString().fromStdString(standardError));
                qDebug() << QString().fromStdString(standardError);
            }
            return;
        }

        std::vector resultVector = splitString(result,"\n");
        QStringList contentList;

        for (const auto& element : resultVector) {
            //adds the item if it is not a folder.
            if (! endsWith(element,"/")){
                contentList.append(QString().fromStdString(element));
            }
        }



        //removes empty strings added to the list(the newline character I think).
        contentList.removeAll(QStringLiteral(""));
        //sets the text in the UI
        mainClass->setFileContentCount(QString::number(contentList.count()));
        mainClass->setViewerElement(contentList);
        return;

    }


    size_t getFilesize(const std::string& filename) {
        struct stat st;
        if(stat(filename.c_str(), &st) != 0) {
            return 0;
        }
        return st.st_size;
    }

    std::tuple<float,QString> convertFileSize(size_t size){
        //convert bytes to KiB
        float convertedSize = size/1024;
        //        size = size/1024;
        //starting unit
        QString unit = QStringLiteral("KiB");

        if (convertedSize > 1024){
            //convert KiB to MiB if more than 1024 KiB
            convertedSize = convertedSize/1024;
            unit = QStringLiteral("MiB");
        }

        if (convertedSize > 1024){
            //convert MiB to GiB if more than 1024 MiB
            convertedSize = convertedSize/1024;
            unit = QStringLiteral("GiB");
        }

        //removes some numbers after the decimal point.
        float convertedSizeTruncated = float(round(convertedSize * 100)) / 100;
        return std::make_tuple(convertedSizeTruncated,unit);
    }


    bool endsWith(const std::string &mainStr, const std::string &toMatch)
    {
        if(mainStr.size() >= toMatch.size() &&
            mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
            return true;
        else
            return false;
    }

    std::vector<std::string> splitString(std::string str, std::string token){
        std::vector<std::string>result;
        while(str.size()){
            const unsigned long index = str.find(token);
            //               int index = str.find(token);
            if(index!=std::string::npos){
                result.push_back(str.substr(0,index));
                str = str.substr(index+token.size());
                if(str.size()==0)result.push_back(str);
            }else{
                result.push_back(str);
                str = "";
            }
        }
        return result;
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







ContainerViewer::ContainerViewer(QObject *parent)
    : QObject{parent}
{
    //the id is used to discard the thread if not longer needed
    activeThreadId=0;
}


void ContainerViewer::loadFile(std::string filePath, bool isArchive, std::string extension)
{
    resetView();
    closeActiveSubProcess();
    //used to discard the thread if not longer needed
    isActiveViewer = true;
    //the id is used to discard the thread if not longer needed
    randomId = rand();
    //to make sure the new id is different
    while (activeThreadId == randomId ){
        randomId = rand();
    }
    activeThreadId = randomId;

    //a thread is used to keep the application from slowing down.
    startThread(filePath,isArchive,extension);
}

void ContainerViewer::startThread(std::string path,bool isArchive,std::string extension)
{

    ContainerVieverThread* t = new ContainerVieverThread;
    t->mainClass = this;
    t->path = path;
    t->isArchive = isArchive;
    t->extension = extension;
    t->start();
    return;

}


void ContainerViewer::stopViewer()
{
    isActiveViewer = false;
    closeActiveSubProcess();
}

void ContainerViewer::closeActiveSubProcess(){
    if (processIsRunning == true){
        killpg(currentProcessPid,SIGTERM);
        processIsRunning = false;
    }
}

void ContainerViewer::resetView()
{
    setErrorMessageText(QStringLiteral(""));
    setViewerElement(QStringList());
    setFileLastModified(QStringLiteral(""));
    setFileContentCount(QStringLiteral(""));
    setFileSize(QStringLiteral(""));
}

QStringList ContainerViewer::viewerElement() const
{
    return m_viewerElement;
}

void ContainerViewer::setViewerElement(const QStringList &newViewerElement)
{
    if (m_viewerElement == newViewerElement)
        return;
    m_viewerElement = newViewerElement;
    Q_EMIT viewerElementChanged();
}

QString ContainerViewer::fileIcon() const
{
    return m_fileIcon;
}

void ContainerViewer::setFileIcon(const QString &newFileIcon)
{
    if (m_fileIcon == newFileIcon)
        return;
    m_fileIcon = newFileIcon;
    Q_EMIT fileIconChanged();
}

QString ContainerViewer::fileSize() const
{
    return m_fileSize;
}

void ContainerViewer::setFileSize(const QString &newFileSize)
{
    if (m_fileSize == newFileSize)
        return;
    m_fileSize = newFileSize;
    Q_EMIT fileSizeChanged();
}

QString ContainerViewer::fileSizeUnit() const
{
    return m_fileSizeUnit;
}

void ContainerViewer::setFileSizeUnit(const QString &newFileSizeUnit)
{
    if (m_fileSizeUnit == newFileSizeUnit)
        return;
    m_fileSizeUnit = newFileSizeUnit;
    Q_EMIT fileSizeUnitChanged();
}

QString ContainerViewer::fileContentCount() const
{
    return m_fileContentCount;
}

void ContainerViewer::setFileContentCount(const QString &newFileContentCount)
{
    if (m_fileContentCount == newFileContentCount)
        return;
    m_fileContentCount = newFileContentCount;
    Q_EMIT fileContentCountChanged();
}

QString ContainerViewer::fileLastModified() const
{
    return m_fileLastModified;
}

void ContainerViewer::setFileLastModified(const QString &newFileLastModified)
{
    if (m_fileLastModified == newFileLastModified)
        return;
    m_fileLastModified = newFileLastModified;
    Q_EMIT fileLastModifiedChanged();
}

QString ContainerViewer::errorMessageText() const
{
    return m_errorMessageText;
}

void ContainerViewer::setErrorMessageText(const QString &newErrorMessageText)
{
    if (m_errorMessageText == newErrorMessageText)
        return;
    m_errorMessageText = newErrorMessageText;
    Q_EMIT errorMessageTextChanged();
}

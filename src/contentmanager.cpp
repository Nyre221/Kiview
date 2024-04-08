//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#include "contentmanager.h"
#include "qapplication.h"
#include <QDebug>
#include <filesystem>
#include <thread>
#include <QProcess>
#include <unistd.h>
QStringList files;
//It is used to decide which viewer should load the file.
std::vector<std::string> textExtensions{".txt", ".sh",".json"};
std::vector<std::string> docExtensions{".pdf",".doc", ".docx", ".odt", ".ods", ".xlsx", ".xls", ".csv", ".odp", ".ppt", ".pptx"};
std::vector<std::string> imgExtensions{".png",".jpg",".jpeg",".kra",".svgz",".svg",".webp"};
std::vector<std::string> mediaExtensions{".mp4",".mp3",".webm"};
std::vector<std::string> containerExtensions{".zip",".gz",".xz",".rar"};
int currentIndex = 0;


ContentManager::ContentManager(QObject *parent)
    : QObject{parent}
{
}

void ContentManager::setFiles(std::string path,bool fileWasSelected ){

    std::filesystem::directory_entry filePath = std::filesystem::directory_entry{path};

    //file was selected
    if (fileWasSelected){
        //the current file is the selected one.
        currentFile = filePath.path();
        //gets the parent folder and searches its contents.
        std::filesystem::path currentSearchPath = filePath.path().parent_path();
        files = searchPath(currentSearchPath);
        //search for the index of the selected file in the list.
        auto it = std::find(files.begin(), files.end(), QString().fromStdString(currentFile.string()));
        //the current index number corresponds to that of the file.
        currentIndex = it - files.begin();


    }
    else{
        //no selection
        //the current file is the (parent) folder given in the variable.
        currentFile = filePath;
        //searches for contents in the given folder.
        std::filesystem::path currentSearchPath = filePath.path();
        files = searchPath(currentSearchPath);
        //the parent folder is inserted into the list.
        //this is to allow the user to see information about the parent folder and be able to scroll through its files.
        files.insert(0,QString().fromStdString(filePath.path().string()));
        //the current index is 0 (parent folder).
        currentIndex = 0;
        //used to manage the information shown in the title bar.
        parentInIndex = true;
    }
    //Enables the ability to scroll through files.
    //It is used to avoid crashing the application if the index is empty.
    indexReady = true;
    //load the file
    loadFileAtIndex(currentIndex);
}



QStringList ContentManager::searchPath(std::filesystem::__cxx11::path path)
{

    QStringList contentList;
    QStringList folderList;
    QStringList fileList;

    for (const auto & entry : std::filesystem::directory_iterator(path)){
        if (entry.path().filename().generic_string()[0] == '.'){
            //if the name begins with "." (hidden folder), move to the next entry in the loop (so as not to count hidden folders).
            continue;
        }

        if (entry.is_directory()){
            folderList.append(QString().fromStdString(entry.path().string()));
        }
        else{
            fileList.append(QString().fromStdString(entry.path().string()));
        }
    }
    //alphabetical order and folders first.
    folderList.sort();
    fileList.sort();
    contentList << folderList << fileList;

    return contentList;
}


void ContentManager::stopViewers()
{
    //It is used to close running threads,subprocesses, and other viewer-specific things.
    if (m_documentViewer->isActiveViewer){
        m_documentViewer->stopViewer();
    }
    if (m_imageViewer->isActiveViewer){
        m_imageViewer->stopViewer();
    }
    if (m_containerViewer->isActiveViewer){
        m_containerViewer->stopViewer();
    }

}






void ContentManager::loadFileAtIndex(int index){
    //stops possible running subprocesses/threads.
    stopViewers();
    //it is used to put a minimum waiting time before being able to change file.
    //the qml interface tends to crash if the user keeps the navigation keys pressed if there is no minimum waiting time.
    switchEnabled = false;
    std::thread th(&ContentManager::switchTimer,this);
    th.detach();


    QString titleBarText;
    if (parentInIndex){
        titleBarText = QStringLiteral("[") + QString::number(index) + QStringLiteral("/") + QString::number(files.size()-1) +QStringLiteral("]");
    }
    else{
        titleBarText = QStringLiteral("[") + QString::number(index+1) + QStringLiteral("/") + QString::number(files.size()) +QStringLiteral("]");
    }

    setWindowTitle(titleBarText);



    currentFilePath = files.at(index).toStdString();
    std::string extension = currentFilePath.extension().string();

    //sets what the current file name is in the "header bar" of the interface.
    setFileName(QString::fromStdString(currentFilePath.filename().string()));
    //It is used to understand if is a text file or not.
    std::string command = R"(file --mime-type ')" + currentFilePath.string() + R"(' | grep text/  )";

    //load the file with the correct viewer.
    if (std::filesystem::is_directory(currentFilePath)) {
        m_containerViewer->loadFile(currentFilePath.string(),false,"");
        setCurrentViewer(QStringLiteral("container_viewer"));
    }
    else if(std::find(std::begin(textExtensions), std::end(textExtensions), extension) != std::end(textExtensions)) {
        m_textViewer->loadFile(currentFilePath.string());
        setCurrentViewer(QStringLiteral("text_viewer"));
    }
    else if (std::find(std::begin(docExtensions), std::end(docExtensions), extension) != std::end(docExtensions)) {
        m_documentViewer->loadFile(currentFilePath.string(),extension);
        setCurrentViewer(QStringLiteral("document_viewer"));
    }
    else if (std::find(std::begin(imgExtensions), std::end(imgExtensions), extension) != std::end(imgExtensions)) {
        m_imageViewer->loadFile(currentFilePath.string(),extension);
        setCurrentViewer(QStringLiteral("image_viewer"));
    }
    else if (std::find(std::begin(mediaExtensions), std::end(mediaExtensions), extension) != std::end(mediaExtensions)) {
        m_videoViewer->loadFile(currentFilePath.string());
        setCurrentViewer(QStringLiteral("video_viewer"));
    }
    else if (std::find(std::begin(containerExtensions), std::end(containerExtensions), extension) != std::end(containerExtensions)) {
        m_containerViewer->loadFile(currentFilePath.string(),true,extension);
        setCurrentViewer(QStringLiteral("container_viewer"));
    }
    else if (! (system(command.c_str()))) {
        //uses a subprocess to figure out if it's a plain text file.
        m_textViewer->loadFile(currentFilePath.string());
        setCurrentViewer(QStringLiteral("text_viewer"));
    }
    else{
        setCurrentViewer(QStringLiteral("fallback_viewer"));
    }


}

void ContentManager::goBack()
{
    if (! switchEnabled || ! indexReady) {
        return;
    }

    if (currentIndex == 0){
        currentIndex = files.size() -1;
    }
    else {

        currentIndex -= 1;
    }

    loadFileAtIndex(currentIndex);

}


void ContentManager::goForward()
{
    if (! switchEnabled || ! indexReady) {
        return;
    }

    if (files.size() -1 == currentIndex){
        currentIndex =0;
    }
    else {

        currentIndex += 1;
    }
    loadFileAtIndex(currentIndex);
}

void ContentManager::openApp()
{
//opens the file with the default application via xdg-open.
    QProcess process;
    //the output must be redirected to QProcess::nullDevice()
    //otherwise Qt applications will not start if the parent process is closed.
    //https://forum.qt.io/topic/129701/qprocess-startdetached-but-the-child-process-closes-when-the-parent-exits/2
    process.setStandardErrorFile(QProcess::nullDevice());
    process.setStandardInputFile(QProcess::nullDevice());
    process.setStandardOutputFile(QProcess::nullDevice());

    process.setProgram(QStringLiteral("xdg-open"));
    process.setArguments(QStringList() << QString().fromStdString(currentFilePath.string().c_str()));
    process.startDetached();

    QApplication::quit();
}

void ContentManager::closing()
{
    //when the app is closing.
    stopViewers();
}

void ContentManager::switchTimer()
{
    //re-enables navigation.
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    switchEnabled = true;
}





TextViewer *ContentManager::textViewer() const
{
    return m_textViewer;
}

ImageViewer *ContentManager::imageViewer() const
{
    return m_imageViewer;
}

DocumentViewer *ContentManager::documentViewer() const
{
    return m_documentViewer;
}


QString ContentManager::currentViewer() const
{
    return m_currentViewer;
}

void ContentManager::setCurrentViewer(const QString &newCurrentViewer)
{
    if (m_currentViewer == newCurrentViewer)
        return;
    m_currentViewer = newCurrentViewer;
    Q_EMIT currentViewerChanged();
}

QString ContentManager::fileName() const
{
    return m_fileName;
}

void ContentManager::setFileName(const QString &newFileName)
{
    if (m_fileName == newFileName)
        return;
    m_fileName = newFileName;
    Q_EMIT fileNameChanged();
}


VideoViewer *ContentManager::videoViewer() const
{
    return m_videoViewer;
}

ContainerViewer *ContentManager::containerViewer() const
{
    return m_containerViewer;
}

QString ContentManager::windowTitle() const
{
    return m_windowTitle;
}

void ContentManager::setWindowTitle(const QString &newWindowTitle)
{
    if (m_windowTitle == newWindowTitle)
        return;
    m_windowTitle = newWindowTitle;
    Q_EMIT windowTitleChanged();
}


QString ContentManager::dolphinBridgeErrorMessage() const
{
    return m_dolphinBridgeErrorMessage;
}

void ContentManager::setDolphinBridgeErrorMessage(const QString &newDolphinBridgeErrorMessage)
{
    if (m_dolphinBridgeErrorMessage == newDolphinBridgeErrorMessage)
        return;
    m_dolphinBridgeErrorMessage = newDolphinBridgeErrorMessage;
    Q_EMIT dolphinBridgeErrorMessageChanged();
}

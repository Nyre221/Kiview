//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#ifndef CONTENTMANAGER_H
#define CONTENTMANAGER_H
#include <QObject>
#include <QDebug>
#include <filesystem>
#include <textviewer.h>
#include <imageviewer.h>
#include <documentviewer.h>
#include <videoviewer.h>
#include "containerviewer.h"
#include <QQuickItem>

class ContentManager : public QObject
{
    Q_OBJECT
    QML_SINGLETON
    QML_ELEMENT

    Q_DISABLE_COPY(ContentManager)

    Q_PROPERTY(TextViewer* textViewer READ textViewer CONSTANT FINAL)
    Q_PROPERTY(ImageViewer* imageViewer READ imageViewer CONSTANT FINAL)
    Q_PROPERTY(DocumentViewer* documentViewer READ documentViewer CONSTANT FINAL)
    Q_PROPERTY(VideoViewer* videoViewer READ videoViewer CONSTANT FINAL)
    Q_PROPERTY(ContainerViewer* containerViewer READ containerViewer CONSTANT FINAL)
    Q_PROPERTY(QString currentViewer READ currentViewer WRITE setCurrentViewer NOTIFY currentViewerChanged FINAL)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged FINAL)
    Q_PROPERTY(QString windowTitle READ windowTitle WRITE setWindowTitle NOTIFY windowTitleChanged FINAL)
    Q_PROPERTY(QString dolphinBridgeErrorMessage READ dolphinBridgeErrorMessage WRITE setDolphinBridgeErrorMessage NOTIFY dolphinBridgeErrorMessageChanged FINAL)

public:

    explicit ContentManager(QObject *parent = nullptr);

    void setFiles(std::string path, bool fileWasSelected);


    Q_INVOKABLE void goBack();
    Q_INVOKABLE void goForward();
    Q_INVOKABLE void openApp();
    Q_INVOKABLE void closing();



    TextViewer *textViewer() const;
    ImageViewer *imageViewer() const;
    DocumentViewer *documentViewer() const;
    VideoViewer *videoViewer() const;
    ContainerViewer *containerViewer() const;


    QString currentViewer() const;
    void setCurrentViewer(const QString &newCurrentViewer);
    QString fileName() const;
    void setFileName(const QString &newFileName);




    QString windowTitle() const;
    void setWindowTitle(const QString &newWindowTitle);

    QString dolphinBridgeErrorMessage() const;
    void setDolphinBridgeErrorMessage(const QString &newDolphinBridgeErrorMessage);

Q_SIGNALS:
    void currentViewerChanged();

    void fileNameChanged();

    void windowTitleChanged();

    void dolphinBridgeErrorMessageChanged();

private:
    TextViewer *m_textViewer = new TextViewer;
    DocumentViewer *m_documentViewer = new DocumentViewer;
    VideoViewer *m_videoViewer = new VideoViewer;
    ImageViewer *m_imageViewer = new ImageViewer;
    ContainerViewer *m_containerViewer = new ContainerViewer;

    QStringList searchPath(std::filesystem::path path);
    bool parentInIndex = false;
    bool indexReady = false;
    void switchTimer();
    bool switchEnabled = true;
    void stopViewers();
    void loadFileAtIndex( int index);
    std::string getFileExtension(std::filesystem::path filePath);
    std::filesystem::path currentFilePath;
    std::filesystem::path currentFile;

    QString m_currentViewer;
    QString m_fileName;
    QString m_windowTitle;
    QString m_dolphinBridgeErrorMessage;
};

#endif // CONTENTMANAGER_H

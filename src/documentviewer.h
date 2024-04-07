//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#ifndef DOCUMENTVIEWER_H
#define DOCUMENTVIEWER_H
#include <QProcess>
#include <QObject>
class DocumentViewer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString viewerDocument READ viewerDocument WRITE setViewerDocument NOTIFY viewerDocumentChanged FINAL)
    Q_PROPERTY(bool messagePageVisible READ messagePageVisible WRITE setMessagePageVisible NOTIFY messagePageVisibleChanged FINAL)
    Q_PROPERTY(QString messageHeaderText READ messageHeaderText WRITE setMessageHeaderText NOTIFY messageHeaderTextChanged FINAL)
    Q_PROPERTY(QString messageDescriptionText READ messageDescriptionText WRITE setMessageDescriptionText NOTIFY messageDescriptionTextChanged FINAL)
    Q_PROPERTY(bool reloadButtonVisible READ reloadButtonVisible WRITE setReloadButtonVisible NOTIFY reloadButtonVisibleChanged FINAL)
    Q_PROPERTY(bool loadingBarVisible READ loadingBarVisible WRITE setLoadingBarVisible NOTIFY loadingBarVisibleChanged FINAL)
    Q_PROPERTY(bool helpPageLinkVisible READ helpPageLinkVisible WRITE setHelpPageLinkVisible NOTIFY helpPageLinkVisibleChanged FINAL)


public:
    explicit DocumentViewer(QObject *parent = nullptr);
    void loadFile(std::string filePath,std::string extension);
    QString viewerDocument() const;
    void setViewerDocument(const QString &newViewerDocument);
    bool isInsideFlatpak = false;
    bool runningFlatpak();


    int activeThreadId;
    int currentProcessPid;
    bool processIsRunning = false;

    std::string currentFilePath;
    void loadingArrested(int errorCode, QString customError = QStringLiteral(""));
    void closeActiveConversionProcess();
    void stopViewer();
    Q_INVOKABLE void reloadDocument();
    bool isActiveViewer = false;
    void convertDocument(std::string path);

    void conversionFinished(std::string path);


    QString messageHeaderText() const;
    void setMessageHeaderText(const QString &newMessageHeaderText);

    bool messagePageVisible() const;
    void setMessagePageVisible(bool newMessagePageVisible);

    QString messageDescriptionText() const;
    void setMessageDescriptionText(const QString &newMessageDescriptionText);

    bool reloadButtonVisible() const;
    void setReloadButtonVisible(bool newReloadButtonVisible);

    bool loadingBarVisible() const;
    void setLoadingBarVisible(bool newLoadingBarVisible);



    bool helpPageLinkVisible() const;
    void setHelpPageLinkVisible(bool newHelpPageLinkVisible);

Q_SIGNALS:

    void viewerDocumentChanged();
    void messageHeaderTextChanged();

    void messagePageVisibleChanged();

    void messageDescriptionTextChanged();

    void reloadButtonVisibleChanged();

    void loadingBarVisibleChanged();


    void helpPageLinkVisibleChanged();

private:
    std::string getLibreofficeCommand();
    std::string tempDir;
    std::string libreofficeCommand;
    int randomId;
    void showLoadingScreen();
    QString m_viewerDocument;
    QString m_messageHeaderText;
    bool m_messagePageVisible;
    QString m_messageDescriptionText;
    bool m_reloadButtonVisible;
    bool m_loadingBarVisible;
    bool m_helpPageLinkVisible;
};

#endif // DOCUMENTVIEWER_H

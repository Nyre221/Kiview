//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H
#include <QObject>

class ImageViewer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString viewerImage READ viewerImage WRITE setViewerImage NOTIFY viewerImageChanged FINAL)
    Q_PROPERTY(QString errorMessageText READ errorMessageText WRITE setErrorMessageText NOTIFY errorMessageTextChanged FINAL)

public:
    explicit ImageViewer(QObject *parent = nullptr);
    void loadFile(std::string filePath,std::string extension);
    int activeThreadId;
    int randomId;
    bool isActiveViewer = false;
    void stopViewer();
    void closeActiveSubProcess();
    int currentProcessPid;
    bool processIsRunning = false;

    QString viewerImage() const;
    void setViewerImage(const QString &newViewerImage);
    void extractPreview(std::string path);


    QString errorMessageText() const;
    void setErrorMessageText(const QString &newErrorMessageText);

Q_SIGNALS:

    void viewerImageChanged();

    void errorMessageTextChanged();

private:
    std::string tempDir;
    QString m_viewerImage;
    QString m_errorMessageText;
};

#endif // IMAGEVIEWER_H

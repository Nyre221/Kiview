//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#ifndef CONTAINERVIEWER_H
#define CONTAINERVIEWER_H

#include <QObject>

class ContainerViewer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList viewerElement READ viewerElement WRITE setViewerElement NOTIFY viewerElementChanged FINAL)
    Q_PROPERTY(QString fileIcon READ fileIcon WRITE setFileIcon NOTIFY fileIconChanged FINAL)
    Q_PROPERTY(QString fileSize READ fileSize WRITE setFileSize NOTIFY fileSizeChanged FINAL)
    Q_PROPERTY(QString fileSizeUnit READ fileSizeUnit WRITE setFileSizeUnit NOTIFY fileSizeUnitChanged FINAL)
    Q_PROPERTY(QString fileContentCount READ fileContentCount WRITE setFileContentCount NOTIFY fileContentCountChanged FINAL)
    Q_PROPERTY(QString fileLastModified READ fileLastModified WRITE setFileLastModified NOTIFY fileLastModifiedChanged FINAL)
    Q_PROPERTY(QString errorMessageText READ errorMessageText WRITE setErrorMessageText NOTIFY errorMessageTextChanged FINAL)

public:
    explicit ContainerViewer(QObject *parent = nullptr);
    void loadFile(std::string filePath, bool isArchive, std::string extension = "");
    int activeThreadId;
    int randomId;
    void stopViewer();
    void closeActiveSubProcess();
    int currentProcessPid;
    bool isActiveViewer = false;
    bool processIsRunning = false;


    QStringList viewerElement() const;
    void setViewerElement(const QStringList &newViewerElement);

    QString fileIcon() const;
    void setFileIcon(const QString &newFileIcon);

    QString fileSize() const;
    void setFileSize(const QString &newFileSize);

    QString fileSizeUnit() const;
    void setFileSizeUnit(const QString &newFileSizeUnit);

    QString fileContentCount() const;
    void setFileContentCount(const QString &newFileContentCount);

    QString fileLastModified() const;
    void setFileLastModified(const QString &newFileLastModified);

    QString errorMessageText() const;
    void setErrorMessageText(const QString &newErrorMessageText);

signals:

    void viewerElementChanged();
    void fileIconChanged();

    void fileSizeChanged();

    void fileSizeUnitChanged();

    void fileContentCountChanged();

    void fileLastModifiedChanged();

    void errorMessageTextChanged();

private:
    void resetView();
    void startThread(std::string path, bool isArchive, std::string extension);

    QStringList m_viewerElement;
    QString m_fileIcon;
    QString m_fileSize;
    QString m_fileSizeUnit;
    QString m_fileContentCount;
    QString m_fileLastModified;
    QString m_errorMessageText;
};

#endif // CONTAINERVIEWER_H

//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#ifndef VIDEOVIEWER_H
#define VIDEOVIEWER_H

#include <QObject>

class VideoViewer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString viewerMedia READ viewerText WRITE setViewerMedia NOTIFY viewerMediaChanged FINAL)

public:
    explicit VideoViewer(QObject *parent = nullptr);
    void loadFile(std::string filePath);

    QString viewerText() const;
    void setViewerMedia(const QString &newViewerMedia);

signals:

    void viewerMediaChanged();
private:
    QString m_viewerMedia;
};

#endif // VIDEOVIEWER_H

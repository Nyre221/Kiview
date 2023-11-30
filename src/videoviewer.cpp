//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#include "videoviewer.h"

VideoViewer::VideoViewer(QObject *parent)
    : QObject{parent}
{

}

void VideoViewer::loadFile(std::string filePath)
{
    QString path;
    path = QString::fromStdString("file://"+filePath);
    setViewerMedia(path);
}

QString VideoViewer::viewerText() const
{
    return m_viewerMedia;
}

void VideoViewer::setViewerMedia(const QString &newViewerMedia)
{
    if (m_viewerMedia == newViewerMedia)
        return;
    m_viewerMedia = newViewerMedia;
    emit viewerMediaChanged();
}

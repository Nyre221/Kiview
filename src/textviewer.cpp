//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#include "textviewer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <QDebug>

using namespace std;

TextViewer::TextViewer(QObject *parent)
    : QObject{parent}
{
}




void TextViewer::loadFile(std::string filePath){
    ifstream file;
    string line;
    QString text;
    //read the file
    file.open (filePath);
    {
        while(getline(file, line)) {
            text.append(QString::fromStdString(line+"\n"));
        }
        file.close();
    }
    //sets the text
    setViewerText(text);

}

QString TextViewer::viewerText() const
{
    return m_viewerText;
}

void TextViewer::setViewerText(const QString &newViewerText)
{
    if (m_viewerText == newViewerText)
        return;
    m_viewerText = newViewerText;
    emit viewerTextChanged();
}

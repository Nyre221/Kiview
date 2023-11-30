//SPDX-License-Identifier: GPL-3.0-or-later
//SPDX-FileCopyrightText: 2023 danilo agostini <nyre334@gmail.com>
#ifndef TEXTVIEWER_H
#define TEXTVIEWER_H
#include <QObject>

class TextViewer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString viewerText READ viewerText WRITE setViewerText NOTIFY viewerTextChanged FINAL)
public:
    explicit TextViewer(QObject *parent = nullptr);

    QString viewerText() const;
    void setViewerText(const QString &newViewerText);
    void loadFile(std::string filePath);

signals:

    void viewerTextChanged();
private:
    QString m_viewerText;
};

#endif // TEXTVIEWER_H

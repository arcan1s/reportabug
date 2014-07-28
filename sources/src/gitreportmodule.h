/***************************************************************************
 * This file is part of reportabug                                         *
 *                                                                         *
 * This library is free software; you can redistribute it and/or           *
 * modify it under the terms of the GNU Lesser General Public              *
 * License as published by the Free Software Foundation; either            *
 * version 3.0 of the License, or (at your option) any later version.      *
 *                                                                         *
 * This library is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU        *
 * Lesser General Public License for more details.                         *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with this library.                                        *
 ***************************************************************************/

#ifndef GITREPORTMODULE_H
#define GITREPORTMODULE_H

#include <QMap>
#include <QObject>
#include <QWebView>

class Reportabug;

class GitreportModule : public QObject
{
    Q_OBJECT

public:
    explicit GitreportModule(QWidget *parent = 0,
                             bool debugCmd = false);
    ~GitreportModule();
    QWebView *webView;

public slots:
    void sendReportUsingGitreport(const QMap<QString, QString> info);

private slots:
    void gitreportFinished(const bool state);
    void gitreportLoaded(const bool state);

private:
    bool debug;
    Reportabug *mainWindow;
};


#endif /* GITREPORTMODULE_H */

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

#ifndef REPORTABUG_H
#define REPORTABUG_H

#include <QKeyEvent>
#include <QMainWindow>

#include "config.h"

#ifdef ENABLE_GITHUB
class QNetworkReply;
#endif /* ENABLE_GITHUB */
#ifdef ENABLE_GITREPORT
class QWebView;
#endif /* ENABLE_GITREPORT */
namespace Ui {
class Reportabug;
}

class Reportabug : public QMainWindow
{
    Q_OBJECT

public:
    explicit Reportabug(QWidget *parent = 0,
                        bool debugCmd = false);
    ~Reportabug();

public slots:
    void sendReport();
    void showWindow();
    void updateTabs(const int index);

private slots:
#ifdef ENABLE_GITHUB
    void sendReportUsingGithub();
    void githubFinished(QNetworkReply *reply);
#endif /* ENABLE_GITHUB */
#ifdef ENABLE_GITREPORT
    void sendReportUsingGitreport();
    void gitreportFinished(const bool state);
    void gitreportLoaded(const bool state);
#endif /* ENABLE_GITREPORT */

private:
    bool debug;
    bool modules[2];
#ifdef ENABLE_GITREPORT
    // add webview which is required by gitreport module
    QWebView *webView;
#endif /* ENABLE_GITREPORT */
    Ui::Reportabug *ui;
    void createActions();
    void createComboBox();
    int getNumberByIndex(const int index);
    void initModules();
    void keyPressEvent(QKeyEvent *pressedKey);
    QString parseString(QString line);
    QByteArray prepareRequest(const QString title, const QString body);
};


#endif /* REPORTABUG_H */

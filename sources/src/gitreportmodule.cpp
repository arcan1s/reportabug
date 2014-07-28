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

#include "gitreportmodule.h"
#include "reportabug.h"

#include <QApplication>
#include <QDebug>
#include <QGridLayout>
#include <QMessageBox>
#include <QPushButton>
#include <QUrl>
#include <QWebElement>
#include <QWebFrame>

#include "config.h"


GitreportModule::GitreportModule(QWidget *parent, bool debugCmd)
    : QObject(parent),
      debug(debugCmd),
      mainWindow((Reportabug *)parent)
{
    webView = new QWebView();
}


GitreportModule::~GitreportModule()
{
    if (debug) qDebug() << "[GitreportModule]" << "[~GitreportModule]";

    delete webView;
}


void GitreportModule::sendReportUsingGitreport(const QMap<QString, QString> info)
{
    if (debug) qDebug() << "[GitreportModule]" << "[sendReportUsingGitreport]";

    QWebElement document = webView->page()->mainFrame()->documentElement();
    QWebElement captcha = document.findFirst(QString("input#captcha"));
    QWebElement captchaImg = document.findFirst(QString("img#[alt=captcha]"));
    QWebElement captchaKey = document.findFirst(QString("input#captcha_key"));
    QWebElement emailInput = document.findFirst(QString("input#email"));
    QWebElement textArea = document.findFirst(QString("textarea#details"));
    QWebElement usernameInput = document.findFirst(QString("input#name"));

    // input
    usernameInput.setAttribute(QString("value"), info[QString("username")]);
    emailInput.setAttribute(QString("value"), info[QString("password")]);
    textArea.setPlainText(info[QString("body")]);
    // captcha
    captchaImg.setAttribute(QString("src"), QString("/simple_captcha?code=%1&amp;time=%2")
                            .arg(QString(CAPTCHA_KEY))
                            .arg(QString(CAPTCHA_TIME)));
    captchaKey.setAttribute(QString("value"), QString(CAPTCHA_KEY));
    captcha.setAttribute(QString("value"), QString(CAPTCHA_TEXT));

    // send request
    document.findFirst(QString("input[name=commit]")).evaluateJavaScript("this.click()");
    disconnect(webView, SIGNAL(loadFinished(bool)), this, SLOT(gitreportLoaded(bool)));
    disconnect(webView, SIGNAL(loadFinished(bool)), this, SLOT(gitreportFinished(bool)));
    connect(webView, SIGNAL(loadFinished(bool)), this, SLOT(gitreportFinished(bool)));
}


void GitreportModule::gitreportLoaded(const bool state)
{
    if (debug) qDebug() << "[GitreportModule]" << "[gitreportLoaded]";
    if (debug) qDebug() << "[GitreportModule]" << "[gitreportLoaded]" << ":" << "State" << state;

    if (state)
        webView->setHidden(!debug);
    else {
        QMessageBox messageBox;
        messageBox.setText(QApplication::translate("Reportabug", "Error!"));
        messageBox.setInformativeText(QApplication::translate("Reportabug", "An error occurs"));
        messageBox.setIcon(QMessageBox::Critical);
        messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Retry);
        messageBox.setDefaultButton(QMessageBox::Ok);
        QSpacerItem *horizontalSpacer = new QSpacerItem(400, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        QGridLayout *layout = (QGridLayout *)messageBox.layout();
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
        messageBox.exec();
    }
}


void GitreportModule::gitreportFinished(const bool state)
{
    if (debug) qDebug() << "[GitreportModule]" << "[gitreportFinished]";
    if (debug) qDebug() << "[GitreportModule]" << "[gitreportFinished]" << ":" << "State" << state;

    QString messageBody, messageTitle;
    QMessageBox::Icon icon = QMessageBox::NoIcon;
    if (state) {
        messageBody += QApplication::translate("Reportabug", "Message has been sended");
        messageTitle = QApplication::translate("Reportabug", "Done!");
        icon = QMessageBox::Information;
    }
    else {
        messageBody += QApplication::translate("Reportabug", "An error occurs");
        messageTitle = QApplication::translate("Reportabug", "Error!");
        icon = QMessageBox::Critical;
    }

    QMessageBox messageBox;
    messageBox.setText(messageTitle);
    messageBox.setInformativeText(messageBody);
    messageBox.setIcon(icon);
    messageBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Retry);
    messageBox.setDefaultButton(QMessageBox::Ok);
    QSpacerItem *horizontalSpacer = new QSpacerItem(400, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout *layout = (QGridLayout *)messageBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    int ret = messageBox.exec();

    switch (ret) {
    case QMessageBox::Ok:
        if (state) mainWindow->close();
        break;
    case QMessageBox::Retry:
        if (state) mainWindow->externalUpdateTab();
        break;
    default:
        break;
    }
}

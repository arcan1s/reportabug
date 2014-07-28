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

#include "reportabug.h"
#include "ui_reportabug.h"

#include <QUrl>
#include <QWebElement>
#include <QWebFrame>
#include <QWebView>

#include "config.h"


void Reportabug::sendReportUsingGitreport()
{
    if (debug) qDebug() << "[Reportabug]" << "[sendReportUsingGitreport]";

    QWebElement document = webView->page()->mainFrame()->documentElement();
    QWebElement captcha = document.findFirst(QString("input#captcha"));
    QWebElement captchaImg = document.findFirst(QString("img#[alt=captcha]"));
    QWebElement captchaKey = document.findFirst(QString("input#captcha_key"));
    QWebElement emailInput = document.findFirst(QString("input#email"));
    QWebElement textArea = document.findFirst(QString("textarea#details"));
    QWebElement usernameInput = document.findFirst(QString("input#name"));

    // input
    usernameInput.setAttribute(QString("value"), ui->lineEdit_username->text());
    emailInput.setAttribute(QString("value"), ui->lineEdit_password->text());
    textArea.setPlainText(ui->textEdit->toPlainText());
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


void Reportabug::gitreportLoaded(const bool state)
{
    if (debug) qDebug() << "[Reportabug]" << "[gitreportLoaded]";
    if (debug) qDebug() << "[Reportabug]" << "[gitreportLoaded]" << ":" << "State" << state;

    if (state) {
        ui->widget_auth->setHidden(false);
        ui->widget_title->setHidden(true);
        ui->textEdit->setHidden(false);
        webView->setHidden(!debug);
        ui->label_password->setText(QApplication::translate("Reportabug", "Email"));
        ui->label_password->setToolTip(QApplication::translate("Reportabug", "Your email"));
        ui->lineEdit_password->setPlaceholderText(QApplication::translate("Reportabug", "email"));
        ui->lineEdit_password->setEchoMode(QLineEdit::Normal);
    }
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


void Reportabug::gitreportFinished(const bool state)
{
    if (debug) qDebug() << "[Reportabug]" << "[gitreportFinished]";
    if (debug) qDebug() << "[Reportabug]" << "[gitreportFinished]" << ":" << "State" << state;

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
        if (state) close();
        break;
    case QMessageBox::Retry:
        if (state) updateTabs(ui->comboBox->currentIndex());
        break;
    default:
        break;
    }
}

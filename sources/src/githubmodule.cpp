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

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrl>

#include "config.h"


void Reportabug::sendReportUsingGithub()
{
    if (debug) qDebug() << "[Reportabug]" << "[sendReportUsingGithub]";

    // authentication
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QString concatenated = username + QString(":") + password;
    QByteArray userData = concatenated.toLocal8Bit().toBase64();
    QString headerData = QString("Basic ") + userData;
    // text
    QString title = ui->lineEdit_title->text();
    QString body = ui->textEdit->toPlainText();
    QByteArray text = prepareRequest(title, body);
    QByteArray textSize = QByteArray::number(text.size());
    // sending request
    QNetworkRequest request = QNetworkRequest(parseString(QString(ISSUES_URL)));
    request.setRawHeader("Authorization", headerData.toLocal8Bit());
    request.setRawHeader("User-Agent", "reportabug");
    request.setRawHeader("Host", "api.github.com");
    request.setRawHeader("Accept", "*/*");
    request.setRawHeader("Content-Type", "application/vnd.github.VERSION.raw+json");
    request.setRawHeader("Content-Length", textSize);
    QNetworkAccessManager *manager = new QNetworkAccessManager;
    manager->post(request, text);
    disconnect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(githubFinished(QNetworkReply *)));
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(githubFinished(QNetworkReply *)));
}


void Reportabug::githubFinished(QNetworkReply *reply)
{
    if (debug) qDebug() << "[Reportabug]" << "[githubFinished]";
    if (debug) qDebug() << "[Reportabug]" << "[githubFinished]" << ":" << "Error state" << reply->error();
    if (debug) qDebug() << "[Reportabug]" << "[githubFinished]" << ":" << "Reply size" << reply->readBufferSize();

    int state = true;
    QString answer = reply->readAll();
    if (debug) qDebug() << "[Reportabug]" << "[replyFinished]" << ":" << answer;
    QString messageBody, messageTitle;
    QMessageBox::Icon icon = QMessageBox::NoIcon;
    if (answer.contains(QString("\"html_url\":"))) {
        QString url;
        for (int i=0; i<answer.split(QChar(',')).count(); i++)
            if (answer.split(QChar(','))[i].split(QChar(':'))[0] == QString("\"html_url\"")) {
                url = answer.split(QChar(','))[i].split(QString("\"html_url\":"))[1].remove(QChar('"'));
                break;
            }
        messageBody += QString("%1\n").arg(QApplication::translate("Reportabug", "Message has been sended"));
        messageBody += QString("Url: %2").arg(url);
        messageTitle = QApplication::translate("Reportabug", "Done!");
        icon = QMessageBox::Information;
        state = true;
    }
    else if (answer.contains(QString("\"Bad credentials\""))) {
        messageBody += QApplication::translate("Reportabug", "Incorrect username or password");
        messageTitle = QApplication::translate("Reportabug", "Error!");
        icon = QMessageBox::Critical;
        state = false;
    }
    else {
        messageBody += QApplication::translate("Reportabug", "An error occurs");
        messageTitle = QApplication::translate("Reportabug", "Error!");
        icon = QMessageBox::Critical;
        state = false;
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

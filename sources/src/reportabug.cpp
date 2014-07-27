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
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QUrl>

#include "config.h"


Reportabug::Reportabug(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::Reportabug)
{
    ui->setupUi(this);
    createComboBox();
    createActions();
    updateTabs(ui->comboBox->currentIndex());
}


Reportabug::~Reportabug()
{
    delete ui;
}


void Reportabug::createActions()
{
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTabs(int)));
    connect(ui->buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(sendReport()));
}


void Reportabug::createComboBox()
{
    ui->comboBox->clear();
    if (ENABLE_GITHUB)
        ui->comboBox->addItem(QString(GITHUB_COMBOBOX));
    if (ENABLE_GITREPORT)
        ui->comboBox->addItem(QString(GITREPORT_COMBOBOX));
}


int Reportabug::getNumberByIndex(const int index)
{
    if (index == -1)
        // nothing to do
        return -1;
    else if ((ENABLE_GITHUB) && (ENABLE_GITREPORT))
        // both are enabled
        return index;
    else if (ENABLE_GITHUB)
        // only github is enabled
        return 0;
    else if (ENABLE_GITREPORT)
        // only gitreport is enabled
        return 1;
    else
        // nothing to do
        return -1;
}


// ESC press event
void Reportabug::keyPressEvent(QKeyEvent *pressedKey)
{
    if (pressedKey->key() == Qt::Key_Escape)
        close();
}


QString Reportabug::parseCmd(QString line,
                             const QString password,
                             const QString text,
                             const QString username)
{
    if (line.contains(QString("$ISSUES_URL")))
        line = line.split(QString("$ISSUES_URL"))[0] +
                parseString(QString(ISSUES_URL)) +
                line.split(QString("$ISSUES_URL"))[1];
    if (line.contains(QString("$PASSWORD")))
        line = line.split(QString("$PASSWORD"))[0] +
                password +
                line.split(QString("$PASSWORD"))[1];
    if (line.contains(QString("$TEXT")))
        line = line.split(QString("$TEXT"))[0] +
                text +
                line.split(QString("$TEXT"))[1];
    if (line.contains(QString("$USERNAME")))
        line = line.split(QString("$USERNAME"))[0] +
                username +
                line.split(QString("$USERNAME"))[1];

    return line;
}


QString Reportabug::parseString(QString line)
{
    if (line.contains(QString("$OWNER")))
        line = line.split(QString("$OWNER"))[0] +
                QString(OWNER) +
                line.split(QString("$OWNER"))[1];
    if (line.contains(QString("$PROJECT")))
        line = line.split(QString("$PROJECT"))[0] +
                QString(PROJECT) +
                line.split(QString("$PROJECT"))[1];

    return line;
}


QByteArray Reportabug::prepareRequest(const QString title, const QString body)
{
    QStringList requestList;
    requestList.append(QString("\"title\":\"") + title + QString("\""));
    requestList.append(QString("\"body\":\"") + body + QString("\""));
    if (!QString(TAG_ASSIGNEE).isEmpty())
        requestList.append(QString("\"assignee\":\"") + parseString(QString(TAG_ASSIGNEE)) + QString("\""));
    if (!QString(TAG_MILESTONE).isEmpty())
        requestList.append(QString("\"milestone\":") + QString(TAG_MILESTONE));
    if (!QString(TAG_LABELS).isEmpty()) {
        QStringList labels = QString(TAG_LABELS).split(QChar(','));
        for (int i=0; i<labels.count(); i++)
            labels[i] = QString("\"") + labels[i] + QString("\"");
        requestList.append(QString("\"labels\":[") + labels.join(QChar(',')) + QString("]"));
    }

    QString request;
    request += QString("{");
    request += requestList.join(QChar(','));
    request += QString("}");

    return request.toLatin1();
}


void Reportabug::sendReport()
{
    int number = getNumberByIndex(ui->comboBox->currentIndex());
    if (number == -1)
        return;
    else if (number == 0)
        sendReportUsingGithub();
    else if (number == 1)
        sendReportUsingGitreport();
}


void Reportabug::updateTabs(const int index)
{
    int number = getNumberByIndex(index);
    ui->stackedWidget->setCurrentIndex(number + 1);
    if (number == -1)
        return;
    else if (number == 0)
        updateGithubTab();
    else if (number == 1)
        updateGitreportTab();
}


void Reportabug::updateGithubTab()
{
    ui->lineEdit_username->clear();
    ui->lineEdit_password->clear();
    ui->lineEdit_title->setText(QString(TAG_TITLE));
    ui->plainTextEdit->setPlainText(QString(TAG_BODY));
}


void Reportabug::updateGitreportTab()
{

}


void Reportabug::replyFinished(QNetworkReply *reply)
{
    int state = true;
    QString answer = reply->readAll();
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


void Reportabug::sendReportUsingGithub()
{
    // authorization
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();
    QString concatenated = username + QString(":") + password;
    QByteArray userData = concatenated.toLocal8Bit().toBase64();
    QString headerData = QString("Basic ") + userData;
    // text
    QString title = ui->lineEdit_title->text();
    QString body = ui->plainTextEdit->toPlainText();
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
    connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(replyFinished(QNetworkReply *)));
}


void Reportabug::sendReportUsingGitreport()
{

}

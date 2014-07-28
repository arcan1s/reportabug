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
#include <QWebElement>
#include <QWebFrame>

#include "config.h"


Reportabug::Reportabug(QWidget *parent, bool debugCmd)
    : QMainWindow(parent),
      debug(debugCmd),
      ui(new Ui::Reportabug)
{
    ui->setupUi(this);
    createComboBox();
    createActions();
    updateTabs(ui->comboBox->currentIndex());
}


Reportabug::~Reportabug()
{
    if (debug) qDebug() << "[Reportabug]" << "[~Reportabug]";

    delete ui;
}


void Reportabug::createActions()
{
    if (debug) qDebug() << "[Reportabug]" << "[createActions]";

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateTabs(int)));
    connect(ui->buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(sendReport()));
}


void Reportabug::createComboBox()
{
    if (debug) qDebug() << "[Reportabug]" << "[createComboBox]";

    ui->comboBox->clear();
    if (ENABLE_GITHUB)
        ui->comboBox->addItem(QString(GITHUB_COMBOBOX));
    if (ENABLE_GITREPORT)
        ui->comboBox->addItem(QString(GITREPORT_COMBOBOX));
}


int Reportabug::getNumberByIndex(const int index)
{
    if (debug) qDebug() << "[Reportabug]" << "[getNumberByIndex]";
    if (debug) qDebug() << "[Reportabug]" << "[getNumberByIndex]" << ":" << "Index" << index;

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
    if (debug) qDebug() << "[Reportabug]" << "[keyPressEvent]";

    if (pressedKey->key() == Qt::Key_Escape)
        close();
}


QString Reportabug::parseString(QString line)
{
    if (debug) qDebug() << "[Reportabug]" << "[parseString]";
    if (debug) qDebug() << "[Reportabug]" << "[parseString]" << ":" << "Parse line" << line;

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
    if (debug) qDebug() << "[Reportabug]" << "[prepareRequest]";
    if (debug) qDebug() << "[Reportabug]" << "[prepareRequest]" << ":" << "Title" << title;
    if (debug) qDebug() << "[Reportabug]" << "[prepareRequest]" << ":" << "Title" << body;

    QStringList requestList;
    requestList.append(QString("\"title\":\"") + title + QString("\""));
    QString fixBody = body;
    fixBody.replace(QString("\n"), QString("<br>"));
    requestList.append(QString("\"body\":\"") + fixBody + QString("\""));
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
    if (debug) qDebug() << "[Reportabug]" << "[sendReport]";

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
    if (debug) qDebug() << "[Reportabug]" << "[updateTabs]";
    if (debug) qDebug() << "[Reportabug]" << "[updateTabs]" << ":" << "Index" << index;

    int number = getNumberByIndex(index);

    if (number == -1) {
        ui->widget_auth->setHidden(true);
        ui->widget_title->setHidden(true);
        ui->textEdit->setHidden(true);
        ui->webView->setHidden(true);
    }
    else if (number == 0) {
        ui->widget_auth->setHidden(false);
        ui->widget_title->setHidden(false);
        ui->textEdit->setHidden(false);
        ui->webView->setHidden(true);
        ui->label_password->setText(QApplication::translate("Reportabug", "Password"));
        ui->label_password->setToolTip(QApplication::translate("Reportabug", "GitHub account password"));
        ui->lineEdit_password->setPlaceholderText(QApplication::translate("Reportabug", "password"));
        ui->lineEdit_password->setEchoMode(QLineEdit::Password);

        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->lineEdit_title->setText(QString(TAG_TITLE));
        ui->textEdit->setPlainText(QString(TAG_BODY));
    }
    else if (number == 1) {
        ui->widget_auth->setHidden(true);
        ui->widget_title->setHidden(true);
        ui->textEdit->setHidden(true);
        ui->webView->setHidden(true);

        ui->lineEdit_username->clear();
        ui->lineEdit_password->clear();
        ui->textEdit->setPlainText(QString(TAG_BODY));
        ui->webView->load(QUrl(parseString(QString(PUBLIC_URL))));
        disconnect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(gitreportLoaded(bool)));
        disconnect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(gitreportFinished(bool)));
        connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(gitreportLoaded(bool)));
    }
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


void Reportabug::gitreportLoaded(const bool state)
{
    if (debug) qDebug() << "[Reportabug]" << "[gitreportLoaded]";
    if (debug) qDebug() << "[Reportabug]" << "[gitreportLoaded]" << ":" << "State" << state;

    if (state) {
        ui->widget_auth->setHidden(false);
        ui->widget_title->setHidden(true);
        ui->textEdit->setHidden(false);
        ui->webView->setHidden(false);
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


void Reportabug::sendReportUsingGitreport()
{
    if (debug) qDebug() << "[Reportabug]" << "[sendReportUsingGitreport]";

    QWebElement document = ui->webView->page()->mainFrame()->documentElement();
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
    disconnect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(gitreportLoaded(bool)));
    disconnect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(gitreportFinished(bool)));
    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(gitreportFinished(bool)));
}

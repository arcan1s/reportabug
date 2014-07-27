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
#include <QPushButton>

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
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(close()));
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


QString Reportabug::parseString(QString line)
{
    if (line.contains(QString("$OWNER")))
        line = line.split(QString("$OWNER"))[0] + QString(OWNER) + line.split(QString("$OWNER"))[1];
    if (line.contains(QString("$PROJECT")))
        line = line.split(QString("$PROJECT"))[0] + QString(PROJECT) + line.split(QString("$PROJECT"))[1];

    return line;
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
        sendReportUsingGithub();
    else if (number == 1)
        sendReportUsingGitreport();
}


void Reportabug::updateGithubTab()
{

}


void Reportabug::updateGitreportTab()
{

}


void Reportabug::sendReportUsingGithub()
{

}


void Reportabug::sendReportUsingGitreport()
{

}

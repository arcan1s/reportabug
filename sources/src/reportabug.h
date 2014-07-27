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


namespace Ui {
class Reportabug;
}

class Reportabug : public QMainWindow
{
    Q_OBJECT

public:
    explicit Reportabug(QWidget *parent = 0);
    ~Reportabug();

public slots:
    // send report
    void sendReport();
    // update functions
    void updateTabs(const int index);
    void updateGithubTab();
    void updateGitreportTab();

private slots:
    void sendReportUsingGithub();
    void sendReportUsingGitreport();

private:
    Ui::Reportabug *ui;
    void createActions();
    void createComboBox();
    int getNumberByIndex(const int index);
    // ESC pressed event
    void keyPressEvent(QKeyEvent *pressedKey);
    QString parseString(QString line);
};


#endif /* REPORTABUG_H */

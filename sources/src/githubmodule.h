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

#ifndef GITHUBMODULE_H
#define GITHUBMODULE_H

#include <QMap>
#include <QObject>


class Reportabug;
class QNetworkReply;

class GithubModule : public QObject
{
    Q_OBJECT

public:
    explicit GithubModule(QWidget *parent = 0,
                          bool debugCmd = false);
    ~GithubModule();

public slots:
    void sendReportUsingGithub(const QMap<QString, QString> info);

private slots:
    void githubFinished(QNetworkReply *reply);

private:
    bool debug;
    Reportabug *mainWindow;
    QString parseString(QString line);
    QByteArray prepareRequest(const QString title, const QString body);
};


#endif /* GITHUBMODULE_H */

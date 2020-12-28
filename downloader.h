/*
 * Copyright (c) 2020 - Uwe Nagel
 *
 * This file is part of HomeMaticBatteriestatus
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * See the LICENSE file for more details.
*/

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QEventLoop>
#include <QSettings>
#include <QDir>
#include "SmtpMime"

class Downloader : public QObject
{
    Q_OBJECT
public:
    explicit Downloader(QObject *parent = nullptr);
    void fetch();

signals:
    void finished();

public slots:
    void replyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager * manager;
    QNetworkAccessManager * manager1;
    void sendMessage(QString str);
    QFile logfile;
    void getSettings();
    QString sCCU_IP;
    QString sCCU_Group;
    QString sSMTP_Host;
    int iSMTP_Port;
    QString sSMTP_User;
    QString sSMTP_Pwd;
    QString sSMTP_From;
    QString sSMTP_To;

};

#endif // DOWNLOADER_H

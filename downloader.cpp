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

#include "downloader.h"

//
// Constructor initializes the application
//

Downloader::Downloader(QObject *parent) : QObject(parent)
{
    QTextStream out(stdout);
    //
    // Create and open logfile in home directory
    //
    logfile.setFileName(QDir::homePath() + "/HomeMaticBatteriestatus.log");
    if (!logfile.open(QIODevice::Text|QIODevice::Append)){
        out << tr("Error on opening log file") << endl;
        emit finished();
    }
    QTextStream logstream(&logfile);
    logstream << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm ") << "Start HomeMaticBatteriestatus" << endl;

    getSettings();

}

//
// Subroutine called from the main program to do the web call
//

void Downloader::fetch()
{
    //
    // Fetches the list of all groups from the CCU
    //
    manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl(QString("http://"+ sCCU_IP +"/addons/xmlapi/functionlist.cgi"))));
}

//
// Web reply received
//

void Downloader::replyFinished(QNetworkReply *reply)
{
    QByteArray data = reply->readAll();
    QXmlStreamReader stream(data);
    QTextStream logstream(&logfile);
    //
    // Put all devices in the defined group into devlist
    //
    QStringList devlist;
    while (!stream.atEnd() && !stream.hasError()) {
        if(stream.readNextStartElement()){
            if(stream.name()=="function" && stream.attributes().value("name") == sCCU_Group){
                do{
                    stream.readNextStartElement();
                    QString ret = stream.attributes().value("address").toString();
                    if(!ret.isEmpty()) devlist.append(ret);
                }        while (stream.name()!="function");
            }
        }
    }
    //
    // Loop through devlist
    //
    if (devlist.count()!=0){
        for (QStringList::iterator it=devlist.begin();it!=devlist.end();it++){
            QString str = QString("http://" +sCCU_IP + "/addons/xmlapi/state.cgi?datapoint_id=BidCos-RF.%1:0.LOWBAT").arg(it->left(10));
            //
            // Call battery status for device from devlist
            //
            manager1= new QNetworkAccessManager(this);
            QNetworkReply *reply = manager1->get(QNetworkRequest(QUrl(str)));
            QEventLoop loop;
            connect(reply,SIGNAL(finished()),&loop,SLOT(quit()));
            loop.exec();
            //
            // Reply received, scan through result.
            if(!reply->error()){
                stream.clear();
                stream.addData(reply->readAll());
                while(!stream.atEnd()&& ! stream.hasError()){
                    if(stream.readNextStartElement()&&stream.name()=="datapoint"){
                        if(stream.attributes().value("value").toString()!="false"){ // "...LOWBAT is not false thus battery is low => send Mail
                            sendMessage(stream.attributes().value("ise_id").toString().mid(10,10));
                        }
                    }
                }
            }
            else{
                logstream << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm ") << QString(tr("Error at item %1")).arg(it->left(10)) << endl;
            }
        }
    }
    else{
        logstream << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm ") << QString(tr("CCU group cannot be read")) << endl;
    }
    logfile.close();
    emit finished();
}

//
// Subroutine to create mail
//

void Downloader::sendMessage(QString mailStr){


    QTextStream logstream(&logfile);
    SmtpClient smtp(sSMTP_Host,iSMTP_Port,SmtpClient::SslConnection);
    smtp.setUser(sSMTP_User);
    smtp.setPassword(sSMTP_Pwd);
    MimeMessage message;
    EmailAddress sender(sSMTP_From,sSMTP_From.left(sSMTP_From.indexOf("@")));
    message.setSender(&sender);
    EmailAddress to(sSMTP_To,sSMTP_To.left(sSMTP_To.indexOf("@")));
    message.addRecipient(&to);
    message.setSubject("HomeMaticBatteriestatus");
    MimeText text;
    text.setText(QString(tr("Empty battery for %1! Please change")).arg(mailStr));
    message.addPart(&text);
    if (!smtp.connectToHost()) {
            logstream << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm ") << tr("Failed to connect to host!") << endl;
        }

    if (!smtp.login()) {
            logstream <<QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm ")<< tr("Failed to login!") << endl;
        }

    if (!smtp.sendMail(message)) {
            logstream << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm ") << tr("Failed to send mail!") << endl;
        }

    smtp.quit();

}

//
// Subroutine to read all settings from the ini file
//

void Downloader::getSettings()
{
    QTextStream logstream(&logfile);
    if (QFile(QDir::homePath()+"/QtCreatorProjects/HomeMaticBatteriestatus/config.ini").exists()){
        QSettings    pConfig(QDir::homePath()+"/QtCreatorProjects/HomeMaticBatteriestatus/config.ini",QSettings::IniFormat);
        sCCU_IP = pConfig.value("CCU_IP").toString();
        sCCU_Group = pConfig.value("CCU_Group").toString();
        sSMTP_Host = pConfig.value("SMTP_Host").toString();
        iSMTP_Port = pConfig.value("SMTP_Port").toInt();
        sSMTP_User = pConfig.value("SMTP_User").toString();
        sSMTP_Pwd = pConfig.value("SMTP_Pwd").toString();
        sSMTP_From = pConfig.value("SMTP_From").toString();
        sSMTP_To = pConfig.value("SMTP_To").toString();
    }
    else{
        logstream << QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm ") << tr("Failed to open config file!") << endl;
        emit finished();
    }
}

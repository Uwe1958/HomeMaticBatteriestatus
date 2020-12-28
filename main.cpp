#include <QCoreApplication>
#include "downloader.h"
#include <QTranslator>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QString locale = QLocale::system().name();
    QTranslator translator;
    if (translator.load(QDir::homePath()+"/QtCreatorProjects/HomeMaticBatteriestatus/HomeMaticBatteriestatus_"+locale)){
        a.installTranslator(&translator);
    }
    Downloader w;
    w.fetch();
    QObject::connect(&w,SIGNAL(finished()),&a,SLOT(quit()));

    return a.exec();
}

#include <QApplication>

#include "HttpRequestManager.h"
#include "Streamer.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//        Streamer streamer("emma_lu1");
//        streamer.checkAvailability();

//        Streamer streamer("scarletloveu");
//        streamer.checkAvailability();

//        Streamer streamer1("misshowl");
//        streamer1.checkAvailability();

//    Streamer streamer1("hannahjames710");
//    streamer1.checkAvailability();

//    Streamer streamer1("aalliyahh");
//    streamer1.checkAvailability();

    MainWindow mv;
    mv.show();

   // QProcess::startDetached("shutdown -s -f -t 60"); //t sono i secondi

    return a.exec();
}

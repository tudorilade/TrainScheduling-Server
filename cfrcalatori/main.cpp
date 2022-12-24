#include <iostream>
#include <QXmlStreamReader>
#include <QtXml>
#include <QTextStream>
#include <string.h>
#include "main.h"
#include "XmlController/xmlcontrollerr.h"
#include "CommandControl/CommandControl/commands.h"
using namespace std;

#include <QApplication>


/*
        cout << "wtf???" << endl;
        for (const auto dir : QDir("/home/tudor/qtcfrcalatori").entryList())
        {
            cout << dir.toStdString() << endl;
        }
*/
int main(int argc, char *argv[])
{
    XmlController xmlFile;
    vector<TrainData>arrivalsInfo;
    GetRequests* getArrivals = new GetArrivals("ARRIVALS -station Tg. Jiu -fromHour 06:30 -toHour 11:30", 1);
    GetRequests* getDepartures = new GetDepartures("DEPARTURES -station sadasdas Nord -fromHour 12:40", 1);

    xmlFile.getArrivalsInfo(arrivalsInfo, getArrivals);

    MainManager app;
    cout << "pornim serverul" << endl;
   // app.start();
    return 0;
}

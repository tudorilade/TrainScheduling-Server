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
    GetRequests* getArrivals = new GetArrivals("ARRIVALS -stationP Târgu Jiu -stationD Turceni -toHour 12:50 -fromHour 11:13", 1);
    GetRequests* getDepartures = new GetDepartures("DEPARTURES -station sadasdas Nord -fromHour 12:40", 1);
    if(getArrivals->isCommandCorrect())
    {
        xmlFile.getArrivalsInfo(arrivalsInfo, getArrivals);
    }
    else
    {
        cout << "la pere cu comanda" << endl;
    }

    MainManager app;
    cout << "pornim serverul" << endl;
   // app.start();
    return 0;
}

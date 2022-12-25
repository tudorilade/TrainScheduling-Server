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
    vector<TrainData>arrivalsInfo, departuresInfo;
    Command* getArrivals = new GetArrivals("ARRIVALS -stationP Amaradia -stationD Turceni -toHour 12:29 -fromHour 11:13", 1);
    Command* getDepartures = new GetDepartures("DEPARTURES -stationP Amaradia -stationD Turceni -toHour 12:30 -fromHour 11:13", 1);
    if(getArrivals->isCommandValid())
    {
        xmlFile.getTrainsInfo(arrivalsInfo, getArrivals);
    }
    else
    {
        cout << "la pere cu comanda" << endl;
    }

    if(getDepartures->isCommandValid())
    {
        xmlFile.getTrainsInfo(departuresInfo, getDepartures);
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

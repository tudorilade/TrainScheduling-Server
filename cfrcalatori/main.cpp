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
    Command* getArrivals = new GetArrivals("GET ARRIVALS -station Bucuresti Nord -fromHour 10:23 -toHour 12:32", 1);

    xmlFile.getArrivalsInfo(arrivalsInfo, getArrivals);

    MainManager app;
    cout << "pornim serverul" << endl;
   // app.start();
    return 0;
}

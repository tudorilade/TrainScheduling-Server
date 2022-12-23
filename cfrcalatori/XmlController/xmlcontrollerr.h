#ifndef XMLCONTROLLER_H
#define XMLCONTROLLER_H

#include <iostream>
#include <QtXml>
#include "../CommandControl/CommandControl/command_ABC.h"

#define XMLPath "/home/tudor/proiecte/repocfr/cfrcalatori/XmlController/data/sample_data.xml" // it works by providing another absolute path too. this is standard

using namespace std;

class TrainData
{
/*
    Struct containing needed info about a train
*/
private:
    char* numeTren;
    char* statieCurenta;
    char* statieDestinatie;
    unsigned int intarziere;
    unsigned int timpStationare;
    unsigned int timpSosire;
    unsigned int timpPlecare;

public:
    string toString();
    string convertFromSeconds();
};

struct NewTrainNode
{
    /* Inserting New Train - to be implemented if enough time*/
};

class XmlController
{
/*
 * Xml Controller class

Class responsible by handling the xmlFile with I/O operations.
*/

private:
    char* xmlPath;
    size_t pathSize = 0;
    bool opened = false;
    QDomDocument document;
    QDomElement trenuriNode;

public:
    XmlController();
    XmlController(char*);
    ~XmlController();
    void getArrivalsInfo(vector<TrainData>&arrivalsInfo, Command* = nullptr);
    void getDeparturesInfo(vector<TrainData>&arrivalsInfo, Command* = nullptr);
    void updateTrainInfo(Command* = nullptr);
    void createTrainInfo(Command* = nullptr);
};

#endif

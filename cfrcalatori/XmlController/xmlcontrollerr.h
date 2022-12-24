#ifndef XMLCONTROLLER_H
#define XMLCONTROLLER_H

#include <iostream>
#include "../CommandControl/CommandControl/commands.h"

#define XMLPath "/home/tudor/proiecte/repocfr/cfrcalatori/XmlController/data/sample_data.xml" // it works by providing another absolute path too. this is standard

using namespace std;

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
    void getArrivalsInfo(vector<TrainData>&arrivalsInfo, GetRequests* = nullptr);
    void getDeparturesInfo(vector<TrainData>&arrivalsInfo, GetRequests* = nullptr);
    void updateTrainInfo(Command* = nullptr);
    void createTrainInfo(Command* = nullptr);
};

#endif

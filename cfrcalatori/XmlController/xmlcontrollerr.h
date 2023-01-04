#ifndef XMLCONTROLLER_H
#define XMLCONTROLLER_H

#include <iostream>
#include <QtXml>
#define XMLPath "/home/tudor/proiecte/repocfr/cfrcalatori/XmlController/data/sample_data.xml" // it works by providing another absolute path too. this is standard

using namespace std;

class XmlController
{
/*
 * Xml Controller class

Class responsible by handling the xmlFile with I/O operations.
*/

private:
    string xmlPath;
    size_t pathSize = 0;
    bool opened = false;
    QDomDocument document;
    QDomElement trenuriNode;

public:
    XmlController();
    XmlController(char*);
    QDomElement getTrenuriNode();
    QDomDocument& getDocument();
    bool writeDocumentOnDisk(QDomDocument);
};

#endif

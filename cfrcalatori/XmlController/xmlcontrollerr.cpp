#include <iostream>
#include "xmlcontrollerr.h"

using namespace std;


XmlController::XmlController()
{
    this->pathSize = strlen(XMLPath);
    this->xmlPath.assign(XMLPath, this->pathSize);
    QFile xmlFile(this->xmlPath.c_str());
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        perror("ERROR WHILE OPENING: ");
        return;
    }

    document.setContent(&xmlFile);
    xmlFile.close();
    trenuriNode = document.documentElement().firstChild().firstChild().firstChild().firstChild().toElement();
    this->opened = true;
}

XmlController::XmlController(char* path)
{

    this->pathSize = strlen(XMLPath);
    this->xmlPath.assign(XMLPath, this->pathSize);
    QFile xmlFile(this->xmlPath.c_str());
    if (!xmlFile.open(QIODevice::ReadOnly))
    {
        perror("ERROR WHILE OPENING: ");
        return;
    }

    document.setContent(&xmlFile);
    xmlFile.close();
    trenuriNode = document.documentElement().firstChild().firstChild().firstChild().firstChild().toElement();
    this->opened = true;
}

QDomElement XmlController::getTrenuriNode()
{
    return this->trenuriNode;
}

QDomDocument& XmlController::getDocument()
{
    return this->document;
}

bool XmlController::writeDocumentOnDisk(QDomDocument doc)
{
    QFile xmlFile(this->xmlPath.c_str());
    if(!xmlFile.open(QIODevice::WriteOnly))
    {
        perror("ERROR WHILE OPENING: ");
        return false;
    }
    QTextStream textStream(&xmlFile);
    textStream << doc.toString();
    xmlFile.flush();
    xmlFile.close();
    return true;
}

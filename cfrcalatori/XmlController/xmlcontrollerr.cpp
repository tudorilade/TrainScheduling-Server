#include <iostream>
#include "xmlcontrollerr.h"

using namespace std;


XmlController::XmlController()
{
    this->pathSize = strlen(XMLPath);
    this->xmlPath = (char*)malloc(this->pathSize);
    strcpy(this->xmlPath, XMLPath);
    QFile xmlFile(this->xmlPath);
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

    this->pathSize = strlen(path);
    this->xmlPath = (char*)malloc(this->pathSize);
    strncpy(this->xmlPath, path, this->pathSize);
    QFile xmlFile(this->xmlPath);
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

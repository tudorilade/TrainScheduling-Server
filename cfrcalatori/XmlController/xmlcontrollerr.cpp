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
    strcpy(this->xmlPath, path);
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

XmlController::~XmlController()
{
    free(xmlPath);
    this->pathSize = 0;
    this->document.clear();
    this->trenuriNode.clear();
    this->opened = false;
}

void XmlController::getArrivalsInfo(vector<TrainData>&arrivalsInfo, GetRequests* command)
{
    QDomElement startTrenuriNode = trenuriNode;
    QDomElement trase, elementTrasa;
    while(!startTrenuriNode.isNull())
    {
        trase = startTrenuriNode.firstChild().toElement();
        while(!(trase.isNull()))
        {
            if("Trase" == trase.tagName().toStdString())
            {
                elementTrasa = startTrenuriNode.firstChild().firstChild().firstChild().toElement();
                while(!elementTrasa.isNull())
                {
                   if(command->isElementValid(elementTrasa))
                   {
                       TrainData infoTrain = command->toTrainData(elementTrasa);
                       if(infoTrain.isValid())
                       {
                        arrivalsInfo.emplace_back(command->toTrainData(elementTrasa));
                       }
                       break;
                   }
                   elementTrasa = elementTrasa.nextSibling().toElement();
                }
            }
            trase = trase.nextSibling().toElement();
        }
        startTrenuriNode = startTrenuriNode.nextSibling().toElement();
    }
}


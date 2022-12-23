#include <iostream>
#include <QXmlStreamReader>
#include <QtXml>
#include <QTextStream>
#include <string.h>
#include "main.h"
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
        QDomDocument studentsXML;
        cout << QFile::exists("/home/tudor/proiecte/repocfr/cfrcalatori/data/sample_data.xml") << endl;

        QFile xmlFile("/home/tudor/proiecte/repocfr/cfrcalatori/data/sample_data.xml");
        if (!xmlFile.open(QIODevice::ReadOnly))
        {
            perror("what is the error???   ");
            return 0;
        }
        studentsXML.setContent(&xmlFile);
        xmlFile.close();

        QDomElement root = studentsXML.documentElement();
        QDomElement trenuriNode = root.firstChild().firstChild().firstChild().firstChild().toElement();
        QDomElement trase, elementTrasa;
        while(!trenuriNode.isNull())
        {
            cout << "Next " << trenuriNode.tagName().toStdString() << " ";
            cout << trenuriNode.attribute("CategorieTren").toStdString() << trenuriNode.attribute("Numar").toStdString() << endl;
            trase = trenuriNode.firstChild().toElement();
            while(!(trase.isNull()))
            {
                if("Trase" == trase.tagName().toStdString())
                {
                    elementTrasa = trenuriNode.firstChild().firstChild().firstChild().toElement();
                    while(!elementTrasa.isNull())
                    {
                       cout << elementTrasa.attribute("DenStaOrigine").toStdString() << endl;
                       elementTrasa = elementTrasa.nextSibling().toElement();

                    }

                }
                trase = trase.nextSibling().toElement();
            }
            trenuriNode = trenuriNode.nextSibling().toElement();
        }

    MainManager app;
    cout << "pornim serverul" << endl;
   // app.start();
    return 0;
}

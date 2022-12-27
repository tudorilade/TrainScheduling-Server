//
// Created by tudor on 02.12.2022.
//
#include <iostream>
#include <cstdlib>
#include "commands.h"


using namespace std;


// Declaration of some utils functions

int checkIfCorrectHourArgument(char* arg)
{
    /* check if supplied hour argument is correct

     returns -1 if it is incorrect
     returns index of ':' if correct
*/
    int k = 0, j = 0;
    while(arg[k] != ':'){if(arg[k] == '\0'){break;};++k;}
    if(k == strlen(arg))
        return -1;

    for(j = 0; j < k; j++)
    {
        if(!isdigit((int)arg[j]))
        {
            return -1;
        }
    }

    for(j = k + 1; arg[j] != '\0'; j++)
    {
        if(!isdigit((int)arg[j]))
        {
            return -1;
        }
    }
    if(strlen(arg) > 5)
    {
        return -1; // correct format is HH:MM, which has length of 5.
    }

    return k;
}

int checkIfCommandExists(const char* command, int& i, int& j)
{
    /* Function responsonsible to determine whether the command exists

    It looks if supplied command exists (e.g DEPARTURES, ARRIVALS, UPDATE).
    Index i corresponds to the begining of command. Index j corresponds to the end of it. (e.g i points to char 'A' and j points to 'S' from ARRIVALS command)

    return value:
        0 -> command correspond and i and j are correctly populated
        1 -> command is invalid (i.e It was supplied only command without arguments)
*/

    while(command[i] == ' '){i++;};
    j = i;
    while(command[j] != '-')
    {
        if(command[j] == '\0')
        {
            // case when input is just "ARRIVALS", or any string
            return 1;
        }
        j++;
    }
    return 0;
}

void removeBlankSpaces(const char* command, int& i, bool &end)
{
    /* Removes blank spaces between commands

    If a command like: "arrivals -stationP name -stationD" is provided, end will become true.
    This results in an incorrect command
    */
    while(command[i] == ' ')
    {
        // remove blank spaces
        i++;
        if(command[i] == '\0')
        {
            end = true;
        }
    }
}

void checkArgument(const char* command, int& i, int& j, int& k)
{
    /* returns the indexes in between argument is found

     e.g: k will be the index of end of argument ("d" in case of Bucuresti Nord). j is the length of the argument
*/
    while(true)
    {
        /* Case when station is formed of multiple words (e.g. Bucuresti Nord) */
        if(command[i] != '-' && command[i] != '\0')
        {
            i++; j++;
        }
        else if(command[i] == '\0' || command[i] == '-')
        {
            j--;
            i--;
            k = i;
            while(command[k]==' ')
            {
                k--;
                j--;
            };
            break;
        }
    }
}
/* GET Commands - interprets the parsed command */
GetRequests::GetRequests(const char* command, int sd) : Command(command, sd)
{
    int i = 0, j = 0, k = 0;

    if(checkIfCommandExists(command, i, j) != 0)
    {
        this->incorectCommand = true;
    }

    this->sizeCommand = (size_t)(j - i - 1);
    this->getCommand.assign(command, i, this->sizeCommand);

    bool end = false;
    char* flag;
    char* arg;

    for(i = j; command[i] != '\0'; i++)
    {
        j = 0; k = 0;
        removeBlankSpaces(command, i, end);
        if(end){break;}

        for(j = i; command[j] != ' '; j++); // get the length of the flag
        flag = (char*)malloc(j-i+1); // allocate memory of it
        strncpy(flag, command + i, j-i+1); // check the flag
        flag[j-i] = '\0';

        i = j; j = 0;
        checkArgument(command, i, j, k);

        if(j == -1)
        {
            // Nu s-a parsat un argument
            this->incorectCommand = true;
            free(flag);
            break;
        }
        arg = (char*)malloc(j+1);
        strncpy(arg, command + k - j + 1, j);
        arg[j] = '\0';

        if(strcmp(flag, "-stationPS") == 0)
        {
            this->sizeStation = (size_t)j;
            this->targetStation.assign(arg, this->sizeStation);
            this->stationPFlag = true;
        }
        else if(strcmp(flag, "-stationD") == 0)
        {
            this->sizeStationD = (size_t)j;
            this->stationD.assign(arg, this->sizeStationD);
            this->stationDFlag = true;
        }
        else if(strcmp(flag, "-toHour") == 0 || strcmp(flag, "-fromHour") == 0)
        {
            if((k = checkIfCorrectHourArgument(arg)) == -1)
            {
                this->incorectHourArguments = true;
                free(flag); free(arg);
                break;
            }
            char* hour = (char*)malloc(4);
            char* min = (char*)malloc(4);
            strncpy(hour, arg, k);
            hour[k] = '\0';
            strncpy(min, arg + k + 1, strlen(arg) - k);
            hour[strlen(arg) - k] = '\0';
            if(strcmp(flag, "-toHour") == 0)
            {
                this->toHour = atoi(hour) * 3600 + atoi(min) * 60;
                this->toHourFlag = true;
            }
            else if(strcmp(flag, "-fromHour") == 0)
            {
                this->fromHour = atoi(hour) * 3600 + atoi(min) * 60;
                this->fromHourFlag = true;
            }
            free(hour); free(min);
        }
        else
        {
            this->incorectCommand = true;
            free(arg); free(flag);
            break;
        }
        free(flag); free(arg);
    }

    if(this->fromHourFlag && !this->toHourFlag)
        this->toHour = (this->fromHour + 3600) % 86400;


    if(this->fromHour > 86340 || this->toHour > 86340)
        this->incorectHourArguments = true;
}


GetRequests::~GetRequests()
{
    this->fromHour = 0;
    this->toHour = 0;
    this->targetStation = ""; this->sizeStation =  0;
}

bool GetRequests::isCommandValid()
{
    return !this->incorectCommand;
}

bool GetRequests::hasFomHourFlag()
{
    return this->fromHourFlag;
}

bool GetRequests::hasToHourFlag()
{
    return this->toHourFlag;
}

bool GetRequests::hasIncorectArugments()
{
    return this->incorectHourArguments;
}

string GetRequests::getStationName()
{
    return this->targetStation;
}

unsigned int GetRequests::getFromHour()
{
    return this->fromHour;
}

unsigned int GetRequests::getToHour()
{
    return this->toHour;
}

size_t GetRequests::getSizeCommand()
{
    return this->sizeCommand;
}

size_t GetRequests::getSizeStationName()
{
    return this->sizeStation;
}

void GetRequests::getTrainsInfo(vector<TrainData> &trainInfo, XmlController &xmlFile)
{
    QDomElement startTrenuriNode = xmlFile.getTrenuriNode();
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
                   if(this->isElementValid(elementTrasa))
                   {
                       TrainData infoTrain = this->toTrainData(elementTrasa);
                       if(infoTrain.isValid())
                       {
                        trainInfo.emplace_back(infoTrain);
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


/* GET ARRIVALS Command */
GetArrivals::GetArrivals(const char* command, int sd) : GetRequests(command, sd) {
    if(!((this->getCommand.compare("ARRIVALS") != 0) || (this->getCommand.compare("DEPARTURES") != 0)))
        this->incorectCommand = true;
    else if(this->incorectHourArguments)
        this->incorectCommand = true;
    else if(!this->stationPFlag)
        this->incorectCommand = true;
};

GetArrivals::~GetArrivals() = default;

struct CommandResult GetArrivals::execute(XmlController &xmlFile){

    vector<TrainData>arrivalsInfo;
    if(this->isCommandValid())
    {
        this->getTrainsInfo(arrivalsInfo, xmlFile);
        this->resultCommand = toTableArrivals(arrivalsInfo);
    }
    else
    {
        this->resultCommand.result = "Comanda invalida! Va rugam sa incercati din nou!\n";
        this->resultCommand.size_result = resultCommand.result.size();
    }

    return resultCommand;
}

string  GetArrivals::get_command() {
    return this->command_t;
}

bool GetArrivals::isElementValid(QDomElement &elementTrasa)
{
    /*
     * returns true if trasa element corresponds to command arguments
     * flase otherwise
    */
    if(!(elementTrasa.attribute("DenStaOrigine").toStdString() == this->targetStation))
        return false;

    unsigned int OraP = elementTrasa.attribute("OraP").toInt();
    unsigned int timpStationare = elementTrasa.attribute("StationareSecunde").toInt();
    unsigned int oraSosireStatie = OraP - timpStationare;

    if(elementTrasa.previousSiblingElement().toElement().isNull())
    {
        // at Departure case there is no train to arrive
        return false;
    }
    else if(this->fromHour <= this->toHour)
    {
        if(!(oraSosireStatie >= this->fromHour && oraSosireStatie <= this->toHour))
        {
            return false;
        }
    }
    else if(oraSosireStatie > this->toHour && oraSosireStatie < this->fromHour)
    {
        return false;
    }

    return true;
}


TrainData GetArrivals::toTrainData(QDomElement &elementTrasa)
{
    /* converts a DomElement to TrainData class given the details within Command */
    TrainData infoAboutATrain;
    QDomElement trenNode = elementTrasa.parentNode().parentNode().parentNode().toElement();
    string trainName, statieP, statieN, statieD;
    unsigned int delayP, delayN, delayD, timpStationareP, timpStationareN, timpStationareD;
    unsigned int timpSosireP, timpSosireD, timpSosireN, timpPlecareP, timpPlecareD, timpPlecareN;

    // Train NamegetTrainsInfo
    trainName = trenNode.attribute("CategorieTren").toStdString() + trenNode.attribute("Numar").toStdString();

    //arrival station
    statieN = elementTrasa.attribute("DenStaOrigine").toStdString();
    delayN = elementTrasa.attribute("Ajustari").toInt();
    timpStationareN = elementTrasa.attribute("StationareSecunde").toInt();
    timpSosireN = elementTrasa.attribute("OraP").toInt() - timpStationareN;
    timpPlecareN= elementTrasa.attribute("OraP").toInt();

    // starting station
    QDomElement startingStation = elementTrasa;
    while(!startingStation.previousSibling().toElement().isNull() && (startingStation.previousSibling().toElement().tagName() == "ElementTrasa"))
    {
        startingStation = startingStation.previousSibling().toElement();
    }
    statieP = startingStation.attribute("DenStaOrigine").toStdString();
    delayP = startingStation.attribute("Ajustari").toInt();
    timpStationareP = startingStation.attribute("StationareSecunde").toInt();
    timpSosireP = startingStation.attribute("OraP").toInt() - timpStationareP;
    timpPlecareP = startingStation.attribute("OraP").toInt();

    // final station
    while(!elementTrasa.nextSibling().toElement().isNull() && (elementTrasa.nextSibling().toElement().tagName() == "ElementTrasa"))
    {
        if(this->stationDFlag)
        {
            // If stationD is provided, it stops there. Otherwise, displays final destination of the route
            if(elementTrasa.attribute("DenStaOrigine").toStdString() == this->stationD)
            {
                break;
            }
        }
        elementTrasa = elementTrasa.nextSibling().toElement();
    }

    statieD = elementTrasa.attribute("DenStaOrigine").toStdString();
    delayD = elementTrasa.attribute("Ajustari").toInt();
    timpStationareD = elementTrasa.attribute("StationareSecunde").toInt();
    timpSosireD = elementTrasa.attribute("OraP").toInt() - timpStationareD;
    timpPlecareD = elementTrasa.attribute("OraP").toInt();

    if(this->stationDFlag)
    {
        if(statieD != this->stationD)
        {
            return TrainData{false}; // means even though an arrival station exists, it is not going to stationD provided
        }
    }

    return TrainData{
    trainName, statieP, statieN, statieD, delayP, delayD, delayN, timpStationareP,
    timpStationareD, timpStationareN, timpSosireP, timpSosireD, timpSosireN, timpPlecareP, timpPlecareD,
           timpPlecareN
    };
}



/* GET DEPARTURES */
GetDepartures::GetDepartures(const char* command, int sd) : GetRequests(command, sd)
{
    if(!((this->getCommand.compare("ARRIVALS") != 0) || (this->getCommand.compare( "DEPARTURES") != 0)))
        this->incorectCommand = true;
    else if(this->incorectHourArguments)
        this->incorectCommand = true;
    else if(!this->stationPFlag)
        this->incorectCommand = true;
}

struct CommandResult GetDepartures::execute(XmlController &xmlFile){
    vector<TrainData>departuresInfo;
    struct CommandResult resultCommand;

    if(this->isCommandValid())
    {
        this->getTrainsInfo(departuresInfo, xmlFile);
        resultCommand = toTableDepartures(departuresInfo);

    }
    else
    {
        resultCommand.result = "Comanda invalida! Va rugam sa incercati din nou!\n";
        resultCommand.size_result = resultCommand.result.size();
    }

    return resultCommand;
}

string  GetDepartures::get_command() {
    return this->command_t;
}

bool GetDepartures::isElementValid(QDomElement &elementTrasa)
{
    /*
     * returns true if trasa element corresponds to command arguments
     * flase otherwise
    */
    if(!(elementTrasa.attribute("DenStaOrigine").toStdString() == this->targetStation))
        return false;

    unsigned int OraP = elementTrasa.attribute("OraP").toInt();

    if(elementTrasa.attribute("DenStaOrigine").toStdString() == elementTrasa.attribute("DenStaDestinatie").toStdString())
    {
        // final station. no departures from there
        return false;
    }
    else if(this->fromHour <= this->toHour)
    {
        if(!(OraP >= this->fromHour && OraP <= this->toHour))
        {
            return false;
        }
    }
    else if(OraP > this->toHour && OraP < this->fromHour)
    {
        return false;
    }

    return true;
}

TrainData GetDepartures::toTrainData(QDomElement &elementTrasa)
{
    /* converts a DomElement to TrainData class given the details within Command */
    TrainData infoAboutATrain;
    QDomElement trenNode = elementTrasa.parentNode().parentNode().parentNode().toElement();
    string trainName, statieP, statieN, statieD;
    unsigned int delayP, delayN, delayD, timpStationareP, timpStationareN, timpStationareD;
    unsigned int timpSosireP, timpSosireD, timpSosireN, timpPlecareP, timpPlecareD, timpPlecareN;

    // Train Name
    trainName = trenNode.attribute("CategorieTren").toStdString() + trenNode.attribute("Numar").toStdString();

    //current station
    statieP = elementTrasa.attribute("DenStaOrigine").toStdString();
    delayP = elementTrasa.attribute("Ajustari").toInt();
    timpStationareP = elementTrasa.attribute("StationareSecunde").toInt();
    timpSosireP = elementTrasa.attribute("OraP").toInt() - timpStationareP;
    timpPlecareP= elementTrasa.attribute("OraP").toInt();


    if(!elementTrasa.nextSibling().toElement().isNull() && (elementTrasa.nextSibling().toElement().tagName() == "ElementTrasa"))
    {
        // next Station
        elementTrasa = elementTrasa.nextSibling().toElement();
        statieN = elementTrasa.attribute("DenStaOrigine").toStdString();
        delayN = elementTrasa.attribute("Ajustari").toInt();
        timpStationareN = elementTrasa.attribute("StationareSecunde").toInt();
        timpSosireN = elementTrasa.attribute("OraP").toInt() - timpStationareN;
        timpPlecareN = elementTrasa.attribute("OraP").toInt();
    }

    if(this->stationDFlag && this->stationD == elementTrasa.attribute("DenStaOrigine").toStdString())
    {
        // next Station and Destination Station are the same
        statieD = elementTrasa.attribute("DenStaOrigine").toStdString();
        delayD = elementTrasa.attribute("Ajustari").toInt();
        timpStationareD = elementTrasa.attribute("StationareSecunde").toInt();
        timpSosireD = elementTrasa.attribute("OraP").toInt() - timpStationareD;
        timpPlecareD = elementTrasa.attribute("OraP").toInt();
    }
    else
    {
        while(!elementTrasa.nextSibling().toElement().isNull() && (elementTrasa.nextSibling().toElement().tagName() == "ElementTrasa"))
        {
            if(this->stationDFlag)
            {
                // If stationD is provided, it stops there. Otherwise, displays final destination of the route
                if(elementTrasa.attribute("DenStaOrigine").toStdString() == this->stationD)
                {
                    break;
                }
            }
            elementTrasa = elementTrasa.nextSibling().toElement();
        }
        statieD = elementTrasa.attribute("DenStaOrigine").toStdString();
        delayD = elementTrasa.attribute("Ajustari").toInt();
        timpStationareD = elementTrasa.attribute("StationareSecunde").toInt();
        timpSosireD = elementTrasa.attribute("OraP").toInt() - timpStationareD;
        timpPlecareD = elementTrasa.attribute("OraP").toInt();
        if(this->stationDFlag)
        {
            if(statieD != this->stationD)
            {
                return TrainData{false}; // means is invalid
            }
        }
    }


    return TrainData{
    trainName, statieP, statieN, statieD, delayP, delayD, delayN, timpStationareP,
    timpStationareD, timpStationareN, timpSosireP, timpSosireD, timpSosireN, timpPlecareP, timpPlecareD,
           timpPlecareN
    };
}


/* UPDATE Command*/

UpdateTrain::UpdateTrain(const char* command, int sd, pthread_mutex_t& mutex) : Command(command, sd)
{
    int i = 0, j = 0, k = 0;

    if(checkIfCommandExists(command, i, j) != 0)
    {
        this->incorectCommand = true;
    }

    this->sizeCommand = (size_t)(j - i - 1);
    this->updateCommand.assign(command, i, this->sizeCommand);

    if(updateCommand != "UPDATE")
    {
        this->incorectCommand = true;
    }

    bool end = false;
    char* flag = 0;
    char* arg = 0;

    for(i = j; command[i] != '\0'; i++)
    {
        j = 0; k = 0;
        removeBlankSpaces(command, i, end);
        if(end){break;}

        for(j = i; command[j] != ' '; j++); // get the length of the flag
        flag = (char*)malloc(j-i+1); // allocate memory of it
        strncpy(flag, command + i, j-i+1); // check the flag
        flag[j-i] = '\0';

        i = j; j = 0;
        checkArgument(command, i, j, k);

        if(j == -1)
        {
            // Nu s-a parsat un argument
            this->incorectCommand = true;
            free(flag);
            break;
        }
        arg = (char*)malloc(j+1);
        strncpy(arg, command + k - j + 1, j);
        arg[j] = '\0';

        if(strcmp(flag, "-fromStation") == 0)
        {
            this->sizeStation = (size_t)j;
            this->targetStation.assign(arg, this->sizeStation);
            this->targetSFlag = true;
        }
        else if(strcmp(flag, "-delay") == 0)
        {
            this->delay = atoi(arg) * 60; // convert minutes in seconds
            this->delayFlag = true;
        }
        else if(strcmp(flag, "-train") == 0)
        {
            this->sizeTrainID = (size_t)j;
            this->trainID.assign(arg, this->sizeTrainID);
            this->trainIDFlag = true;
        }
        else if(strcmp(flag, "-toStation") == 0)
        {
            this->stopStation.assign(arg, (size_t)j);
            this->stopStationFlag = true;
        }
        else
        {
            this->incorectCommand = true;
            free(flag); free(arg);
            break;
        }
    }

    if(this->delayFlag)
    {
        if(this->delay > 24 * 3600) // delay over 24 hours is not considered
            this->incorectCommand = true;
    }

    if(!this->trainIDFlag || !this->delayFlag)
    {
        this->incorectCommand = true;
    }

    this->writeMutex = mutex;

}

struct CommandResult UpdateTrain::execute(XmlController &xmlFile){
    if(this->incorectCommand)
    {
        this->resultCommand.result = "Comanda este invalida. Va rugam sa transmiteti o comanda cu toate argumentele valide.";
        this->resultCommand.size_result = this->resultCommand.result.size();
        return this->resultCommand;
    }
    QDomDocument document = xmlFile.getDocument();
    QDomElement root = document.documentElement(), tren;
    QDomNodeList trenuri = root.firstChild().firstChild().firstChild().childNodes();
    bool trainFound = false;
    string tag = root.firstChild().firstChild().firstChild().toElement().tagName().toStdString();
    int nodeCount = trenuri.count();
    int foundNdx = 0;
    for(int i = 0; i < nodeCount; i++)
    {
        QDomElement tren = trenuri.at(i).toElement();
        if(this->isElementValid(tren))
        {
            this->updateElement(tren);
            trainFound = true;
            foundNdx = i;
            break;
        }

    }

    if(this->incorectCommand)
    {
        this->resultCommand.result = "Comanda este invalida. Va rugam sa transmiteti o comanda cu toate argumentele valide.";
        this->resultCommand.size_result = this->resultCommand.result.size();
        return this->resultCommand;
    }

    if(trainFound)
    {
        document.firstChild().firstChild().firstChild().replaceChild(tren, trenuri.at(foundNdx));
        pthread_mutex_lock(&this->writeMutex);
        xmlFile.writeDocumentOnDisk(document);
        pthread_mutex_unlock(&this->writeMutex);
        trainFound = true;
        this->resultCommand.result = "Trenul " + this->trainID + " a fost actualizat cu o intarziere de " + to_string(this->delay / 60) + " minute.\n";
        this->resultCommand.size_result = this->resultCommand.result.size();
    }
    else
    {
        this->resultCommand.result = "Trenul " + this->trainID + " nu a fost gasit.\n";
        this->resultCommand.size_result = this->resultCommand.result.size();
    }
    return this->resultCommand;
}

void UpdateTrain::updateElement(QDomElement& tren)
{
    QDomElement trasa = tren.firstChild().firstChild().toElement();
    QDomNodeList elementTrasaList = trasa.childNodes();
    int countElTrasa = elementTrasaList.count();
    string tra = trasa.tagName().toStdString();

    int i = 0, stopIndex = countElTrasa;

    if(this->targetSFlag)
    {
        for(; i < countElTrasa; i++)
        {
            if(elementTrasaList.at(i).toElement().attribute("DenStaOrigine").toStdString() == this->targetStation)
            {
                break;
            }
        }
    }

    if(this->stopStationFlag)
    {
        for(; stopIndex >= 0; stopIndex--)
        {
            if(elementTrasaList.at(stopIndex).toElement().attribute("DenStaOrigine").toStdString() == this->stopStation)
            {
                break;
            }
        }
    }

    if(i > stopIndex)
    {
        this->incorectCommand = true;
    }

    for(; i <= stopIndex; i++)
    {
        if(i == countElTrasa)
            break;

        QDomElement elTrasa = elementTrasaList.at(i).toElement();
        if(elTrasa.toElement().tagName().toStdString() != "ElementTrasa")
            continue;

        elTrasa.setAttribute("Ajustari", this->delay);
        string atr = elTrasa.attribute("Ajustari").toStdString();
        string st = elTrasa.attribute("DenStaOrigine").toStdString();
        tren.firstChild().firstChild().toDocument().replaceChild(elTrasa, elementTrasaList.at(i));
    }
}

string  UpdateTrain::get_command() {
    return this->command_t;
}

bool UpdateTrain::isCommandValid()
{
    return !this->incorectCommand;
}

bool UpdateTrain::isElementValid(QDomElement &element){
    if(!(element.tagName() == "Tren"))
        return false;

    string trainID;
    trainID = element.attribute("CategorieTren").toStdString() + element.attribute("Numar").toStdString();

    if(trainID != this->trainID)
        return false;

    return true;
}; // dummy override

TrainData UpdateTrain::toTrainData(QDomElement&){return TrainData{false};}; // dummy override

/* EXIT COMMAND */
struct CommandResult ExitCommand::execute(XmlController &xmlFile){
    cout << "Se execute comanda: Exit connection" << endl;
    this->resultCommand.result.assign("EXIT");
    this->resultCommand.size_result = this->resultCommand.result.size();
    return this->resultCommand;
}

string  ExitCommand::get_command() {
    return this->command_t;
}

bool ExitCommand::isCommandValid()
{
    return !this->incorectCommand;
}

TrainData ExitCommand::toTrainData(QDomElement&){return TrainData{false};}; // dummy override
bool ExitCommand::isElementValid(QDomElement&){return false;}; // dummy override


/* UNRECOGNIZED COMMAND */
string  UnRecognizedCommand::get_command() {
    return this->command_t;
}

struct CommandResult UnRecognizedCommand::execute(XmlController &xmlFile) {
    struct CommandResult res {};
    res.result.assign("Comanda invalida! Va rugam incercati din nou!\n");
    res.size_result = res.result.size();
    return res;
}

bool UnRecognizedCommand::isCommandValid()
{
    return !this->incorectCommand;
}

TrainData UnRecognizedCommand::toTrainData(QDomElement&){return TrainData{false};}; // dummy override
bool UnRecognizedCommand::isElementValid(QDomElement&){return false;}; // dummy override


// Train Data
TrainData::TrainData(
        string numeTren, string statieP, string statieN, string statieD,
        unsigned int intarziereP, unsigned int intarziereD, unsigned int intarziereN,
        unsigned int timpStationareP, unsigned int timpStationareD, unsigned int timpStationareN,
        unsigned int timpSosireP, unsigned int timpSosireD, unsigned int timpSosireN,
        unsigned int timpPlecareP, unsigned int timpPlecareD, unsigned int timpPlecareN
)
{

    this->numeTren = numeTren;
    this->statieP = statieP;
    this->statieN = statieN;
    this->statieD = statieD;
    this->intarziereP = intarziereP;
    this->intarziereD = intarziereD;
    this->intarziereN = intarziereN;
    this->timpStationareP = timpStationareP;
    this->timpStationareN = timpStationareN;
    this->timpSosireP = timpSosireP;
    this->timpSosireD = timpSosireD;
    this->timpSosireN = timpSosireN;
    this->timpPlecareP = timpPlecareP;
    this->timpPlecareN = timpPlecareN;
}

TrainData::TrainData(bool invalid)
{
    this->valid = invalid;
}

bool TrainData::isValid()
{
    return this->valid;
}

const string TrainData::toDeparturesString()
{
    /* It will represent a row in a table */
    string row;
    row += "| ";
    row += centerContent(this->numeTren, 10) += " | ";
    row += centerContent(this->statieP, 30) += " | ";
    row += centerContent(this->statieD, 30) += "  | ";
    row += centerContent(secondsToHourMinutes(this->timpPlecareP), 15) += " | ";
    row += centerContent(secondsToMinutes(this->intarziereP), 15) += " | ";
    row += centerContent(secondsToHourMinutes(this->timpSosireD), 15) += " | ";
    row += centerContent(secondsToMinutes(this->intarziereD), 15) += " |\n";
   return row;
}

const string TrainData::toArrivalsString()
{
    /* It will represent a row in a table */
    string row;
    row += "| ";
    row += centerContent(this->numeTren, 10) += " | ";
    row += centerContent(this->statieN, 30) += " | ";
    row += centerContent(this->statieD, 30) += "  | ";
    row += centerContent(secondsToHourMinutes(this->timpSosireN), 15) += " | ";
    row += centerContent(secondsToMinutes(this->intarziereN), 15) += " | ";
    row += centerContent(secondsToHourMinutes(this->timpSosireD), 15) += " | ";
    row += centerContent(secondsToMinutes(this->intarziereD), 15) += " |\n";
   return row;
}

string centerContent(const string s, const int width) {
    /* Centers te content of table header */
    stringstream ss, spaces;
    int padding = width - s.size();// count excess room to pad
    for(int i=0; i<padding/2; ++i)
        spaces << " ";

    ss << spaces.str() << s << spaces.str();    // format with padding
    if(padding>0 && padding%2!=0)               // if odd #, add 1 space
        ss << " ";
    return ss.str();
}

string secondsToHourMinutes(const unsigned int seconds)
{
    /* converts seconds to HH:MM format */
    stringstream format;
    format << (int)(seconds / 3600) << ":" << (int)((seconds % 3600) / 60);
    return format.str();
}

string secondsToMinutes(const unsigned int seconds)
{
    /* convers seconds to MM format */
    stringstream format;
    format << (int)(seconds / 60);
    return format.str();
}


struct CommandResult toTableDepartures(vector<TrainData>&trainInfo)
{
    /* table header departures
        Tren |  Statie Plecare | Statie Destinatie | OraPP | IntarziereP | OraSF | IntarziereF
    */
    struct CommandResult resultCommand;
    resultCommand.result += "\n| ";
    resultCommand.result += centerContent("Tren", 10) += " | ";
    resultCommand.result += centerContent("Statie Plecare", 30) += " | ";
    resultCommand.result += centerContent("Statia Destinatie", 30) += " | ";
    resultCommand.result += centerContent("OraP", 15) += " | ";
    resultCommand.result += centerContent("IntarziereP", 15) += " | ";
    resultCommand.result += centerContent("OraSF", 15) += " | ";
    resultCommand.result += centerContent("IntarziereF", 15) += " |\n";

    for(auto& train : trainInfo)
    {
        resultCommand.result += train.toDeparturesString();
    }

    resultCommand.size_result = resultCommand.result.size();
    return resultCommand;
}

struct CommandResult toTableArrivals(vector<TrainData>&trainInfo)
{
    /* table header
        Tren |  Statie Sosiri | Statie Finala | OraSP | IntarziereP | OraSF | IntarziereF
    */
    struct CommandResult resultCommand;
    resultCommand.result += "\n| ";
    resultCommand.result += centerContent("Tren", 10) += " | ";
    resultCommand.result += centerContent("Statie Sosiri", 30) += " | ";
    resultCommand.result += centerContent("Statie Finala", 30) += " | ";
    resultCommand.result += centerContent("OraS", 15) += " | ";
    resultCommand.result += centerContent("IntarziereS", 15) += " | ";
    resultCommand.result += centerContent("OraSF", 15) += " | ";
    resultCommand.result += centerContent("IntarziereF", 15) += " |\n";

    for(auto& train : trainInfo)
    {
        resultCommand.result += train.toArrivalsString();
    }

    resultCommand.size_result = resultCommand.result.size();
    return resultCommand;
}

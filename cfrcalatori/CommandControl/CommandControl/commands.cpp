//
// Created by tudor on 02.12.2022.
//
#include <iostream>
#include <cstdlib>
#include "commands.h"
using namespace std;

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
/* GET Commands - interprets the parsed command */
GetRequests::GetRequests(char* command, int sd) : Command(command, sd)
{
    int i = 0, j = 0, k = 0;
    while(command[i] == ' '){i++;};
    j = i;
    while(command[j] != '-')
    {
        if(command[j] == '\0')
        {
            // case when input is just "ARRIVALS", or any string
            this->incorectCommand = true;
            break;
        }
        j++;
    }

    this->sizeCommand = (size_t)(j - i - 1);
    this->getCommand.assign(command, i, this->sizeCommand);

    bool end = false;
    char* flag;
    char* arg;

    for(i = j; command[i] != '\0'; i++)
    {
        j = 0; k = 0;
        while(command[i] == ' ')
        {
            // remove blank spaces
            i++;
            if(command[i] == '\0')
            {
                end = true;
                break;
            }
        }
        if(end){break;}

        for(j = i; command[j] != ' '; j++); // get the length of the flag

        flag = (char*)malloc(j-i+1); // allocate memory of it

        strncpy(flag, command + i, j-i+1); // check the flag
        flag[j-i] = '\0';

        i = j; j = 0;
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
        if(j == -1)
        {
            // Nu s-a parsat un argument
            this->incorectCommand = true;
            break;
        }
        arg = (char*)malloc(j+1);
        strncpy(arg, command + k - j + 1, j);
        arg[j] = '\0';

        if(strcmp(flag, "-stationP") == 0)
        {
            this->sizeStation = (size_t)j;
            this->targetStation.assign(arg, this->sizeStation);
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

TrainData GetRequests::toTrainData(QDomElement elementTrasa)
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
    timpSosireP = elementTrasa.attribute("OraP").toInt() - timpStationareP + delayP;
    timpPlecareP= elementTrasa.attribute("OraP").toInt();


    if(!elementTrasa.nextSibling().toElement().isNull() && (elementTrasa.nextSibling().toElement().tagName() == "ElementTrasa"))
    {
        // next Station
        elementTrasa = elementTrasa.nextSibling().toElement();
        statieN = elementTrasa.attribute("DenStaOrigine").toStdString();
        delayN = elementTrasa.attribute("Ajustari").toInt();
        timpStationareN = elementTrasa.attribute("StationareSecunde").toInt();
        timpSosireN = elementTrasa.attribute("OraP").toInt() - timpStationareN + delayN;
        timpPlecareN = elementTrasa.attribute("OraP").toInt();
    }

    if(this->stationDFlag && this->stationD == elementTrasa.attribute("DenStaOrigine").toStdString())
    {
        // next Station and Destination Station are the same
        statieD = elementTrasa.attribute("DenStaOrigine").toStdString();
        delayD = elementTrasa.attribute("Ajustari").toInt();
        timpStationareD = elementTrasa.attribute("StationareSecunde").toInt();
        timpSosireD = elementTrasa.attribute("OraP").toInt() - timpStationareD + delayD;
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
        timpSosireD = elementTrasa.attribute("OraP").toInt() - timpStationareD + delayD;
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


/* GET ARRIVALS Command */
GetArrivals::GetArrivals(char* command, int sd) : GetRequests(command, sd) {
    if(!((this->getCommand.compare("ARRIVALS") != 0) || (this->getCommand.compare("DEPARTURES") != 0)))
        this->incorectCommand = true;
    else if(this->incorectHourArguments)
        this->incorectCommand = true;
};

GetArrivals::~GetArrivals() = default;

struct CommandResult GetArrivals::execute_command(){
    cout << "Se execute comanda: Get Arrivals" << endl;
    return test_res;
}

string  GetArrivals::get_command() {
    return this->command_t;
}

bool GetArrivals::isElementValid(QDomElement elementTrasa)
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


    if(this->fromHour <= this->toHour)
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



/* GET DEPARTURES */

GetDepartures::GetDepartures(char* command, int sd) : GetRequests(command, sd)
{
    if(!((this->getCommand.compare("ARRIVALS") != 0) || (this->getCommand.compare( "DEPARTURES") != 0)))
        this->incorectCommand = true;
    else if(this->incorectHourArguments)
        this->incorectCommand = true;
}

struct CommandResult GetDepartures::execute_command(){
    cout << "Se execute comanda: Get Departures Arrivals" << endl;
    return test_res;
}

string  GetDepartures::get_command() {
    return this->command_t;
}

bool GetDepartures::isElementValid(QDomElement elementTrasa)
{
    /*
     * returns true if trasa element corresponds to command arguments
     * flase otherwise
    */
    if(!(elementTrasa.attribute("DenStaOrigine").toStdString() == this->targetStation))
        return false;

    unsigned int OraP = elementTrasa.attribute("OraP").toInt();

    if(this->fromHour <= this->toHour)
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


/* UPDATE Command*/
struct CommandResult UpdateTrain::execute_command(){
    cout << "Se execute comanda: Update train route" << endl;
    return test_res;
}

string  UpdateTrain::get_command() {
    return this->command_t;
}

bool UpdateTrain::isCommandValid()
{
    return !this->incorectCommand;
}

TrainData UpdateTrain::toTrainData(QDomElement){return TrainData{false};}; // dummy override
bool UpdateTrain::isElementValid(QDomElement){return false;}; // dummy override


/* CREATE Command */
struct CommandResult CreateNewRoute::execute_command(){
    cout << "Se execute comanda: Create new Route" << endl;
    return test_res;
}

string  CreateNewRoute::get_command() {
    return this->command_t;
}

bool CreateNewRoute::isCommandValid()
{
    return !this->incorectCommand;
}

TrainData CreateNewRoute::toTrainData(QDomElement){return TrainData{false};}; // dummy override
bool CreateNewRoute::isElementValid(QDomElement){return false;}; // dummy override


struct CommandResult ExitCommand::execute_command(){
    cout << "Se execute comanda: Exit connection" << endl;
    test_res.result.assign("EXIT");
    test_res.size_result = 4;
    test_res.result[4] = '\0';
    return test_res;
}

string  ExitCommand::get_command() {
    return this->command_t;
}

bool ExitCommand::isCommandValid()
{
    return !this->incorectCommand;
}

TrainData ExitCommand::toTrainData(QDomElement){return TrainData{false};}; // dummy override
bool ExitCommand::isElementValid(QDomElement){return false;}; // dummy override


string  UnRecognizedCommand::get_command() {
    return this->command_t;
}

struct CommandResult UnRecognizedCommand::execute_command() {
    struct CommandResult res {};
    res.result = reinterpret_cast<char*>(malloc(1));
    res.result.assign("0");
    res.size_result = 1;
    return res;
}

bool UnRecognizedCommand::isCommandValid()
{
    return !this->incorectCommand;
}

TrainData UnRecognizedCommand::toTrainData(QDomElement){return TrainData{false};}; // dummy override
bool UnRecognizedCommand::isElementValid(QDomElement){return false;}; // dummy override


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
    this->timpStationareD = timpStationareD;
    this->timpSosireP = timpSosireP;
    this->timpSosireD = timpSosireD;
    this->timpSosireN = timpSosireN;
    this->timpPlecareP = timpPlecareP;
    this->timpPlecareD = timpPlecareD;
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

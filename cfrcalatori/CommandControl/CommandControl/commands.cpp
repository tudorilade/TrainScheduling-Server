//
// Created by tudor on 02.12.2022.
//
#include <iostream>
#include <cstdlib>
#include "commands.h"
using namespace std;


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

        arg = (char*)malloc(j+1);
        strncpy(arg, command + k - j + 1, j);
        arg[j] = '\0';

        if(strcmp(flag, "-station") == 0)
        {
            this->sizeStation = (size_t)j;
            this->targetStation.assign(arg, this->sizeStation);
        }
        else
        {
            k = 0;
            while(arg[k] != ':'){++k;}
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
        free(flag); free(arg);
    }

    if(this->fromHour && !this->toHourFlag)
        this->toHour = (this->toHour + this->fromHour) % 3600;


    if(this->fromHour > 86340 || this->toHour > 86340)
        this->incorectHourArguments = true;
}


GetRequests::~GetRequests()
{
    this->fromHour = 0;
    this->toHour = 3600;
    this->targetStation = ""; this->sizeStation =  0;
}

bool GetRequests::isCommandIncorrect()
{
    return this->incorectCommand;
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

}

bool GetRequests::isElementValid(QDomElement elementTrasa)
{
    /*
     * returns true if trasa element corresponds to command arguments
     * flase otherwise
    */

    return false;
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


/* UPDATE Command*/
struct CommandResult UpdateTrain::execute_command(){
    cout << "Se execute comanda: Update train route" << endl;
    return test_res;
}

string  UpdateTrain::get_command() {
    return this->command_t;
}


struct CommandResult CreateNewRoute::execute_command(){
    cout << "Se execute comanda: Create new Route" << endl;
    return test_res;
}

string  CreateNewRoute::get_command() {
    return this->command_t;
}


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

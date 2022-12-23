//
// Created by tudor on 02.12.2022.
//
#include <iostream>
#include <cstdlib>
#include "commands.h"
using namespace std;

/* UPDATE Command*/
struct CommandResult UpdateTrain::execute_command(){
    cout << "Se execute comanda: Update train route" << endl;
    return test_res;
}

char*  UpdateTrain::get_command() {
    return this->command;
}




/* GET ARRIVALS Command */
GetArrivals::GetArrivals(char* command, int sd) : Command(command, sd)
{
    bool end = false;
    char* flag;
    char* arg;
    int j = 0;
    for(int i = this->sizeCommand; command[i] != '\0'; i++)
    {
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

        for(j = i; command[i] != ' ';j++); // get the length of the flag

        flag = (char*)malloc(j-i+1); // allocate memory of it

        strncpy(flag, command + i, j-i+1); // check the flag
        flag[j-i] = '\0';

        while(command[j++] == ' '); // remove blank spaces

        i = j; j = 0;
        while(command[i] != ' ')
        {
            j++;
            i++;
        }
        arg = (char*)malloc(j+1);
        strncpy(arg, command + i - j, j);
        arg[j] = '\0';

        if(strcmp(flag, "-station") != 0)
        {
            strncpy(this->targetStation, arg, j);
        }
        else if(strcmp(flag, "-toHour") != 0)
        {
            // ar trebui sa luam ora de genu 00-24, apoi minute: 0-60
            // alg:
            // 3600 * h + 60 * m - nr secunde
        }
        else if(strcmp(flag, "-fromHour") != 0)
        {
            // ar trebui sa luam ora de genu 00-24, apoi minute: 0-60
            // alg:
            // 3600 * h + 60 * m - nr secunde
        }

    }
}


GetArrivals::~GetArrivals()
{

}


struct CommandResult GetArrivals::execute_command(){
    cout << "Se execute comanda: Get Arrivals" << endl;
    return test_res;
}

char*  GetArrivals::get_command() {
    return this->command;
}



/* GET DEPARTURES */
struct CommandResult GetDepartures::execute_command(){
    cout << "Se execute comanda: Get Departures Arrivals" << endl;
    return test_res;
}

char*  GetDepartures::get_command() {
    return this->command;
}

struct CommandResult CreateNewRoute::execute_command(){
    cout << "Se execute comanda: Create new Route" << endl;
    return test_res;
}

char*  CreateNewRoute::get_command() {
    return this->command;
}


struct CommandResult ExitCommand::execute_command(){
    cout << "Se execute comanda: Exit connection" << endl;
    strcpy(test_res.result, "EXIT");
    test_res.size_result = 4;
    test_res.result[4] = '\0';
    return test_res;
}

char*  ExitCommand::get_command() {
    return this->command;
}

char*  UnRecognizedCommand::get_command() {
    return this->command;
}

struct CommandResult UnRecognizedCommand::execute_command() {
    struct CommandResult res {};
    res.result = reinterpret_cast<char*>(malloc(1));
    strcpy(res.result, "0");
    res.size_result = 1;
    return res;
}

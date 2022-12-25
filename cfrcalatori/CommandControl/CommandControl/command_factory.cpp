//
// Created by tudor on 03.12.2022.
//

#include "command_factory.h"
#include "commands.h"
#include <string.h>
#include <iostream>


Command* CommandFactory::create_command(char *command, int sd) {
    string commandS;
    for(int i = 0; command[i] != '-' && command[i] != '\0'; i++)
        commandS += command[i];

    int ndxCom = commandS.size();
    while(commandS[ndxCom--] == ' ');

    commandS.erase(commandS.begin()+ ndxCom, commandS.end());


    if(commandS == "ARRIVALS"){
        return new GetArrivals(command, sd);
    }
    else if(commandS == "DEPARTURES"){
        return new GetDepartures(command, sd);
    }
    else if(commandS == "CREATE"){
        return new CreateNewRoute(command, sd);
    }
    else if(commandS == "UPDATE"){
        return new UpdateTrain(command, sd);
    }
    else if(strcmp(command, "EXIT\n") == 0)
    {
        return new ExitCommand(command, sd);
    }
    else{
        char* unrec_com = static_cast<char*>(malloc(44));
        strcpy(unrec_com, "Comanda gresita ! Va rugam incercati din nou");
        unrec_com[44] = '\0';
        Command* unrec = new UnRecognizedCommand(unrec_com, sd);
        free(unrec_com);
        return unrec;
    }
}

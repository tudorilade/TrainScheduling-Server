//
// Created by tudor on 03.12.2022.
//

#include "command_factory.h"
#include "commands.h"
#include <string.h>
#include <iostream>


Command* CommandFactory::create_command(char *command, int sd, pthread_mutex_t &mutex) {
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
    else if(commandS == "UPDATE"){
        return new UpdateTrain(command, sd, mutex);
    }
    else if(strcmp(command, "EXIT") == 0)
    {
        return new ExitCommand(command, sd);
    }
    else{
        return new UnRecognizedCommand("Comanda gresita ! Va rugam incercati din nou", sd);
    }
}

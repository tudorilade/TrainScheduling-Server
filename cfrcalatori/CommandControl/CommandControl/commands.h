//
// Created by tudor on 02.12.2022.
//

#ifndef CFRCALATORI_COMMANDS_H
#define CFRCALATORI_COMMANDS_H

#include "command_ABC.h"

class UpdateTrain : public Command{
public:
    UpdateTrain(char* command, int sd): Command(command, sd) {};
    struct CommandResult execute_command() override;
    string get_command() override;
};


class GetRequests : public Command
{
protected:
    string targetStation; // trains that arrive in a targetStation
    unsigned int fromHour = 0; // from given hour (request time if not provided)
    unsigned int toHour = 86340; // to given hour (until end of the day if not provided: 23:59)
    string getCommand;
    size_t sizeCommand = 12;
    size_t sizeStation = 0;
    bool fromHourFlag = false, toHourFlag = false, incorectHourArguments = false, incorectCommand = false;
public:
    GetRequests(char* command, int sd);
    GetRequests() = default;
    ~GetRequests();
    string getStationName();
    unsigned int getFromHour();
    unsigned int getToHour();
    size_t getSizeCommand();
    size_t getSizeStationName();
    bool hasFomHourFlag();
    bool hasToHourFlag();
    bool hasIncorectArugments();
    bool isCommandIncorrect();
    TrainData toTrainData(QDomElement);
    bool isElementValid(QDomElement);
};

class GetArrivals : public GetRequests{
public:
    GetArrivals(char*, int);
    GetArrivals() = default;
    ~GetArrivals();
    struct CommandResult execute_command() override;
    string get_command() override;
};


class GetDepartures : public GetRequests{
public:
    GetDepartures(char*, int);
    GetDepartures() = default;
    struct CommandResult execute_command() override;
    string get_command() override;
};


class CreateNewRoute : public Command{
public:
    CreateNewRoute(char* command, int sd) : Command(command, sd) {};
    CreateNewRoute() = default;
    struct CommandResult execute_command() override;
    string get_command() override;
};


class ExitCommand : public Command{
public:
    ExitCommand(char* command, int sd) : Command(command, sd) {};
    ExitCommand() = default;
    struct CommandResult execute_command() override;
    string get_command() override;
};


class UnRecognizedCommand : public Command{
public:
    UnRecognizedCommand(char* command, int sd) : Command(command, sd) {};
    UnRecognizedCommand() = default;
    struct CommandResult execute_command() override;
    string get_command() override;
};


#endif //CFRCALATORI_COMMANDS_H

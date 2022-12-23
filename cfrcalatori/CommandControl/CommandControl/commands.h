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
    char* get_command() override;
};


class GetArrivals : public Command{
private:
    char* targetStation; // trains that arrive in a targetStation
    unsigned int fromHour; // from given hour (request time if not provided)
    unsigned int toHour = 3600; // to given hour (1h default if not provided)
    char* getArrivalsCommand = "GET ARRIVALS";
    size_t sizeCommand = 12;
public:
    GetArrivals(char* command, int sd);
    GetArrivals() = default;
    ~GetArrivals();
    struct CommandResult execute_command() override;
    char* get_command() override;
};


class GetDepartures : public Command{
public:
    GetDepartures(char* command, int sd) : Command(command, sd) {};
    GetDepartures() = default;
    struct CommandResult execute_command() override;
    char* get_command() override;
};


class CreateNewRoute : public Command{
public:
    CreateNewRoute(char* command, int sd) : Command(command, sd) {};
    CreateNewRoute() = default;
    struct CommandResult execute_command() override;
    char* get_command() override;
};


class ExitCommand : public Command{
public:
    ExitCommand(char* command, int sd) : Command(command, sd) {};
    ExitCommand() = default;
    struct CommandResult execute_command() override;
    char* get_command() override;
};


class UnRecognizedCommand : public Command{
public:
    UnRecognizedCommand(char* command, int sd) : Command(command, sd) {};
    UnRecognizedCommand() = default;
    struct CommandResult execute_command() override;
    char* get_command() override;
};


#endif //CFRCALATORI_COMMANDS_H

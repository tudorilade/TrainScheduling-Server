//
// Created by tudor on 02.12.2022.
//

#ifndef CFRCALATORI_COMMANDS_H
#define CFRCALATORI_COMMANDS_H

#include "command_ABC.h"



class GetRequests : public Command
{
protected:
    string targetStation; // trains that arrive in a targetStation
    string stationD; // destination train
    unsigned int fromHour = 0; // from given hour (request time if not provided)
    unsigned int toHour = 86340; // to given hour (until end of the day if not provided: 23:59)
    string getCommand;
    size_t sizeCommand = 12;
    size_t sizeStation = 0;
    size_t sizeStationD = 0;
    bool fromHourFlag = false, toHourFlag = false, incorectHourArguments = false, stationDFlag = false, stationPFlag = false;
public:
    GetRequests(const char* command, int sd);
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
    bool isCommandValid() override;
    void getTrainsInfo(vector<TrainData>&, XmlController&);
};


class GetArrivals : public GetRequests{
public:
    GetArrivals(const char*, int);
    GetArrivals() = default;
    ~GetArrivals();
    struct CommandResult execute(XmlController&) override;
    string get_command() override;
    bool isElementValid(QDomElement&) override;
    TrainData toTrainData(QDomElement&) override;

};


class GetDepartures : public GetRequests{
public:
    GetDepartures(const char*, int);
    GetDepartures() = default;
    struct CommandResult execute(XmlController&) override;
    string get_command() override;
    bool isElementValid(QDomElement&) override;
    TrainData toTrainData(QDomElement&) override;
};


class UpdateTrain : public Command{
private:
    string targetStation; // target station from where delay should be propagated. If non supplied, all the route will be updated with supplied delay.
    string stopStation; // station where the delay propagation should be stopped
    string updateCommand;
    string trainID;
    unsigned int delay = 0;
    size_t sizeCommand = 12;
    size_t sizeStation = 0;
    size_t sizeTrainID = 0;
    bool targetSFlag = false, delayFlag = false, trainIDFlag = false, stopStationFlag = false;
    pthread_mutex_t writeMutex;
public:
    UpdateTrain(const char* command, int sd, pthread_mutex_t&);
    struct CommandResult execute(XmlController&) override;
    string get_command() override;
    bool isCommandValid() override;
    TrainData toTrainData(QDomElement&) override;
    bool isElementValid(QDomElement&) override;
    void updateElement(QDomElement&);

};


class ExitCommand : public Command{
public:
    ExitCommand(const char* command, int sd) : Command(command, sd) {};
    ExitCommand() = default;
    struct CommandResult execute(XmlController&) override;
    string get_command() override;
    bool isCommandValid() override;
    TrainData toTrainData(QDomElement&) override;
    bool isElementValid(QDomElement&) override;
};


class UnRecognizedCommand : public Command{
public:
    UnRecognizedCommand(string command, int sd) : Command(command.c_str(), sd) {};
    UnRecognizedCommand() = default;
    struct CommandResult execute(XmlController&) override;
    string get_command() override;
    bool isCommandValid() override;
    TrainData toTrainData(QDomElement&) override;
    bool isElementValid(QDomElement&) override;
};


class ManCommand : public Command{
    /* Manual class that shows details about commands */
private:
    string manCommand;
    bool arrivalsComm = false, departuresComm = false, updateComm = false;
public:
    ManCommand(const char*, int);
    ManCommand() = default;
    ~ManCommand() = default;
    struct CommandResult execute(XmlController&) override;
    string get_command() override;
    bool isCommandValid() override;
    TrainData toTrainData(QDomElement&) override;
    bool isElementValid(QDomElement&) override;
};


// Utils functions

int checkIfCorrectHourArgument(char*);
string formatContent(const string, const int);
string centerContent(const string, const int);
string secondsToHourMinutes(const unsigned int);
string secondsToMinutes(const unsigned int);
struct CommandResult toTableDepartures(vector<TrainData>&);
struct CommandResult toTableArrivals(vector<TrainData>&);



#endif //CFRCALATORI_COMMANDS_H

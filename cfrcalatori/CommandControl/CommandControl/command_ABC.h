//
// Created by tudor on 02.12.2022.
//

#ifndef CFRCALATORI_COMMAND_ABC_H
#define CFRCALATORI_COMMAND_ABC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <QtXml>
#include "../../XmlController/xmlcontrollerr.h"

using namespace std;

class TrainData
{
/*
    Struct containing needed info about a train
*/
private:
    string numeTren;
    string statieP; // arrival/departure station
    string statieN; // next station
    string statieD; // destination. Last station if not provided
    unsigned int intarziereP; // delay arrival/departure
    unsigned int intarziereD; // delay at final destination
    unsigned int intarziereN; // delay at next station
    unsigned int timpStationareP; // station time at arrival/departure station
    unsigned int timpStationareD; // station time at final destination station
    unsigned int timpStationareN; // station time at next station
    unsigned int timpSosireP; // arrival time at stationP
    unsigned int timpSosireD; // arrival time at stattionD
    unsigned int timpSosireN; // arrival time at next station
    unsigned int timpPlecareP; // departure time at stationP
    unsigned int timpPlecareD; // departure time at stationD
    unsigned int timpPlecareN; // departure time at next station
    bool valid = true; // when statieD provided and we found same statieP but with different statieD than provided

public:
    TrainData(
            string, string, string,string, unsigned int, unsigned int,
            unsigned int, unsigned int, unsigned int, unsigned int,
            unsigned int, unsigned int, unsigned int, unsigned int,
            unsigned int, unsigned int
    );
    TrainData(bool);
    TrainData() = default;
    ~TrainData() = default;
    bool isValid();
    const string toDeparturesString();
    const string toArrivalsString();
};


struct CommandResult{
    string result;
    size_t size_result;
};

class Command{
protected:
    struct CommandResult resultCommand{};
    int sd;
    string command_t;
    size_t size_command;
    bool incorectCommand = false;
public:
    Command(char* com, int sd){
        this->size_command = strlen(com);
        this->sd = sd;
        this->command_t.assign(com, this->size_command);
        this->resultCommand.result.assign("Command test", 12);
    }
    Command() = default;
    ~Command(){command_t = ""; sd = 0; size_command = 0; resultCommand = {};}

    struct CommandResult getResult(){return this->resultCommand;};
    virtual struct CommandResult execute(XmlController&) = 0;
    virtual string get_command() = 0;
    virtual bool isCommandValid() = 0;
    virtual bool isElementValid(QDomElement) = 0;
    virtual TrainData toTrainData(QDomElement) = 0;
    [[nodiscard]] int get_client_sd() const{return this->sd;};
    [[nodiscard]] size_t get_size_command()const{return this->size_command;};

};

#endif //CFRCALATORI_COMMAND_ABC_H

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


using namespace std;

class TrainData
{
/*
    Struct containing needed info about a train
*/
private:
    string numeTren;
    string statieCurenta;
    string statieDestinatie;
    string finalDestination;
    unsigned int intarziere;
    unsigned int timpStationare;
    unsigned int timpSosire;
    unsigned int timpPlecare;
    unsigned int timpSosireFinal;
    unsigned int delayFinal;

public:
    TrainData(
            string, string, string,string, unsigned int, unsigned int,
            unsigned int, unsigned int, unsigned int, unsigned int
    );
    TrainData() = default;
    ~TrainData() = default;
    string toString();
    string convertFromSeconds();
};


struct CommandResult{
    string result;
    size_t size_result;
};

class Command{
protected:
    struct CommandResult test_res{};
    int sd;
    string command_t;
    size_t size_command;
public:
    Command(char* com, int sd){
        this->size_command = strlen(com);
        this->sd = sd;
        this->command_t.assign(com, this->size_command);
        test_res.result.assign("Command test", 12);
    }
    Command() = default;
    ~Command(){command_t = ""; sd = 0; size_command = 0; test_res = {};}

    virtual struct CommandResult execute_command() = 0;
    virtual string get_command() = 0;
    [[nodiscard]] int get_client_sd() const{return this->sd;};
    [[nodiscard]] size_t get_size_command()const{return this->size_command;};

};

#endif //CFRCALATORI_COMMAND_ABC_H

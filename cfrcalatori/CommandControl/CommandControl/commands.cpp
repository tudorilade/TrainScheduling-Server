//
// Created by tudor on 02.12.2022.
//
#include <iostream>
#include <cstdlib>
#include "commands.h"
using namespace std;


struct CommandResult UpdateTrain::execute_command(){
    cout << "Se execute comanda: Update train route" << endl;
    return test_res;
}

char*  UpdateTrain::get_command() {
    return this->command;
}

struct CommandResult GetArrivals::execute_command(){
    cout << "Se execute comanda: Get Arrivals" << endl;
    return test_res;
}

char*  GetArrivals::get_command() {
    return this->command;
}

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
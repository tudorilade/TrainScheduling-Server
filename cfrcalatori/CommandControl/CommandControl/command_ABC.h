//
// Created by tudor on 02.12.2022.
//

#ifndef CFRCALATORI_COMMAND_ABC_H
#define CFRCALATORI_COMMAND_ABC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct CommandResult{
    char* result;
    size_t size_result;
};

class Command{
protected:
    struct CommandResult test_res{};
    int sd;
    char* command;
    size_t size_command;
public:
    Command(char* com, int sd){
        this->size_command = strlen(com);
        this->sd = sd;
        this->command = reinterpret_cast<char*>(malloc(this->size_command));
        this->command[size_command] = '\0';
        this->test_res.result = reinterpret_cast<char*>(malloc(12));
        strcpy(test_res.result, "Command test");
        test_res.result[12] = '\0';
        test_res.size_result = 12;
    }
    Command() = default;
    ~Command(){free(command); sd = 0; size_command = 0; test_res = {};}

    virtual struct CommandResult execute_command() = 0;
    virtual char* get_command() = 0;
    [[nodiscard]] int get_client_sd() const{return this->sd;};
    [[nodiscard]] size_t get_size_command()const{return this->size_command;};

};

#endif //CFRCALATORI_COMMAND_ABC_H

//
// Created by tudor on 03.12.2022.
//

#ifndef CFRCALATORI_COMMAND_FACTORY_H
#define CFRCALATORI_COMMAND_FACTORY_H

#include "command_ABC.h"

class CommandFactory{
public:
    static Command* create_command(char* command, int sd, pthread_mutex_t& mutex);
};

#endif //CFRCALATORI_COMMAND_FACTORY_H

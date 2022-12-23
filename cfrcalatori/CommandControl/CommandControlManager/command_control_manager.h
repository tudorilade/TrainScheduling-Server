//
// Created by tudor on 02.12.2022.
//

#ifndef CFRCALATORI_COMMAND_CONTROL_MANAGER_H
#define CFRCALATORI_COMMAND_CONTROL_MANAGER_H
#include <thread>
#include <deque>
#include "../CommandControl/command_ABC.h"

class CommandManager{
private:
    bool running { false };
private:
    std::thread commandThreads;
    std::deque<Command*> queue {};
public:
    CommandManager() = default;
    ~CommandManager();
    void ManageCommands();
    void RunCommands();
    void QueueCommands(Command*);
    void executeCommands(Command*);

};

#endif //CFRCALATORI_COMMAND_CONTROL_MANAGER_H

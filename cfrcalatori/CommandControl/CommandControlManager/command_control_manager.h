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
    XmlController xmlFile;
private:
    std::thread commandThreads;
    std::thread xmlThread;
    std::deque<Command*> queue {};
public:
    CommandManager() = default;
    CommandManager(XmlController);
    ~CommandManager();
    void ManageCommands();
    void RunCommands();
    void updateXmlFile();
    void QueueCommands(Command*);
    void executeCommands(Command*);

};

#endif //CFRCALATORI_COMMAND_CONTROL_MANAGER_H

//
// Created by tudor on 02.12.2022.
//

#include <iostream>
#include <thread>
#include <cstring>
#include "command_control_manager.h"
#include "../../Requests/RequestsController/requests_controller.h"

using namespace  std;


CommandManager::~CommandManager() {
    if(commandThreads.joinable()) // if it is still running
    {
        running = false; // shut down RunCommands while loop
        commandThreads.join(); // make sure  we finish to process the ongoing command
    }
}

CommandManager::CommandManager(XmlController xmlFile)
{
    this->xmlFile = xmlFile;
}

void CommandManager::ManageCommands(){
    std::cout << "Starting Command Manager\n" << std::endl;
    if(commandThreads.joinable())
        commandThreads.join();

    queue.clear();
    commandThreads = std::thread(&CommandManager::RunCommands, this);
}


void CommandManager::RunCommands() {
    std::cout << "Comands ready to be executed" << std::endl;

    running = true;
    while( running )
    {
        if(queue.empty())
            continue;
        this->executeCommands(queue.front());
        queue.pop_front();
    }

    std::cout << "Commands finished to run" << std::endl;
}

void CommandManager::QueueCommands(Command* comm) {
    std::cout << "Queueing command" << std::endl;
    if(!running)
        return; // Queuing not started

    queue.push_back(comm);
}

void CommandManager::executeCommands(Command * command) {

    struct CommandResult res = command->execute(this->xmlFile);
    char *out_msg = static_cast<char *>(malloc(res.size_result+1));
    strncpy(out_msg, res.result.c_str(), res.size_result);
    RequestsController::send_message(command->get_client_sd(), out_msg);
    if(res.result.compare("EXIT") == 0){
        running = false;
    }

    free(out_msg);

}

#ifndef CFRCALATORI_MAIN_H
#define CFRCALATORI_MAIN_H

#include "ClientController/client_controller.h"

class MainManager{

public:
    MainManager() = default;
    ~MainManager() = default;
    void start(){
        ClientController client_controller;
        client_controller.start_server();
        client_controller.manage_clients();
    }

};

#endif //CFRCALATORI_MAIN_H

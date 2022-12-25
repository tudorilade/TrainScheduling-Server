//
// Created by tudor on 01.12.2022.
//

#include "../../ClientThreadFactory/thread_factory.h"
#ifndef CFRCALATORI_REQUESTS_CONTROLLER_H
#define CFRCALATORI_REQUESTS_CONTROLLER_H

class RequestsController{
private:
    char* ERROR_REQUEST;
    int connection {1};
private:
    void message_to_upper(char*);
public:
    RequestsController(){
        ERROR_REQUEST = static_cast<char*>(malloc(51));
        strncpy(ERROR_REQUEST, "Reqestul nu a putut fi procesat. Incercati din nou", 51);
        ERROR_REQUEST[51] = '\0';
    };
    ~RequestsController(){
        free(ERROR_REQUEST);
    };
    void handle_request(void* arg);
    static int send_message(const int&, char*);
    static int receive_message(const int&, char*&);
    int check_close_connection_request(char*);
};

#endif //CFRCALATORI_REQUESTS_CONTROLLER_H

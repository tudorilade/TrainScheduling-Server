#include <iostream>
#include <cstdio>
#include <cstring>
#include "requests_controller.h"
#include "../../CommandControl/CommandControlManager/command_control_manager.h"
#include "../../CommandControl/CommandControl/command_factory.h"

pthread_mutex_t writeXmlLock=PTHREAD_MUTEX_INITIALIZER;

using namespace std;

int RequestsController::send_message(const int &fd, char* message)
{
    size_t length = strlen(message);
    if(-1 == write(fd, &length, sizeof(size_t)))
        return -1;

    if(-1 == write(fd, message, length))
        return -1;

    return 0;
}

int RequestsController::receive_message(const int &client, char* &in_message)
{
    int offset=0, offset1;
    size_t stream_c = 0;
    offset1 = read(client, &stream_c, sizeof(size_t));
    if(offset1 == -1)
        return -1;
    if(offset1 == 0)
        return -2;


    in_message = (char*)malloc(stream_c);
    offset = read(client, in_message, stream_c);
    if(stream_c != offset)
        return -1;
    in_message[stream_c-1] = '\0'; // remove \n
    return 0;
}

int RequestsController::check_close_connection_request(char* in_message){
    return strcmp(in_message, "EXIT");
}

void RequestsController::message_to_upper(char* mess){
    for(int i = 0; mess[i] != '-' && mess[i] != '\0'; i++)
        mess[i] = (char)toupper(mess[i]);
}

void RequestsController::handle_request(void *arg) {
    char* in_msg;
    struct thData tdL {};
    int status = 1;
    XmlController xmlFile;
    CommandManager manager {xmlFile};
    tdL= *((struct thData*)arg);
    manager.ManageCommands();
    while( connection )
    {
        if(-1 == (status = this->receive_message(tdL.client, in_msg))){
            std::cout << "Eroare citire din thread: " << tdL.idThread << std::endl;
            connection = 0; // CLOSING CLIENT CONNECTION
            close(tdL.client);
            continue;
        }
        if(status == -2)
        {
            // SIGINT and SIGSTP
            close(tdL.client);
            connection = 0;
            continue;
        }
        this->message_to_upper(in_msg); // upper just ARRIVALS - DEPARTURES etc.
        printf ("\n[Thread %d]A fost primita comanda: %s", tdL.idThread, in_msg);
        Command* comm = CommandFactory::create_command(in_msg, tdL.client, writeXmlLock);
        manager.QueueCommands(comm);
        if(check_close_connection_request(in_msg) == 0){
            connection = 0; // CLOSING CLIENT CONNECTION
            printf("\n[Thread %d]Conexiunea se va inchide.\n", tdL.idThread);
        }
        free(in_msg);
    }
}

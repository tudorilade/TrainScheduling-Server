#include <iostream>
#include <cstdio>
#include <cstring>
#include "requests_controller.h"
#include "../../CommandControl/CommandControlManager/command_control_manager.h"
#include "../../CommandControl/CommandControl/command_factory.h"
#include "../../CommandControl/CommandControl/command_ABC.h"

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

int RequestsController::receive_message(const int &client, char* in_message)
{
    int offset=0, offset1;
    size_t stream_c = 0;
    offset1 = read(client, &stream_c, sizeof(size_t));
    if(offset1 == -1)
        return -1;
    offset = read(client, in_message, stream_c);
    if(stream_c != offset)
        return -1;
    in_message[stream_c] = '\0';
    return 0;
}

int RequestsController::check_close_connection_request(char* in_message){
    return strcmp(in_message, "EXIT\n");
}

void RequestsController::message_to_upper(char* mess){
    for(int i = 0; mess[i] != '\0'; i++)
        mess[i] = (char)toupper(mess[i]);
}

void RequestsController::handle_request(void *arg) {
    char* in_msg;
    struct thData tdL {};
    CommandManager manager;

    tdL= *((struct thData*)arg);
    manager.ManageCommands();
    while( connection )
    {
        in_msg = static_cast<char *>(malloc(4096));
        if(-1 == this->receive_message(tdL.client, in_msg)){
            std::cout << "Eroare citire din thread: " << tdL.idThread << std::endl;
            this->send_message(tdL.client, this->ERROR_REQUEST); free(in_msg);
            continue;
        }
        this->message_to_upper(in_msg);
        printf ("\n[Thread %d]A fost primita comanda: %s", tdL.idThread, in_msg);
        Command* comm = CommandFactory::create_command(in_msg, tdL.client);
        manager.QueueCommands(comm);
        if(check_close_connection_request(in_msg) == 0){
            connection = 0; // CLOSING CLIENT CONNECTION
            printf("\n[Thread %d]Conexiunea se va inchide.\n", tdL.idThread);
        }
        free(in_msg);
    }
}
//
// Created by tudor on 01.12.2022.
//
#include "client_controller.h"
#include <iostream>

using namespace std;

void ClientController::start_server() {
    printf("Pornim serverul !\n");
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
        throw "[SERVER] Eroare creare socket()\n";

    setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
    memset (&server, 0, sizeof (server));
    memset (&from, 0, sizeof (from));

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl (INADDR_ANY);
    server.sin_port = htons (PORT);

    if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
        throw "[server]Eroare la bind().\n";

    std::cout << "Serverul CFRCalatori a pornit!" << std::endl;
}

[[noreturn]] void ClientController::manage_clients() {
    if (listen (sd, 2) == -1)
        throw "[server]Eroare la listen().\n";
    int client = -1;
    while( true )
    {
        socklen_t length = sizeof (from);
        pthread_mutex_lock(&mlock);
        if ((client = accept (sd, (struct sockaddr *) &from, &length)) < 0 )
        {
            perror ("[server] Eroare la accept().\n");
            pthread_mutex_unlock(&mlock);
            continue;
        }
        pthread_mutex_unlock(&mlock);
        thread_factory.create_thread(client);
    }


}

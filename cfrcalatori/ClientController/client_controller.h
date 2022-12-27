#ifndef CFRCALATORI_CLIENT_CONTROLLER_H
#define CFRCALATORI_CLIENT_CONTROLLER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include "../ClientThreadFactory/thread_factory.h"


#define PORT 2049

using namespace std;

class ClientController{

protected:
    struct sockaddr_in server {};	// server info
    struct sockaddr_in from {};   // client info
    int sd = -1, on = 1;
    ThreadFactory thread_factory {};
    pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER;

public:
    ClientController() = default;
    ~ClientController() = default;
    void start_server();
    [[noreturn]] void manage_clients();
};



#endif

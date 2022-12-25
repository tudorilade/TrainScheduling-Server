//
// Created by tudor on 01.12.2022.
//

#ifndef CFRCALATORI_THREAD_FACTORY_H
#define CFRCALATORI_THREAD_FACTORY_H
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define THREADS 10000

typedef struct thData{
    int idThread;
    int client; // client descriptor
}thData;


class ThreadFactory{
protected:
    int i = 0 ;
    pthread_t th[THREADS] {};    //identificatorii threadurilor

public:
    ThreadFactory() = default;
    void create_thread(int);
};

#endif //CFRCALATORI_THREAD_FACTORY_H

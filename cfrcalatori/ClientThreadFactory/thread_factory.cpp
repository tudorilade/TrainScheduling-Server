//
// Created by tudor on 01.12.2022.
//
#include "thread_factory.h"
#include "../Requests/RequestsController/requests_controller.h"
#include <pthread.h>


static void *thread_data_handler(void * arg)
{
    struct thData tdL {};
    RequestsController request_controller;

    tdL= *((struct thData*)arg);
    pthread_detach(pthread_self());

    request_controller.handle_request((struct thData*)arg);

    close ((intptr_t)arg);
    return(nullptr);
};

void ThreadFactory::create_thread(int client){
    thData* td;
    td=(struct thData*)malloc(sizeof(struct thData));
    td->idThread=i++;
    td->client=client;
    pthread_create(&th[i], nullptr, &thread_data_handler, td);
}


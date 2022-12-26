#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>

int send_message_to_client(int fd, char* message)
{
    size_t length = strlen(message);
    int nr = 0;
    if(-1 == write(fd, &length, sizeof(length)))
        return -1;

    if(-1 == write(fd, message, length))
        return -1;
    return 0;
}

int receive_message_from_client(int client, char* in_message)
{
    free(in_message);
    int offset=0, stream_c=0;
    if((offset = read(client, &stream_c, sizeof(int))) == -1)
        return -1;
    in_message = (char*)malloc(stream_c * sizeof(char) + 1);
    offset = read(client, in_message, stream_c);
    if(stream_c != offset)
        return -1;

    in_message[stream_c] = '\0';
    return 0;
}

void to_upper(char* msg)
{
   for(int i = 0; msg[i] != '\0'; i++){
     msg[i] = toupper(msg[i]);
   }

}

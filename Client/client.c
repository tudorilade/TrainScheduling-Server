#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>

extern int errno;

int port;

void to_upper(char*);

int main (int argc, char *argv[])
{
    int sd;			// descriptorul de socket
    struct sockaddr_in server;	// structura folosita pentru conectare
    // mesajul trimis
    int nr=0;

    /* exista toate argumentele in linia de comanda? */
    if (argc != 3)
    {
        printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
        return -1;
    }

    /* stabilim portul */
    port = atoi (argv[2]);

    /* cream socketul */
    if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror ("Eroare la socket().\n");
        return errno;
    }

    /* umplem structura folosita pentru realizarea conexiunii cu serverul */
    /* familia socket-ului */
    server.sin_family = AF_INET;
    /* adresa IP a serverului */
    server.sin_addr.s_addr = inet_addr(argv[1]);
    /* portul de conectare */
    server.sin_port = htons (port);

    /* ne conectam la server */
    if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
        perror ("[client]Eroare la connect().\n");
        return errno;
    }
    char* msg = (char*)malloc(1000);
    char* buf = (char*)malloc(4096);
    int bytes = 0;
    while( NULL != fgets(buf, 4096, stdin), !feof(stdin))
    {

        size_t length = strlen(buf);
        buf[length] = '\0';

        printf("[client] Am citit %s", buf);

        /* trimiterea mesajului la server */
        write(sd, &length, sizeof(size_t));

        if (write (sd, buf, length) <= 0)
        {
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
        }


        read(sd, &length, sizeof(size_t));

        msg = (char*)malloc(length);

        if ((bytes = read (sd, msg, length)) < 0)
        {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
        }
        msg[bytes] = '\0';
        printf ("[client] Mesajul primit este: %s\n", msg);
        to_upper(buf);
        if(strcmp(buf, "EXIT\n") == 0)
        {
            free(msg); free(buf); break;
        }

        free(msg); free(buf);
        buf = (char*)malloc(4096);
    }
    close (sd);
}


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
#include "mainwindow.h"
#include <QApplication>
#include "utils/sig_handlers.h"

extern int errno;

int connection = 1;
int sd;			// descriptorul de socket
int port;

void sig_stp(int signal)
{
	connection = 0;
	close(sd);
	_exit(1);
}

int main (int argc, char *argv[])
{
    struct sigaction act;
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
    char* msg = (char*)malloc(4096);
    char* buf = (char*)malloc(4096);
    signal(SIGTSTP, sig_stp);
    signal(SIGINT, sig_stp);
    act.sa_handler = sig_stp;
    sigaction(SIGINT, &act, 0);
    sigaction(SIGTSTP, &act, 0);
    sigaction(SIGTERM, &act, 0);

    while( NULL != fgets(buf, 4096, stdin), !feof(stdin))
    {
    
        size_t length = strlen(buf);
        buf[length] = '\0';

        printf("[client] Am citit de la tastatura: %s", buf);

        /* trimiterea mesajului la server */
        write(sd, &length, sizeof(size_t));

        if (write (sd, buf, length) <= 0)
        {
            perror ("[client]Eroare la write() spre server.\n");
            return errno;
        }


        read(sd, &length, sizeof(size_t));

        msg = (char*)malloc(length + 1);

        if ((read (sd, msg, length)) <= 0)
        {
            perror ("[client]Eroare la read() de la server.\n");
            return errno;
        }
        msg[length] = '\0';
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

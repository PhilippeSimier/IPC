/* 
 * File:   main.c
 * Author: psimier
 *
 * Created on 11 octobre 2023, 16:55
 * 
 * Client tchat
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char** argv) {

    int fd_socket_client;
    struct sockaddr_in infoServeur;
    struct sockaddr_in infoReception;
    int retour;

    char valToSend[255];
    char valReceived[255];
    int taille;

    while (1) {

        //Definition mode de communication (socket)
        fd_socket_client = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (fd_socket_client == -1) {
            printf("pb socket : %s\n", strerror(errno));
            exit(errno);
        }

        //Init structure sockaddr_in
        infoServeur.sin_family = AF_INET;
        infoServeur.sin_port = htons(9999);
        infoServeur.sin_addr.s_addr = inet_addr(argv[1]);

        //Connect
        retour = connect(fd_socket_client, (struct sockaddr *) &infoServeur, sizeof (infoServeur));

        //Envoyer un message lu au clavier 
        fgets(valToSend, sizeof (valToSend), stdin); //lecture du message au clavier
        retour = write(fd_socket_client, valToSend, sizeof (valToSend));
        if (retour == -1) {
            printf("probleme l envoi : %s\n", strerror(errno));
            exit(errno);

        }

        //Recevoir
        taille = sizeof (infoReception);
        retour = read(fd_socket_client, valReceived, sizeof (valReceived));

        if (retour == -1) {
            printf("probleme avec le reception : %s\n", strerror(errno));
            exit(errno);

        } else {
            printf("Serveur : %s\n", valReceived);
        }

        //Fermer socket
        retour = close(fd_socket_client);

        if (retour == -1) {
            printf("probleme avec la fermeture de la socket : %s\n", strerror(errno));
            exit(errno);

        }



    }





    return (EXIT_SUCCESS);
}



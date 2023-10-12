/* 
 * File:   main.c
 * Author: psimier
 * 
 * Serveur chat
 * Created on 11 octobre 2023, 15:59
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
    int fd_sock_serveur;
    int fd_sock_client;
    struct sockaddr_in socket_serveur;
    struct sockaddr_in socket_client;

    int  retour;
    int  taille;
    char valToSend[255];
    char valReceived[255];



    //Definition mode de communication (socket TCP)
    fd_sock_serveur = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (fd_sock_serveur == -1) {
        printf("pb socket : %s\n", strerror(errno));
        exit(errno);
    }

    //Init structure sockaddr_in
    socket_serveur.sin_family = AF_INET;
    socket_serveur.sin_port = htons(9999);
    socket_serveur.sin_addr.s_addr = INADDR_ANY;

    //Bind
    retour = bind(fd_sock_serveur, 
            (struct sockaddr *) &socket_serveur, 
             sizeof (socket_serveur));
    if (retour == -1) {
        printf("probleme avec bind : %s\n", strerror(errno));
        exit(errno);

    }

    //Listen
    retour = listen(fd_sock_serveur, 10);
    if (retour == -1) {
        printf("probleme listen : %s\n", strerror(errno));
        exit(errno);

    }

    while (1) {
        taille = sizeof (socket_client);
        fd_sock_client = accept(fd_sock_serveur, (struct sockaddr *) &socket_client, &taille);

        //Reception d'un message du client
        retour = read(fd_sock_client, &valReceived, sizeof (valReceived));

        printf("Client : %s\n", valReceived);
        if (retour == -1) {
            printf("probleme avec la fermeture de la socket : %s\n", strerror(errno));
            exit(errno);

        }


        //Envoyer une réponse au client 
        fgets(valToSend, sizeof(valToSend), stdin); // lecture du clavier
        retour = write(fd_sock_client, &valToSend, sizeof (valToSend));
        if (retour == -1) {
            printf("probleme avec write dans la socket : %s\n", strerror(errno));
            exit(errno);

        }
    }

    //Fermer socket
    retour = close(fd_sock_serveur);

    if (retour == -1) {
        printf("probleme avec la fermeture de la socket : %s\n", strerror(errno));
        exit(errno);

    }



    return (EXIT_SUCCESS);
}

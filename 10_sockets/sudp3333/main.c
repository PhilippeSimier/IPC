/*
 * Serveur UDP port 3333
 * 
 * File:   main.c
 * Author: psimier
 *
 * Created on 8 octobre 2021, 10:29
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char** argv) {

    int fdSocket;

    struct sockaddr_in adresseServeur;
    struct sockaddr_in adresseClient;

    int retour;
    char buffer[255];
    int tailleclient;
    float valRec;

    printf("serveur UDP sur port 3333 attend un Reel\n");
    tailleclient = sizeof (adresseClient);

    // Création de la socket
    fdSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (fdSocket == -1) {
        printf("pb socket : %s\n", strerror(errno));
        exit(1);
    }

    // Bind
    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(3333);
    adresseServeur.sin_addr.s_addr = htons(INADDR_ANY); // ecoute toutes les adresses

    retour = bind(fdSocket,
             (struct sockaddr*) &adresseServeur,
             sizeof (adresseServeur));
    if (retour == -1) {
        printf("pb bind : %s\n", strerror(errno));
        exit(2);
    }

    // ecoute du client avec recvfrom
    while (1) {

        retour = recvfrom(fdSocket,
                &valRec,           // le buffer de réception
                sizeof (valRec),   // La taille du buffer de reception
                MSG_WAITALL,       // Le flag  lecture bloquée  jusqu'à  ce  que  la requête  complète  soit  satisfaite
                (struct sockaddr*) &adresseClient, // IN L'adresse du client
                &tailleclient);  // IN la taille de l'adresse du client

        if (retour == -1) {
            printf("pb recvfrom : %s\n", strerror(errno));
            exit(3);
        }

        // affichage de la reception des valeurs entières
        printf("Message recu du client %s:%d -> %.2f\n",
                inet_ntoa(adresseClient.sin_addr),
                adresseClient.sin_port,
                valRec);

        // calcule de la réponse
        valRec *= -1.0;

        // envoyer la réponse au client
        retour = sendto(fdSocket,
                &valRec,
                sizeof (valRec),
                0,
                (struct sockaddr*) &adresseClient,
                sizeof (adresseClient));

        if (retour == -1) {
            printf("pb sendto : %s\n", strerror(errno));
            exit(4);
        }
    }

    return (EXIT_SUCCESS);
}


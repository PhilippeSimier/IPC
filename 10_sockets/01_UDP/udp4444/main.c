/*
 * Client udp 4444		  					   *
 * Envoie une date au serveur udp et affiche la valeur retournée 
 */

/* 
 * File:   main.c
 * Author: psimier
 *
 * Created on 8 octobre 2021, 09:31
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

typedef struct {
    unsigned char jour;
    unsigned char mois;
    unsigned short int annee;
    char jourDeLaSemaine[10]; // le jour en toute lettre
} datePerso;

int main(int argc, char *argv[]) {

    int fdSocket;
    datePerso valeurEnv;
    datePerso valeurRet;
    struct sockaddr_in adresseServeur;
    struct sockaddr_in adresseServeurReponse;
    int retour, tailleReponse;

    fdSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (fdSocket == -1) {
        printf("pb socket : %s\n", strerror(errno));
        exit(1);
    }

    adresseServeur.sin_family = AF_INET;

    adresseServeur.sin_port = htons(4444); //numero de port du serveur dans l'ordre des octets du réseau
    adresseServeur.sin_addr.s_addr = inet_addr(argv[1]); // adresse IP du serveur dans l'ordre des octets du reseau

    valeurEnv.jour = 5;
    valeurEnv.mois = 10;
    valeurEnv.annee = 2023;
    strcpy(valeurEnv.jourDeLaSemaine, "jeudi");

    while (1) {

        
        retour = sendto(fdSocket,
                &valeurEnv,
                sizeof (valeurEnv),
                0,
                (struct sockaddr *) &adresseServeur,
                sizeof (adresseServeur));

        if (retour == -1) {
            printf("pb sendto : %s\n", strerror(errno));
            exit(errno);
        }
        // reponse du serveur
        retour = recvfrom(fdSocket,
                &valeurRet,
                sizeof (valeurRet),
                0,
                (struct sockaddr *) &adresseServeurReponse,
                &tailleReponse);

        if (retour == -1) {
            printf("pb recvfrom : %s\n", strerror(errno));
            exit(errno);
        }
        printf("%s %d %d %d\n",
                valeurRet.jourDeLaSemaine,
                valeurRet.jour,
                valeurRet.mois,
                valeurRet.annee);
        sleep(1);

    }
    return EXIT_SUCCESS;
}


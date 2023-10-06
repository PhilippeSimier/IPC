/* 
 * File:   main.c
 * Author: psimier
 *
 * Created on 6 octobre 2023, 15:12
 * client TCP port 5555
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {

    int fdSocket;

    struct sockaddr_in adresseServeur;
    int retour;
    int tailleClient;

    int valeurEnv, valeurRet = 0;


    adresseServeur.sin_family = AF_INET;
    adresseServeur.sin_port = htons(5555); //numero de port du serveur dans l'ordre des octets du réseau
    adresseServeur.sin_addr.s_addr = inet_addr(argv[1]); // adresse IP du serveur dans l'ordre des octets du reseau





    while (1) {

        fdSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (fdSocket == -1) {
            printf("pb socket : %s\n", strerror(errno));
            exit(errno);
        }

        //demande de connection au serveur
        retour = connect(fdSocket,
                (struct sockaddr *) & adresseServeur,
                sizeof (adresseServeur));

        if (retour == -1) {
            printf("pb connect : %s\n", strerror(errno));
            exit(errno);
        }

        printf("Entier a envoyer : ");
        scanf("%d", &valeurEnv);

        retour = write(fdSocket, &valeurEnv, sizeof (valeurEnv)); // envoyer donnees au serveur

        if (retour == -1) {
            printf("pb connect : %s\n", strerror(errno));
            exit(errno);
        }

        retour = read(fdSocket, &valeurRet, sizeof (valeurRet));
        if (retour == -1) {
            printf("pb connect : %s\n", strerror(errno));
            exit(errno);
        }
        printf("%s: %d\n", inet_ntoa(adresseServeur.sin_addr), valeurRet);

        close(fdSocket);

    }


    return EXIT_SUCCESS;
}


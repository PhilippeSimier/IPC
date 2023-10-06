/* 
 * File:   main.c
 * Author: psimier
 *
 * Created on 5 octobre 2023, 17:26
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    int fdSocket, fdSocketClient;

    struct sockaddr_in adresseServeur;
    struct sockaddr_in adresseClient;

    int retour;
    int tailleClient;
    char buffer[255];
    int valeurEnv, valeurRet = 0;


    printf("serveur TCP sur port 5555 attend un ENTIER\n");
    adresseServeur.sin_family = AF_INET;

    adresseServeur.sin_port = htons(5555); //numero de port du serveur dans l'ordre des octets du réseau
    adresseServeur.sin_addr.s_addr = INADDR_ANY; // adresse IP du serveur dans l'ordre des octets du reseau
    
    fdSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (fdSocket == -1) {
        printf("pb socket : %s\n", strerror(errno));
        exit(errno);
    }
    retour = bind(fdSocket, 
             (struct sockaddr*) &adresseServeur, 
             sizeof ( adresseServeur)
            ); //attachement ip-port

    if (retour == -1) {
        printf("pb bind : %s\n", strerror(errno));
        exit(errno);
    }

    retour = listen(fdSocket, 10); //je peux gerer 10 clients en file d'attente
    if (retour == -1) {
        printf("pb listen : %s\n", strerror(errno));
        exit(errno);
    }

    tailleClient = sizeof ( adresseClient);


    while (1) {
        
        fdSocketClient = accept(fdSocket, 
                (struct sockaddr *) &adresseClient, 
                &tailleClient); // acceptation de la connexion client et creation de la socket de communication

        if (fdSocketClient == -1) {
            printf("pb accept : %s\n", strerror(errno));
            exit(errno);
        }

        retour = read(fdSocketClient, &valeurEnv, sizeof ( valeurEnv));
        
        if (retour == -1) {
            printf("pb connect : %s\n", strerror(errno));
            exit(errno);
        }
        
        printf("%s: %d\n", inet_ntoa(adresseClient.sin_addr), valeurEnv);
        valeurRet = -valeurEnv;

        retour = write(fdSocketClient, &valeurRet, sizeof ( valeurRet)); // envoyer donnees au serveur

        if (retour == -1) {
            printf("pb connect : %s\n", strerror(errno));
            exit(0);
        }


        close(fdSocketClient);

        /*memset(buffer, '\0', 255);
        read(fdSocket,buffer,255);
        printf("reposne: %s\n",buffer);*/
    }


    return EXIT_SUCCESS;
}


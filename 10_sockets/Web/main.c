/* 
 * File:   main.c
 * Author: psimier
 *
 * Created on 11 octobre 2023, 15:59
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
#include <arpa/inet.h>

// Prototype d'une fonction de service
void service(int fdSocketClient);
// Prototype d'une fonction de service
void envoyer404(int fdSocketClient, char* fichier);

int main(int argc, char** argv) {

    int fdSocketServeur;
    int fdSocketClient;

    struct sockaddr_in adresseServeur;
    struct sockaddr_in adresseClient;

    int retour = 0;

    // socket
    fdSocketServeur = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // AF_INET désigne la famille de protocole de couche réseau IPv4.
    if (fdSocketServeur == -1) {
        printf("pb socket : %s\n", strerror(errno));
        exit(1);
    }

    //bind 
    adresseServeur.sin_family = AF_INET; // AF_INET désigne la famille de protocole de couche réseau IPv4.
    adresseServeur.sin_port = htons(8888);
    adresseServeur.sin_addr.s_addr = htonl(INADDR_ANY);
    retour = bind(fdSocketServeur, (struct sockaddr*) &adresseServeur, sizeof (adresseServeur)); //attachement ip-port
    if (retour == -1) {
        printf("pb bind : %s\n", strerror(errno));
        exit(2);
    }

    // listen
    retour = listen(fdSocketServeur, 10); //10 clients maxi en file d'attente
    if (retour == -1) {
        printf("pb listen : %s\n", strerror(errno));
        exit(2);
    }

    int tailleClient = sizeof (adresseClient);

    while (1) {
        printf("Attente de requête sur le port 8888\n");

        // accept (acceptation de la connexion client) 
        // et creation de la socket de communication

        fdSocketClient = accept(fdSocketServeur,
                (struct sockaddr *) &adresseClient,
                &tailleClient);
        // appel de la fonction de service
        service(fdSocketClient);

    }
    
    return (EXIT_SUCCESS);
}

/**
 * Fonction de service repond a la requête du client 
 */
void service(int fdSocketClient) {

    char requete[512];
    int retour;

    char reponse[2048];
    char reponseComplete[4096];
    memset(reponseComplete, '\0', 4096);

    // Lecture de la requête
    retour = read(fdSocketClient, requete, 512);
    if (retour == -1) {
        printf("pb read : %s\n", strerror(errno));
        exit(3);
    }

    // découpage de la requête pour obtenir les trois premiers mots
    char *mot[10];
    mot[0] = strtok(requete, " \t\n");
    mot[1] = strtok(NULL, " \t\n");
    mot[2] = strtok(NULL, " \t\n");


    // si requéte GET lecture du fichier demandé 
    if (strcmp(mot[0], "GET") == 0) {
        printf("requete GET OK\n");
        printf("fichier demandé  %s\n", mot[1]);
        printf("protocole  %s\n\n", mot[2]);

        FILE * frep = fopen(mot[1] + 1, "r");
        if (frep == NULL) {
            printf("pb avec fopen %s\n", strerror(errno));
            printf(">> Erreur 404: Fichier non trouvé\n");
            envoyer404(fdSocketClient, mot[1] + 1);

        } else {
            memset(reponse, '\0', 2048);
            retour = fread(reponse, 1, 2048, frep);
            fclose(frep);

            sprintf(reponseComplete,
                    "HTTP/1.1 200 OK\nContent-length: %d\nContent-Type: text/html\n\n",
                    strlen(reponse));
            strncat(reponseComplete, reponse, 4096);
            printf("%s\n", reponseComplete);

            // ecriture de la réponse

            retour = write(fdSocketClient, reponseComplete, strlen(reponseComplete));
        }
        shutdown(fdSocketClient, SHUT_RDWR);
        close(fdSocketClient);
    }
}

void envoyer404(int fdSocketClient, char* fichier) {
    write(fdSocketClient, "HTTP/1.1 404 Not found\r\n", 24);
    write(fdSocketClient, "Connection: close\r\n", 19);
    write(fdSocketClient, "Content-type: text/html\r\n", 25);
    write(fdSocketClient, "\r\n", 2); // Fin de l'entête
    write(fdSocketClient, "<html><head><title>Erreur 404 !</title><meta charset=\"UTF-8\"></head>", 67);
    write(fdSocketClient, "<body><p>Désolé, le fichier demandé n'a pas été trouvé : ", 63);
    char chaine[512];
    sprintf(chaine, "<tt>%s</tt> </body></html>\r\n", fichier);
    write(fdSocketClient, chaine, strlen(chaine));
}
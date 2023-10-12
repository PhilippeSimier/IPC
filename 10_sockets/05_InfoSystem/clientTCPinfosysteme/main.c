/* 
 * File:   main.c
 * Author: afavre, psimier
 *
 * Created on 11 octobre 2023, 15:48
 * Client infosystème
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char** argv) {
    int fd_sock;
    struct sockaddr_in infoServeur;
    int retour;
    int choix;
    int n = 0;
    int fin = 0;
    char buffer[1024];

    infoServeur.sin_family = AF_INET; //toujours la même chose
    infoServeur.sin_port = htons(5678); // dans l'ordre des octets du réseau
    infoServeur.sin_addr.s_addr = inet_addr(argv[1]); // dans l'ordre des octets du réseau

    while (!fin) {

        printf("Veuillez choisir une option :\n");
        printf("0. Obtenir la date du système\n");
        printf("1. Obtenir la charge du système\n");
        printf("2. Quitter\n");
        printf("Votre choix :");
        scanf("%d", &choix);

        if (choix == 2) {
            // Quitte le programme si le choix est 2
            fin = 1;
        } else {

            // Création du socket
            fd_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

            if (fd_sock == -1) {
                printf("probleme socket : %s\n", strerror(errno));
                exit(errno);
            }

            //connection
            retour = connect(fd_sock, (struct sockaddr *) &infoServeur, sizeof (infoServeur));
            if (retour == -1) {
                printf("probleme connect : %s\n", strerror(errno));
                exit(errno);
            }

            //sendto
            if (send(fd_sock, &choix, sizeof (choix), 0) == -1) {
                perror("Erreur lors de l'envoi du choix au serveur");
                exit(errno);
            }
            for (n = 0; n < 5; n++) {

                memset(buffer, 0, sizeof (buffer));
                // Réception des données du serveur
                ssize_t bytesRead = read(fd_sock, buffer, sizeof (buffer) - 1);
                if (bytesRead == -1) {
                    perror("Erreur lors de la réception des données du serveur");
                    break;
                } else if (bytesRead == 0) {
                    printf("Déconnexion du serveur\n");
                    break;
                }

                printf("Réponse du serveur : %s\n", buffer);


            }
            // Fermeture du socket
            close(fd_sock);
        }
    }

    return 0;
}
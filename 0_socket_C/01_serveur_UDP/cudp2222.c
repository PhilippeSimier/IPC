/***************************************************************************
 *   Client UDP interroge le serveur sur le port 2222		           *
 *   Envoie un entier au serveur udp et affiche la valeur retournée        *
 *   Il faut noter que la fonction select()  est utilisée ici pour gérer   *
 *   une temporisation d'attente de la réponse du serveur. Le protocole de *
 *   couche transport UDP n'est pas orienté connexion et n'offre aucun     *
 *   service de fiabilisation. Il incombe donc à la couche application     *
 *   d'assurer une forme de détection d'erreur. Dans notre cas, le client  *
 *   attend la réponse du traitement effectué par le serveur pendant       *
 *   deux secondes. Si aucune réponse n'a été reçue dans le temps imparti,   *
 *   il faut considérer que le message émis a été perdu.                   *
 *									   *
 *   Compilation  : gcc cudp2222.c -o cudp2222                             *
 *   Utilisation : ./cudp2222 127.0.0.1				           *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
	int fdSocket;
	int valeurEnv, valeurRet=0;
	struct sockaddr_in adresseServeur;
	struct sockaddr_in adresseServeurReponse;
	int retour,tailleReponse;
	fd_set readSet;
	struct timeval timeVal;

    if (argc != 2)
    {
	printf("Usage ./cudp2222 127.0.0.1\n");
	exit(0);
    }

    fdSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (fdSocket == -1)
    {
	printf("pb socket : %s\n", strerror(errno));
	exit(0);
    }

    adresseServeur.sin_family = AF_INET;

    adresseServeur.sin_port = htons(2222);	//numero de port du serveur dans l'ordre des octets du réseau
    adresseServeur.sin_addr.s_addr = inet_addr(argv[1]);	// adresse IP du serveur dans l'ordre des octets du reseau
    printf("Pour quitter entrer la valeur 0\n");
    printf("valeur a envoyer : ");
    scanf("%d",&valeurEnv);

    while(valeurEnv != 0){
	retour = sendto(fdSocket, &valeurEnv, sizeof(valeurEnv), 0, (struct sockaddr *)&adresseServeur, sizeof(adresseServeur));
	if (retour == -1)
	{
		printf("pb sendto : %s\n", strerror(errno));
		exit(0);
	}

        // Attente de la réponse pendant deux secondes.
	FD_ZERO(&readSet);
	FD_SET(fdSocket, &readSet);
	timeVal.tv_sec = 2;
	timeVal.tv_usec = 0;
	if (select(fdSocket+1, &readSet, NULL, NULL, &timeVal)) {
		retour = recvfrom(fdSocket, &valeurRet, sizeof(valeurRet), 0, (struct sockaddr *) &adresseServeurReponse, &tailleReponse);
		if (retour == -1)
		{
			printf("pb recvfrom : %s\n", strerror(errno));
			exit(0);
		}

		printf("le serveur a retourne : %d\n\n",valeurRet);
	}
	else {
		printf("Pas de réponse dans les deux secondes.\n");
	}
	printf("Autre valeur : ");
        scanf("%d",&valeurEnv);
    }
    close(fdSocket);
    printf(" Au revoir !!\n");
    return EXIT_SUCCESS;
}



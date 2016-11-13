/***************************************************************************
 *   Copyright (C) 2016 by philippe SIMIER  				   *
 *   philaure@wanadoo.fr  						   *
 *   Sujet : Mini serveur web itératif en C qui renvoie l'heure courante   *
 *           serveur minimaliste pour la mise en oeuvre des sockets en C   *
 *   Compilation :  gcc serveur_time.c -o serveur_time -Wall		   *
 *   Exécution   :  ./serveur_time					   *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>

/* fonction qui remplit le service  (renvoie l'heure au client) */
void service(int fdSocketClient, struct sockaddr_in adresseClient){

    char headHTTP[]="HTTP/1.1 200 OK\nContent-Length: 89\nContent-type: text/html\n\n";
    char buffer2[512];
    char pageDEB[]="<html><head><title>Mini Serveur Web C</title></head><body><h1>";
    char pageFIN[]="</h1></body></html>";
    char all[1024];
    time_t crt = time(NULL);
    char heure[20];
    int  retour;

    memset(buffer2, '\0', 512);
    memset(all, '\0', 1024);
    retour=read(fdSocketClient,buffer2,512);
        if (retour == -1)
        {
            printf("pb read : %s\n", strerror(errno));
            exit(0);
        }
    // TRAITEMENT   contruction de la réponse
    crt = time(NULL);
    strftime(heure, sizeof(heure), "%T", localtime(&crt));
    sprintf(all,"%s%s%s%s",headHTTP,pageDEB,heure,pageFIN);
    // WRITE  envoie de la réponse au client
    retour = write(fdSocketClient,all,strlen(all));
        if (retour == -1)
        {
            printf("pb write : %s\n", strerror(errno));
            exit(errno);
        }
    // CLOSE
    close(fdSocketClient);
    // Affichage de la réponse
    printf(">>  Requete du client %s/%d\n%s\n", inet_ntoa(adresseClient.sin_addr), adresseClient.sin_port, buffer2);
    printf(">>  Reponse du serveur : %d octets write\n",retour);
    printf(">>  %s\n\n",all);
}


int main(int argc, char *argv[])
{
    int fdSocketServeur;
    int fdSocketClient;

    struct sockaddr_in adresseServeur;
    struct sockaddr_in adresseClient;
    int retour;
    socklen_t tailleClient;
    int serverPort;

    fdSocketServeur = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // AF_INET désigne la famille de protocole de couche réseau IPv4.
    if (fdSocketServeur == -1)
	{
		printf("pb socket : %s\n", strerror(errno));
		exit(0);
	}

    do{
    	puts("Entrez le numéro de port utilisé en écoute (entre 1024 et 65535) : ");
    	scanf("%d", &serverPort);
    }
    while(serverPort < 1024 || serverPort > 65535);

    adresseServeur.sin_family = AF_INET;  // AF_INET désigne la famille de protocole de couche réseau IPv4.
    adresseServeur.sin_port = htons(serverPort);
    adresseServeur.sin_addr.s_addr = htonl(INADDR_ANY);
    retour = bind(fdSocketServeur, (struct sockaddr*)&adresseServeur, sizeof(adresseServeur));  //attachement ip-port
    if (retour == -1)
	{
		printf("pb bind : %s\n", strerror(errno));
		exit(0);
	}

    retour=listen(fdSocketServeur,10);	//10 clients maxi en file d'attente
    if (retour == -1)
	{
		printf("pb listen : %s\n", strerror(errno));
		exit(0);
	}
    tailleClient = sizeof(adresseClient);
    while(1){

        printf("Attente de requête sur le port %d\n", serverPort);
    	 // ACCEPT acceptation de la connexion client et creation de la socket de communication
        fdSocketClient=accept(fdSocketServeur,(struct sockaddr *) &adresseClient, &tailleClient);
	// appel de la fonction service pour construction de la réponse
        service( fdSocketClient, adresseClient);
    }

    return EXIT_SUCCESS;
}


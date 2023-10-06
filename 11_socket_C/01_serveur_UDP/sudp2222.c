/****************************************************
 * serveur UDP port 2222
 *  reçoit un entier et retourne son inverse
 * **************************************************/

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

	struct sockaddr_in adresseServeur;

	struct sockaddr_in adresseClient;
	int retour;
	int tailleClient;
	char buffer[255];
	int valRec;

	memset(buffer, '\0', 255);
	printf("serveur UDP sur port 2222 attend un ENTIER\n");
	fdSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (fdSocket == -1)
	{
		printf("pb socket : %s\n", strerror(errno));
		exit(0);
	}

	adresseServeur.sin_family = AF_INET;

	adresseServeur.sin_port = htons(2222);	//numero de port du serveur dans l'ordre des octets du réseau
	adresseServeur.sin_addr.s_addr = htonl(INADDR_ANY);	// adresse IP du serveur dans l'ordre des octets du reseau

	retour = bind(fdSocket, (struct sockaddr*) &adresseServeur, sizeof(adresseServeur));	//attachement ip-port

	if (retour == -1)
	{
		printf("pb sendto : %s\n", strerror(errno));
		exit(0);
	}

	tailleClient = sizeof(adresseClient);
	while (1){
	memset(buffer, '\0', 255);
	retour = recvfrom(fdSocket, &valRec, sizeof(valRec), 0, (struct sockaddr *) &adresseClient, &tailleClient);

	if (retour == -1)
	{
		printf("pb recvfrom : %s\n", strerror(errno));
		exit(0);
	}

	printf("message du client %s/%d: %d\n", inet_ntoa(adresseClient.sin_addr), adresseClient.sin_port, valRec);

	valRec=-valRec;
	retour = sendto(fdSocket, &valRec, sizeof(valRec), 0, (struct sockaddr *)&adresseClient, sizeof(adresseClient));

	if (retour == -1)
	{
		printf("pb sendto : %s\n", strerror(errno));
		exit(0);
	}
	}

	return EXIT_SUCCESS;
}

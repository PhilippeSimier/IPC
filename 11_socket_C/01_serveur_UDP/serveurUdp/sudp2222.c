/***************************************************************************
 *   Copyright (C) 2010 by dbernard   *
 *   dbernard@thunder   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/



#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
/*
struct sockaddr_in {
    sa_family_t    sin_family; // famille d'adresses : AF_INET
    uint16_t       sin_port;   // port dans l'ordre des
                                  octets réseau
    struct in_addr sin_addr;   // adresse Internet
};

// Adresse Internet
struct in_addr {
    uint32_t       s_addr;     // adresse dans l'ordre des
                                  octets réseau
};

*/

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
	while (1==1){
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

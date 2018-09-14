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
typedef struct{
	unsigned char jour;
	unsigned char mois;
	unsigned short int annee;
	char	jourDeLaSemaine[10];
}datePerso;

int main ( int argc, char *argv[] )
{
	int fdSocket,fdSocketClient;

	struct sockaddr_in adresseServeur;
	struct sockaddr_in adresseClient;

	int retour;
	int tailleClient;
	char buffer[255];
	int valeurEnv, valeurRet=0;
	datePerso dateClient;

	printf("serveur TCP sur port 7777 attend une structure de type datePerso\n");
	memset ( buffer, '\0', 255 );



	adresseServeur.sin_family = AF_INET;

	adresseServeur.sin_port = htons ( 7777 );	//numero de port du serveur dans l'ordre des octets du réseau
	adresseServeur.sin_addr.s_addr = INADDR_ANY;	// adresse IP du serveur dans l'ordre des octets du reseau
	//stresseur de serveur
	fdSocket = socket ( PF_INET, SOCK_STREAM, IPPROTO_TCP );

	if ( fdSocket == -1 )
	{
		printf ( "pb socket : %s\n", strerror ( errno ) );
		exit ( 0 );
	}
	retour = bind ( fdSocket, ( struct sockaddr* ) &adresseServeur, sizeof ( adresseServeur ) );	//attachement ip-port

	if ( retour == -1 )
	{
		printf ( "pb bind : %s\n", strerror ( errno ) );
		exit ( 0 );
	}

	retour=listen ( fdSocket,10 );	//je peux gerer 10 clients en file d'attente
	if ( retour == -1 )
	{
		printf ( "pb listen : %s\n", strerror ( errno ) );
		exit ( 0 );
	}

	tailleClient = sizeof ( adresseClient );


	while ( 1==1 )
	{
		fdSocketClient=accept ( fdSocket, ( struct sockaddr * ) &adresseClient, &tailleClient );	// acceptation de la connexion client et creation de la socket de communication

		if ( fdSocketClient == -1 )
		{
			printf ( "pb accept : %s\n", strerror ( errno ) );
			exit ( 0 );
		}

		retour = read ( fdSocketClient,&dateClient,sizeof ( dateClient ) );
		if ( retour == -1 )
		{
			printf ( "pb connect : %s\n", strerror ( errno ) );
			exit ( 0 );
		}
		printf("message du client %s/%d: \n", inet_ntoa(adresseClient.sin_addr), adresseClient.sin_port);
		printf("%s %d/%d/%d\n",dateClient.jourDeLaSemaine,dateClient.jour,dateClient.mois,dateClient.annee);

		

		close ( fdSocketClient );

		/*memset(buffer, '\0', 255);
		read(fdSocket,buffer,255);
		printf("reposne: %s\n",buffer);*/
	}


	return EXIT_SUCCESS;
}

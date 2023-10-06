/**********************************************************************
*  showip.c --
*  Affiche les adresses IP correspondant à un nom d'hôte donné en
*  ligne de commande
*
*  Compilation : gcc showip.c -o showip -Wall
*  Utilisation : ./showip localhost
*    	Adresse IP localhost:	            
*	 --> IPv6: ::1
*	 --> IPv4: 127.0.0.1
*
*  test	2	: ./showip ipv6.test-ipv6.ams.vr.org 
*	Adresses IP pour ipv6.test-ipv6.ams.vr.org:
*	 --> IPv6: 2607:f740:d::f77
*
*  test 3	: ./showip ds.test-ipv6.ams.vr.org
*	Adresses IP pour ds.test-ipv6.ams.vr.org:
*	 --> IPv4: 208.111.44.226
*	 --> IPv6: 2607:f740:d::f77
*
***********************************************************************/
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

int main (int argc, char *argv[]) {
    struct addrinfo hints; //  sert à positionner des options avantl'appel à getaddrinfo()
    struct addrinfo *res;  // pointeur pour adresser le premier résultat
    struct addrinfo *p;	   // pointeur pour parcourir la liste des enregistrements contenant les informations sur les adresses IP.	
    void *addr;
    int status;
    char ipstr[INET6_ADDRSTRLEN], ipver;

    if (argc != 2) {
 	fprintf(stderr, "usage: showip hostname\n");
	return 1;
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    // Les informations sur les adresses IPv4 et|ou IPv6
    hints.ai_socktype = SOCK_DGRAM; // Une seule famille de socket UDP datagramme
    hints.ai_protocol = 0;          // Tous les protocoles

    // argv[1]  correspond à la chaîne de caractères décrivant l'hôte dont on souhaite obtenir
    //          la liste des adresses réseau.	
    if ((status = getaddrinfo(argv[1], NULL, &hints, &res)) != 0) {
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
 	return 2;
    }

    printf("Adresses IP pour %s:\n\n", argv[1]);
    p = res;   // initialisation du pointeur p à la première adresse
    while (p != NULL) { //Si cette adresse vaut NULL, il n'y a plus d'enregistrement d'adresse dans la liste, on sort de la boucle.
 	// Identification de l'adresse courante
 	if (p->ai_family == AF_INET) { // si l'adresse est au format IPv4
 	    struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
 	    addr = &(ipv4->sin_addr);
 	    ipver = '4';
 	}
        else { // sinon c'est une adresse au format IPv6
 	    struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
 	    addr = &(ipv6->sin6_addr);
  	    ipver = '6';
 	}

    	// Conversion de l'adresse IP en une chaîne de caractères
    	inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    	printf(" --> IPv%c: %s\n", ipver, ipstr);
    	// On fait pointer  sur l'enregistrement d'adresse suivant.
    	p = p->ai_next;
    }
    // Libération de la mémoire occupée par les enregistrements
    freeaddrinfo(res);
 return 0;
}


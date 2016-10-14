/************************************************
 *	zone.h
 *	Le type des données de la
 *	file de message
 ************************************************/

#ifndef ZONE_H_INCLUDED
#define ZONE_H_INCLUDED

#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/msg.h>


// type de ce qui va etre partage
struct donnees{
	long type;
	char texte[9];
};

#endif // ZONE_H_INCLUDED

/************************************************/
/*	zone.h
 *	Le type des donnees de la
 *	file de message	
 ************************************************/


#ifndef ZONE_H
#define ZONE_H

// type de ce qui va etre partage
typedef struct {
	long type;
	char texte[255];
}donnees;

#endif


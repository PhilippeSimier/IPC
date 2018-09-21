/****************************************************
*  1) Donnez l'affichage du programme suivant:      *
*     Combien d'étoiles vont'elles s'afficher ?	    *
*     Compilation: gcc TD_fork1.c -o TD_fork1       *
*****************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   // Biblio pour programmation systeme (fork wait ...)

int main ( int argc, char *argv[] )
{
	fork();
	fork();
	fork();
	printf("* ");

	return EXIT_SUCCESS;
}

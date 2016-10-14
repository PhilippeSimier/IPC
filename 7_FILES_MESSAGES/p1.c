/*************************************************************
*	p1.c
*	met des messages de type 2(temp) et 4(press)
*	dans la file de message
*
*  Compilation : gcc p1.c -o p1 -Wall
*
*************************************************************/


#include "zone.h"

float randomF(){
	return ((float)100.0*(rand()/(RAND_MAX+0.1)));
}
unsigned int randomI(){
	return ((unsigned int)100.0*(rand()/(RAND_MAX+0.1)));
}

int main(int argc,char *argv[]){

	struct donnees maFile;
	int idFile;
	unsigned int val=0;
	float valF;

        // Obtention de la clé
	key_t key;
	if ((key = ftok("/tmp/bidon" , 5678 )) == -1){
		perror("ftok");
		exit(2);
	}
	/* obtention de la file pour la clé key */
	idFile = msgget( key , 0666|IPC_CREAT );
	if (idFile==-1){
		printf("pb creation file : %s\n",strerror(errno));
		exit(1);
	}

	while(1){

		valF=randomF();                    /* tirage au sort d'un float */

		sprintf(maFile.texte,"%f",valF);   /* conversion en chaine de caractères */
		maFile.type=2;                     /* message de type 2 */
		printf("type = %ld message = %s\n",maFile.type, maFile.texte);  /* affichage du message */
		msgsnd(idFile,(void*)&maFile,sizeof(maFile.texte),IPC_NOWAIT); /* envoi du message */


		val=randomI();                      /* tirage au sort d'un entier */
		sprintf(maFile.texte,"%d",val);   /* conversion en chaine de caractères */
		maFile.type=4;
		printf("type = %ld message = %s\n",maFile.type, maFile.texte);  /* affichage du message */
		msgsnd(idFile,(void*)&maFile,sizeof(maFile.texte),IPC_NOWAIT);    /* envoi du message */
		sleep(1);
	}

	return EXIT_SUCCESS;
}

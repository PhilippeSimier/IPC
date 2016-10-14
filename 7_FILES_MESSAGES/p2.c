/*****************************************
 *	p2.c
 *	met des messages de type 3(ordre)
 *	dans la file de message ayant pour clef
 *	5678
 * *****************************************/
#include "zone.h"

float randomC(){
	return ((char)('A'+((char)24.0*(rand()/(RAND_MAX+0.1)))));
}

int main(int argc,char *argv[]){

	struct donnees maFile;
	int idFile;

	idFile = msgget((key_t)5678,0666|IPC_CREAT);
	if (idFile==-1){
		printf("pb creation file : %s\n",strerror(errno));
		exit(0);
	}
	while(1){
		maFile.texte[0]=randomC();
		maFile.type=3;   /* affectation du type içi 3 */
		msgsnd(idFile,(void*)&maFile,sizeof(char),IPC_NOWAIT);   /* envoi du message dans la file */
		sleep(1);
	}

	return EXIT_SUCCESS;
}

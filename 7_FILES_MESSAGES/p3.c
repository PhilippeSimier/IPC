/*****************************************************************
 *	p3.c
 *	affiche les messages de type 2(temp), 3(ordre) et 4(press)
 *	de la file de message ayant pour clef 	5678
 * ***************************************************************/
#include "zone.h"


int main(int argc,char *argv[]){

	struct donnees maFile;
	int idFile;
	int ret;


    /* obtention de la file pour la clé 5678 */
	idFile = msgget((key_t)5678,0666|IPC_CREAT);
	if (idFile==-1){
		printf("pb creation file : %s\n",strerror(errno));
		exit(0);
	}

	while(1){
		memset(maFile.texte,'\0',9);  /* efface le text */
		ret=msgrcv(idFile,(void*)&maFile,9,2,IPC_NOWAIT);  /* retrait d'un message de type 2 */
		if (ret!=-1){
			printf("temp : %s\n",maFile.texte);
		}
		memset(maFile.texte,'\0',9);
		ret=msgrcv(idFile,(void*)&maFile,9,3,IPC_NOWAIT);  /* retrait d'un message de type 3 */
		if (ret!=-1){
			printf("ordre : %s\n",maFile.texte);
		}
		memset(maFile.texte,'\0',9);
		ret=msgrcv(idFile,(void*)&maFile,9,4,IPC_NOWAIT);  /* retrait d'un message de type 4 */
		if (ret!=-1){
			printf("press : %s\n",maFile.texte);
		}
	}

	return EXIT_SUCCESS;
}

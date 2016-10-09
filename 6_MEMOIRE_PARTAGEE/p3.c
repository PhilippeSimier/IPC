/************************************************
*   p3.c
*   Progamme affichant le contenu de la
*   zone memoire partagee
*   Compilation :    gcc p3.c -o p3 -Wall
* ************************************************/
#include "zone.h"

int main(){
    struct donnees *ZoneDePartage;
    int id;
    key_t key;

// Création de la clé
    if ((key = ftok("/tmp/bidon",'a')) == -1){
	perror("ftok");
	exit(2);
    }
// creation de la zone mémoire partagée avec autorisation R/W
    id = shmget( key, sizeof(struct donnees), IPC_CREAT | 0666 );
    if (id == -1){
	perror("shmget");
	exit(2);
    }
// attribution adresse virtuelle du segment

	ZoneDePartage = (struct donnees *)shmat(id,NULL,SHM_W); // attribution AV en R/W
	if (ZoneDePartage == NULL){
		perror("shmat");
		exit(1);
	}
// Lecture et affichage de la zone de memoire partagee
    while (1){	//boucle infinie
	printf("ordre : %c temp : %.2f press : %d\n",
	ZoneDePartage->ordre,
	ZoneDePartage->temp,
	ZoneDePartage->press);
	sleep(1);
	}
}

/************************************************
*   p2.c
*   Progamme mettant à jour le contenu de la
*   zone mémoire partagée
*   pour le champ ordre
*
*   gcc p2.c -o p2 -Wall
*
**************************************************/
#include "zone.h"

float randomC(){
    return ((char)('A'+((char)(25.0*(rand()/(RAND_MAX+0.1))))));
}

int main(){
    struct donnees *ZoneDePartage;
    int id;
    key_t key;

// Création de la clé
    if ((key = ftok("/tmp/bidon", 1234 )) == -1){
        perror("ftok");
        exit(2);
    }

// création de la zone mémoire partagée
	id = shmget( key, sizeof(struct donnees), IPC_CREAT | 0666); //autorisation de R/W
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
// Ecriture en continue dans le champ ordre
    while (1){	//boucle infinie
	ZoneDePartage->ordre = randomC();
	printf ("ordre : %c\n", ZoneDePartage->ordre);
	sleep(1);
    }
}

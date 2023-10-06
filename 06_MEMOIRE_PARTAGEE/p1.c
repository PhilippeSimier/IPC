/******************************************************
*	Correction du TD 6 mémoire partagée   p1.c
*	Progamme mettant a jour le contenu
*	de la zone mémoire partagée
*	pour temp et press
*
*       Compilation : gcc p1.c -o p1 -Wall
*******************************************************/
#include "zone.h"

float randomF()
{
    return ((float)100.0*(rand()/(RAND_MAX+0.1)));
}
int randomI()
{
    return ((int)100.0*(rand()/(RAND_MAX+0.1)));
}

int main()
{
    struct donnees *ZoneDePartage;
    int id;
    key_t key;

// Création de la clé
    if ((key = ftok("/tmp/bidon" , 1234 )) == -1){
        perror("ftok");
        exit(2);
    }

// création de la zone mémoire partagée autorisation de R/W
    id = shmget( key, sizeof(struct donnees), IPC_CREAT | 0600 );
    if (id == -1)
    {
        if (errno!=EEXIST)
        {
            printf("pb shmget : %s\n",strerror(errno));
            exit(2);
        }

    }
// attribution adresse virtuelle du segment

    ZoneDePartage = (struct donnees *)shmat(id,NULL,SHM_W); // attribution AV en R/W
    if (ZoneDePartage == NULL)
    {
        perror("shmat");
        exit(1);
    }
// Ecriture en continu de valeurs aléatoires
// dans la zone de memoire partagée
    while (1) 	//boucle infinie
    {
        ZoneDePartage->temp=randomF();
        ZoneDePartage->press=randomI();
	printf(" temp : %.2f press : %d\n",
		ZoneDePartage->temp,
		ZoneDePartage->press);

	sleep(1);
    }
}


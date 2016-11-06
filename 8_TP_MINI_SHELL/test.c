/****************************************************
*   programme test pour montrer le fct des signaux
*   sous linux aux étudiants
*   Auteur : Philippe SIMIER
*****************************************************/
#include <stdio.h>
#include <stdlib.h>


int main()
{
    int b,k;
    char buffer[10];

    printf("Programme test\n");
    printf("   0 : return 0 \n");
    printf("   1 : return 1 \n");
    printf("   2 : abondon  \n");
    printf("   3 : division par zéro\n");
    printf("   4 : Violation mémoire\n");
    printf("   5 : Expiration de délai (1s)\n");
    printf("   6 : S'envoyer un signal USR1\n");
    printf("   7 : S'envoyer un signal USR2\n");
    printf(" Votre choix :");
    scanf("%d",&b);


    switch(b)
    {
        case 0:
           return 0;  // terminaison normale avec status = 0
	case 1:
	   return 1;  //  Terminaison normale avec status = 1
	case 2:
	   abort();   // pour provoquer une terminaison par abandon
	case 3:
           b = 100/0;  // division pa zéro
        case 4:
           for(k=0; ; k++){
		buffer[k] = 'A';
           }          //provoquer une violation de la segmentation mémoire
	case 5:
	    alarm(1); // lancement d'un timer pour 1s
	    pause();  // attente on fait rien
	    break;
        case 6:
	    kill(getpid(),10); //  Permet de s'envoyer un signal USR1
        case 7:
            kill(getpid(),12); //  Permet de s'envoyer un signal USR2


    }
}

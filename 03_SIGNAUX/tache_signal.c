/************************************************************************
*   Ce programme consiste à faire faire une tâche à un programme	*
*   lors de la réception d'un signal. Nous redirigeons SIGINT de	*
*   la même façon qu'avant pour terminer correctement le programme,	*
*   permettant de faire du nettoyage (s'il le faut) et ensuite		*
*   le signal SIGUSR1 pour afficher un message lors de sa réception.	*
*									*
*   Pour envoyer un signal avec le shell     kill -SIGUSR1 pid          *
*									*
*************************************************************************/
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* 
 * Volatile permet de dire au compilateur de ne pas optimiser le code
 * lorsque la variable continuer est en jeu.
 * Cela force la verification de la valeur de cette variable dans la boucle
 * de la fonction main
 */ 
volatile int continuer = 1;

void gestion_signal(int signum)
{
    /* Si c'est SIGINT, on met continuer a 0 */
    if(signum == SIGINT) {
        continuer = 0;
    }
    else {
        /* Sinon si c'est SIGUSR1, on affiche bonjour */
        if(signum == SIGUSR1) {
            printf("Hello World !!!\n");
        }
    }
}

int main(void)
{
    struct sigaction act;

    /* Affichage du pid */
    printf("Le programme commence : %d\n", getpid());

    /* Prepare la structure sigaction */
    memset(&act,0,sizeof(act));
    act.sa_handler = gestion_signal;

    if(sigaction(SIGINT,&act,NULL)==-1) {
        fprintf(stderr,"Erreur avec sigaction\n");
        exit(EXIT_FAILURE);
    }

    if(sigaction(SIGUSR1,&act,NULL)==-1) {
        fprintf(stderr,"Erreur avec sigaction\n");
        exit(EXIT_FAILURE);
    }

    /* La boucle generale */
    while(continuer) {
        /* Dormir pour une journee en attente d'un signal */
        sleep(86400);
    }

    /* Un affichage de fin de programme */
    printf("Le programme se termine\n");
    return EXIT_SUCCESS;
}

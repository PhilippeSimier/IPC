/****************************************************************
*   IPC Snir2  TD sur les signaux				*
*   Etude des signaux  Exo n°2 question 3 4 5                   *
*   Compilation : gcc TD_signal2_3.c -o TD_signal2_3 -Wall      *
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


// handler affichage
void affichage(int sig){

    static int cpt = 0;

    cpt++;
    printf("%d : signal %d recu par le processus %d\n", cpt, sig, getpid());
}

int main ( int argc, char *argv[] )
{
	int pid1;
	int pid2;
	int pid3;
	int retour;

	pid1 = getpid();
	(void) signal(SIGUSR1, affichage); // redirection des signaux SIGUSR1 vers la fonction affichage
	printf ("Père   p1 pid = %d\n", pid1);
	pid2 = fork();
	if ( pid2 == 0 )
	{       // Enfant P2
		pid3 = fork();
		if (pid3 == 0){
			// Enfant P3
			printf ( "Enfant p3 pid = %d ppid = %d\n", getpid(), getppid());
			sleep(1);
			printf ("P3 envoie un signal USR1 à P1\n");
			retour = kill(pid1, SIGUSR1);
			if (retour !=0) {
				printf("Erreur avec kill");
			}
                        printf("Le processus P3 attend un signal\n");
			pause(); // attente d'un signal
			printf ("Fin de processus  P3\n");
                }
		else{
			// Enfant P2
			printf ( "Enfant p2 pid = %d ppid = %d\n", getpid(), getppid());
                        sleep(1);
			printf ("P2 envoie un signal USR1 à P1\n");
			retour = kill(pid1, SIGUSR1); // envoi d'un signal USR1 au père P1
			printf ("P1 envoie un signal USR1 à P3\n");
                        retour = kill(pid3, SIGUSR1); // envoi d'un signal USR1 a enfant P3
			if (retour !=0) {
                                printf("Erreur avec kill");
                        }
			printf("Le processus P2 attend un signal\n");
			pause();
			printf ("Fin de processus P2\n");
                }
	}
	else
	{	// Père P1
		sleep(3);
                printf ("P1 envoie un signal USR1 à P2\n");
		retour = kill(pid2, SIGUSR1); // envoi d'un signal USR1 a enfant 2
                printf("Le processus P1 attend un signal\n");
		pause();
		printf ("Fin de processus P1\n");
	}
	return EXIT_SUCCESS;
}

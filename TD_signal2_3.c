/****************************************************************
*   IPC Snir2  Signaux  Exo n°2 question 3                      *
*   Compilation : gcc TD_signal2_3.c -o TD_signal2_3 -Wall      *
*****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


// fonction pour l'affichage du signal SIGUSR1
void affichage(int sig)
{   static int cpt = 0;
    cpt++;
    printf("%d :Signal %d\n", cpt, sig);
    if (cpt == 2){
        printf("deux signaux SIGUSR1 reçu par pid :%d\n", getpid());
    }
}

int main ( int argc, char *argv[] )
{
	int pid1,pid2,pid3;
	int statut,retour;

	pid1 = getpid();
	printf ("Père   p1 pid = %d\n", pid1);
	pid2 = fork();
	if ( pid2 == 0 )
	{       // Enfant P2
		pid3 = fork();
		if (pid3 == 0){
			// Enfant P3
			printf ( "Enfant p3 pid = %d ppid = %d\n", getpid(), getppid());
			sleep(1);
			retour = kill(pid1, SIGUSR1); // envoi d'un signal USR1 au père P1
			if (retour !=0) {
				printf("Erreur avec kill");
			}
			sleep(1);
			printf ("Fin de Enfant 3\n");
                }
		else{
			// Enfant P2
			printf ( "Enfant p2 pid = %d ppid = %d\n", getpid(), getppid());
			retour = kill(pid1, SIGUSR1); // envoi d'un signal USR1 au père P1
                        if (retour !=0) {
                                printf("Erreur avec kill");
                        }
			wait(&statut);
			sleep(1);
			printf ("Fin de Enfant 2\n");
                }
	}
	else
	{	// Père P1
		(void) signal(SIGUSR1, affichage); // redirection des signaux SIGUSR1 vers la fonction affichage
		wait(&statut);
		printf ("Fin de père\n");
	}
	return EXIT_SUCCESS;
}

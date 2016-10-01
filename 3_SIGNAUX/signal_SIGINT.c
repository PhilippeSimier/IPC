/*********************************************************************
*  Programmation des signaux
*
*  Ecrivez un programme dans lequel un processus ouvre un fichier
*  sur lequel il travaille (il écrit 10 fois la chaîne "abcdef...xyz"
*  dans le fichier. En cas de réception du signal SIGINT le processus
*  ferme le fichier avant de se terminer.
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>

int desc;  //une variable globale

void onIntr()
{
    printf("\nInterruption avec  CTRL C\n");
    close(desc);
    exit(1);
}

int main ( int argc, char *argv[] )
{
    int i = 0;

    // ouverture du fichier test
    // et création s'il n'existe pas.
    if ((desc = open("test", O_RDWR | O_CREAT, S_IWUSR | S_IRUSR)) < 0)
    {
	printf("probleme avec l'ouverture du fichier test !\n");
	return 1;
    }
    (void) signal(SIGINT, onIntr);  // redirection des signaux SIGINT vers onInt
    while (i < 10){
	printf(" %d\n",i);
        write (desc, "abcdefghijklmnopqrstuvwxyz\n", 27);
	i = i + 1;
	sleep(1);
    }
    printf("Fin du processus normal\n");
    close (desc);
    return 0;
}

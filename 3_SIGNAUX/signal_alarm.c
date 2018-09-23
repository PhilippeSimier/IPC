/*****************************************************************
*  Ecrivez un programme dans lequel un processus crée un fils
*  puis se met en attente de la fin de son fils.
*  Le fils éxécute un code qui boucle.
*  Au bout de 10 secondes le fils n'étant pas achevé, le père
*  tue son fils. C'est horrible !!!
*
*  Compilation : gcc signal_alarm.c -o signal_alarm -Wall
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t pid;  // variable globale

void onAlarm()
{
    printf("Attente terminée \n");
    printf("Envoie de SIGKILL au processus %d\n", pid);
    kill (pid, SIGKILL);
}

int main()
{
    int status;
    pid_t pid_fin;

    pid = fork();
    if (pid == -1) {
	printf("Probleme avec fork");
    }
    else
    {
	if (pid == 0)
        { // Processus fils
	    printf(" - Je suis le processus fils pid : %d\n", getpid());
	    for (;;)
		{
		printf(" - Je boucle !\n");
		sleep(1);
		}
	}
        else
	{
	    printf("Je suis le processus père pid : %d\n", getpid());
	    (void) signal (SIGALRM, onAlarm); //le handler onAlarm est attaché au signal alarm
	    alarm(10);  // alarm dans 10 secondes
	    pid_fin = wait (&status); // attente de la fin du fils
            printf("Fin du processus %d avec le status : %d\n", pid_fin, status);
	}
    }
    return 0;
}

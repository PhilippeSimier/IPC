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

void onalarm()
{
    printf("Handler onalarm\n");
    kill (pid, SIGKILL);
    exit (1);
}

int main()
{
    int status;

    pid = fork();
    if (pid == -1) {
	printf("Probleme avec fork");
    }
    else
    {
	if (pid == 0) 
        { // Processus fils
	    printf("Je suis le fils !\n");
	    for (;;)
		{
		printf(" Je boucle !\n");
		sleep(1);
		}
	}
        else
	{
	    printf(" Je suis le père\n");
	    (void) signal (SIGALRM, onalarm); //le handler onalarm est attaché au signal alarm
	    alarm(10);  // alarm dans 10 secondes
	    wait (&status);
	}
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main ()
{
	int pid0, pid1;
	int status;

	pid1 = fork();
	if ( pid1 == 0 )
	{       // Enfant P1
		printf ( "je suis le fils, mon pid est %d \n", getpid());
		printf ( "pid de mon père, %d\n", getppid());
		sleep(2);
		exit(1);
	}
	else
	{       // Père
	    printf ( "je suis le père, mon pid est %d \n", getpid());
		printf ( "pid de mon fils, %d\n", pid1);
		int fils_mort = wait(&status);
		printf ( "Le code de retour de mon fils est %d\n", status);
	}
}

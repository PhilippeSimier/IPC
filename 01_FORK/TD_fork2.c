/******************************************************************
*    2) Dessinez l'arbre des processus généré			  *
*       par le programme suivant:				  *
*    Compilation : gcc TD_fork2.c -o TD_fork2			  *
*******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ( int argc, char *argv[] )
{
	int pid1,pid2,pid3,pid4;
	pid4 = getpid();
	pid1 = fork();
	if ( pid1 == 0 )
	{       // Enfant P1
		printf ( "p1 pid=%d ppid=%d \n",getpid(),getppid());
	}
	else
	{       // Père
		pid2 = fork();
		if ( pid2 == 0 )
		{       // Enfant P2
			printf ( "p2 pid=%d ppid=%d \n",getpid(),getppid());
		}
		else
		{       // Père
			pid3 = fork();
			if ( pid3 == 0 )
			{       // Enfant P3
				printf ( "p3 pid=%d ppid=%d \n",getpid(),getppid());
			}
			else
			{       // Père P4
				printf ( "p4 pid=%d\n",pid4);
			}
		}
	}
	return EXIT_SUCCESS;
}

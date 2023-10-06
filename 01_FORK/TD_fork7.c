/**********************************************************
   exo 7 du TD1
   compilation gcc TD_fork7.c -o TD_fork7
**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main ()
{
    int pid1,pid2,pid3,pid4,pid5,pid6,pid7;
    int pid;

    pid1 = getpid();
    pid3 = fork();
    if (pid3 == 0) // enfant de P1 nommé P3
    {
        pid2 = fork();
        if (pid2 == 0) // enfant de P3 nommé P2
        {
		printf ( "p2 : pid=%d ppid=%d \n",getpid(),getppid());
                sleep(5);
        }
        else {   // on est toujours dans p3
            pid4 = fork();
            if (pid4 == 0) // enfant de P3 nommé P4
               {
                   printf ( "p4 : pid=%d ppid=%d \n",getpid(),getppid());
                   sleep(3);
               }
               else {  // on est toujours dans P3
                   pid5 = fork();
		   if (pid5 == 0) // enfant de P3 nommé P5
        	   {
		       pid6 = fork();
                       if (pid6 == 0) // enfant de P5 nommé P6
                       {
                           printf ( "p6 : pid=%d ppid=%d \n",getpid(),getppid());
                           sleep(2);
                       }
                       else // On est dans P5
                       {
			  pid7 = fork();
                          if(pid7 == 0) // enfant de P5 nommé P7
                          {
			       printf ( "p7 : pid=%d ppid=%d \n",getpid(),getppid());
                               sleep(1);
                          }
                          else{  // on est dans P5
                              printf ( "p5 : pid=%d ppid=%d \n",getpid(),getppid());
                              pid = wait(NULL); // attente du premier fils
                              printf ( "dans P5 fin du fils : %d \n", pid);
                              pid = wait(NULL);  // attente du second fils
                              printf ( "dans P5 fin du fils : %d \n", pid);
                          }
                       }
        	   }
                   else { // on est toujours dans P3 et on attend la fin d'exécution des fils
			printf ( "p3 : pid=%d ppid=%d \n",getpid(),getppid());
                        pid = wait(NULL);
                        printf ( "dans P3 fin du fils : %d \n", pid);
                        pid = wait(NULL);
                        printf ( "dans P3 fin du fils : %d \n", pid);
                        pid = wait(NULL);
                        printf ( "dans P3 fin du fils : %d \n", pid);
                   }
               }
            }

    }
    else
    { // père P1
        printf ( "p1 : pid=%d ppid=%d \n", pid1, getppid());
        pid = wait(NULL);
        printf ( "dans P1 fin du fils : %d \n", pid);
    }

    return EXIT_SUCCESS;
}

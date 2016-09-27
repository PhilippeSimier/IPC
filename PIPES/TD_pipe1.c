/************************************************************
*    TD n°4   PIPE                                          *
* 1) En utilisant popen afficher le résultat donné par le   *
*    programme externe "/bin/uname"                         *
*    Compilation:  gcc TD_pipe1.c -o TD_pipe1               *
************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main()
{
   FILE *pp;
   char buffer[255];
   int nbRead;

   pp = popen("/bin/uname -a", "r");
   if ( pp== NULL)
      {
           printf("Pb avec popen : %s",strerror(errno));
           exit(0);
      }

    /*while (fgets(buffer, sizeof(buffer), pp)){
            fputs(buffer, stdout);
    }*/

    nbRead = fread(buffer, sizeof(buffer),1,pp);
    printf("%s",buffer);


   pclose(pp);
   return 0;
}



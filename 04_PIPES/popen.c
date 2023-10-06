/*   Question 1  Popen
     Compilation gcc popen.c -o popen
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main()
{
   FILE *fd;
   char buffer[1024];
   int nbRead;

   memset(buffer,0,1024);

   fd = popen("/bin/uname", "r");
   if ( fd == NULL)
      {
           printf("Pb avec popen : %s",strerror(errno));
           exit(0);
      }

    nbRead = fread(buffer, sizeof(buffer),1,fd);

    printf("%s",buffer);


   pclose(fd);
   return 0;
}

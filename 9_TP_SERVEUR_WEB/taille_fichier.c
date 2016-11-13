/***********************************************************************
   Programme pour connaître la taille d'un fichier avant de l'ouvrir
***********************************************************************/

       #include <sys/types.h>
       #include <sys/stat.h>
       #include <time.h>
       #include <stdio.h>
       #include <stdlib.h>

       int
       main(int argc, char *argv[])
       {
           struct stat sb;

           if (argc != 2) {
               fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           if (stat(argv[1], &sb) == -1) {

               perror("stat");
               exit(EXIT_FAILURE);
           }
           printf("File size: %d bytes\n",(int) sb.st_size);

           exit(EXIT_SUCCESS);
       }

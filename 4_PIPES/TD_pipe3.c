/************************************************************
*    TD n°4   PIPE                                          *
* 3) Question
* Soit le processus P1 créant un fils P2                    *
* Ecrivez un programme où P1 écrit un message dans un tube  *
* (pipe) à destination de P2                                *
* en plus, lorsque P2 a lu et affiché le message, il écrit  *
* à P1 un autre message dans un autre tube                  *
************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pid1,pid2;
    int status;
    int descTube[2];
    int descRep[2];
    const char chaine[] = "Hello this is your father !!\n";
    const char reponse[] = "Nice to meet you !!\n";
    char buffer[255];
    int nbOctets = 0;

    pid1 = getpid(); //pid1 le pid du père
    memset(buffer, '\0', 255); // on efface le buffer

    if (( pipe(descTube) == 0) & ( pipe(descRep) == 0)){
        pid2 = fork();


        if (pid2 == 0){
            // fils P2
            printf("  >> I'm the sun\n");

            // le fils s'endort pour deux secondes
            sleep(2);
            // puis lit le message à l'entrée du tube
            nbOctets = read(descTube[0], buffer, 255);
            printf("  >> Octets lus: %d: %s\n", nbOctets, buffer);
            nbOctets = write(descRep[1], reponse, strlen(reponse));
            printf("  >> %d octets écrits\n", nbOctets);

        }
        else {
            printf("I'm the father\n");
            // ecriture du message chaine dans le tube
            nbOctets = write(descTube[1], chaine, strlen(chaine));
            printf("%d octets écrits\n", nbOctets);
            nbOctets = read(descRep[0], buffer, 255);
            printf("Octets lus: %d: %s\n", nbOctets, buffer);
            //le père attend la fin d'éxécution du fils
            wait (&status);
        }
    }
    return 0;
}

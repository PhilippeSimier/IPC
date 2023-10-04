/*****************************************
 *	p3.c
 *	affiche les messages de type 2(temp), 3(ordre) et 4(press)
 *	dans la file de message ayant pour clef 5678
 *	
 * *****************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "zone.h"

int main() {

    donnees maFile;

    int idFile;
    int ret;

    // creation de la file 
    idFile = msgget((key_t) 5678, IPC_CREAT | 0666);
    if (idFile == -1) {
        printf("pb creation file : %s\n", strerror(errno));
        exit(1);
    }

    // boucle infinie
    while (1) {
        // reception des messages de type 2 à 4

        for (long i = 2; i < 5; i++) {

            ret = msgrcv(idFile, (void*) &maFile, sizeof (maFile.texte), i, IPC_NOWAIT);
            if (ret != -1) {
                printf("%s type : %ld\n", maFile.texte, maFile.type);
            }

        }
    }

    exit(EXIT_SUCCESS);
}
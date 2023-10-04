/*****************************************
 *	p1.c
 *	met des messages de type 2(temp) et 4(press)
 *	dans la file de messages ayant pour clef
 *	5678
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

float randomF();
int randomI();
char randomC();

int main() {

    donnees maFile;
    int idFile;
    
    // creation de la file
    idFile = msgget( (key_t) 5678, IPC_CREAT | 0666);
    if (idFile == -1) {
        printf("pb creation file : %s\n", strerror(errno));
        exit(1);
    }

    // boucle infinie pour envoyer des temperatures et des pressions
    while (1) {


        maFile.type = 2;
        snprintf(maFile.texte, sizeof(maFile.texte), "Temperature : %.2f", randomF());
        msgsnd(idFile, (void*) &maFile, sizeof(maFile.texte), IPC_NOWAIT);
        sleep(1);

  
        maFile.type = 4;
        snprintf(maFile.texte, sizeof(maFile.texte), "Pression : %d", randomI());
        msgsnd(idFile, (void*) &maFile, sizeof(maFile.texte), IPC_NOWAIT);
        sleep(2);
    }

    exit(EXIT_SUCCESS);
}

float randomF() {
    return ((float) 100.0 * (rand() / (RAND_MAX + 0.1)));
}

int randomI() {
    return ((int) 100.0 * (rand() / (RAND_MAX + 0.1)));
}

char randomC() {
    char tabCar[3] = {'a', 'b', 'c'};
    int index = 0;
    index = randomI() % 3;
    return tabCar[index];
}


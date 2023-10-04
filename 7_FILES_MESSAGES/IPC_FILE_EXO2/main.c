/*****************************************
 *	p2.c
 *	met des messages de type 3(ordre)
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
    int ret;
    
    // creation de la file
    idFile = msgget( (key_t) 5678, IPC_CREAT | 0666);
    if (idFile == -1) {
        printf("pb creation file : %s\n", strerror(errno));
        exit(1);
    }

    // boucle infinie pour envoyer des ordres (lettre comprise entre a et c)
    while (1) {


        maFile.type = 3;
        snprintf(maFile.texte, sizeof(maFile.texte), "Ordre : %c", randomC());
        ret = msgsnd(idFile, (void*) &maFile, sizeof(maFile.texte), IPC_NOWAIT);
        if (ret == -1){
            printf("pb msgsnd : %s\n", strerror(errno));
        }
        sleep(1);

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


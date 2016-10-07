/***********************************************************************************************
*
*     Corrigé du td n°5  élève question 3 et 4
*
<<<<<<< HEAD
*     Un mutex est un objet d'exclusion mutuelle (MUTual EXclusion), et est très pratique
*     pour protéger des données partagées
*
*     Q3 Modifiez le comportement des threads afin que chacun mette à jour les 3 champs
=======
*     Q3 Modifiez le comportement des threads afin que chacun met à jour les 3 champs
>>>>>>> 2667b096a81d63d8257b5be9eb1742cc3e2fedc9
*     de la structure.
*
*     Q4 Modifiez votre programme pour que la validité des données soit assurée (mutex)
*
*     Compilation: gcc TD_thread1_suite.c -o TD_thread1_suite -l pthread -std=c99
*
************************************************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/syscall.h>



typedef struct {
    int v1;
    int v2;
    int v3;
}laStruct;

laStruct        global;    /** variable globale partagée **/
pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER; /** Création du mutex et initialisation**/

void *tache1(void *p_data)
{

    laStruct val = *((laStruct *) p_data);
    int tid;
    int j = 1;

    for (int i=0 ; i< 20000; i++){     /** boucle pour retarder la tache1 **/
        j *= 2;
    }


    pthread_mutex_lock(&verrou); /** On verrouille le mutex **/
    global = val;
    pthread_mutex_unlock (&verrou); /** On déverrouille le mutex **/

    tid = syscall(SYS_gettid);
    printf("je suis T1 TID = %d v1 = %d,\n", tid, global.v1);
    printf("dans T1 l'argument vaut : %d\n", val.v1);
    sleep(3);

    pthread_exit((void *)"Job terminé pour T1 \n");
}

void *tache2(void *p_data)
{


    laStruct val = *((laStruct *) p_data);
    int tid;
    int j = 0;

    for (int i=0 ; i< 10000; i++){     /** boucle pour retarder la tache2 **/
        j *= 2;
    }



    pthread_mutex_lock(&verrou); /** On verrouille le mutex **/
    global = val;
    pthread_mutex_unlock (&verrou); /** On déverrouille le mutex **/

    tid = syscall(SYS_gettid);

    printf("je suis T2 TID = %d v2 = %d,\n", tid, global.v2);
    printf("dans T2 l'argument vaut : %d\n", val.v2);
    sleep(1);
    pthread_exit((void *)"job terminé pour T2 \n");
}

void *tache3(void *p_data)
{


    laStruct val = *((laStruct *) p_data);
    int tid;

    pthread_mutex_lock(&verrou); /** On verrouille le mutex **/
    global = val;
    pthread_mutex_unlock (&verrou); /** On déverrouille le mutex **/

    tid = syscall(SYS_gettid);
    printf("je suis T3 TID = %d v3 = %d,\n", tid, global.v3);
    printf("dans T3 l'argument vaut : %d\n", val.v3);
    sleep(2);

    pthread_exit((void *)"Job terminé pour T3 \n");
}

int main()
{
    int i,res;
    pthread_t T[3];


    void *thread_result;



    laStruct a = {11,12,13};
    laStruct b = {21,22,23};
    laStruct c = {31,32,33};

    /** Création des trois threads  **/


    res = pthread_create(&T[0], NULL, tache1, (void *)&a);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&T[1], NULL, tache2, (void *)&b);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&T[2], NULL, tache3, (void *)&c);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    /** attente de la fin des trois threads **/

    printf("Attente de la fin des thread...\n");

    for ( i = 0; i < 3; i++){
        res = pthread_join(T[i], &thread_result);
        if (res != 0) {
            perror("Thread join a échoué");
            exit(EXIT_FAILURE);
        }
        printf("OK, valeur de retour du thread join : %s\n", (char *)thread_result);
    }

    /** affichage de la variable globale **/

    printf(" V1 = %d\n V2 = %d\n V3 = %d\n", global.v1, global.v2, global.v3);

    pthread_mutex_destroy(&verrou);

    exit(EXIT_SUCCESS);
}

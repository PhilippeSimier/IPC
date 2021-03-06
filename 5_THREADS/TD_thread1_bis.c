/****************************************************************************
*  Corrigé version 2 sans les (void *) du td n°5 élève question 1 et 2
*
*  Q1   Ecrivez un programme qui va créer trois threads
*  chacun mettra à jour  un champ d'une variable de laStruct
*  Le processus principal attendra la fin des 3 threads et affichera
*  la valeurs des champs de la structure.
*  Le processus principal attendra la fin des 3 threads et affichera
*  les valeurs des champs de la structure.
*  Chaque thread affichera son identifiant, et le nom du champs de la structure
*  avec une valeur qu'il affecte à ce champ.
*
*  Q2   Modifié le code des threads afin que chacun mettre à jour l'un des
*  champs de la structure.
*
*  Compilation :  gcc TD_thread1_bis.c -o TD_thread1_bis -l pthread
*  Auteur      :  Philippe Simier
*
****************************************************************************/
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

laStruct global;

char *tache1(int p_data)
{
    global.v1 = p_data;
    int tid = syscall(SYS_gettid);

    printf("je suis T1 TID = %d v1 = %d,\n", tid, global.v1);
    printf("dans T1 l'argument vaut : %d\n", p_data);
    sleep(3);

    pthread_exit("Job terminé pour T1 \n");
}

char *tache2(int p_data)
{

    global.v2 = p_data;
    int tid = syscall(SYS_gettid);

    printf("je suis T2 TID = %d v2 = %d,\n", tid, global.v2);
    printf("dans T2 l'argument vaut : %d\n", p_data);
    sleep(3);
    pthread_exit("job terminé pour T2 \n");
}

char *tache3(int p_data)
{
    global.v3 = p_data;
    int tid = syscall(SYS_gettid);

    printf("je suis T3 TID = %d v3 = %d,\n", tid, global.v3);
    printf("dans T3 l'argument vaut : %d\n", p_data);
    sleep(2);

    pthread_exit("Job terminé pour T3 \n");
}

int main()
{
    int i,res;
    pthread_t T[3];
    char *thread_result;

    int a = 10;
    int b = 20;
    int c = 30;

    global.v1 = 0;
    global.v2 = 0;
    global.v3 = 0;

    /** Création des trois threads  **/


    res = pthread_create(&T[0], NULL, tache1, a);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&T[1], NULL, tache2, b);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }

    res = pthread_create(&T[2], NULL, tache3, c);
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
        printf("OK, valeur de retour du thread join : %s\n", thread_result);
    }

    printf(" V1 = %d\n V2 = %d\n V3 = %d\n", global.v1, global.v2, global.v3);

    exit(EXIT_SUCCESS);
}


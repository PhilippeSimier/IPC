/****************************************************************************
*  Programme minimaliste de démonstration du fct
*  des threads.
*  Le thread s'endort pendant 3s puis modifie la variable buffer.
*  Le processus principal attends la fin du thread
*
*  Compilation :  gcc thread_demo.c -o thread_demo -l pthread
*
****************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

char buffer[255]="Debian inside";    // Zone commune

void *ma_fonction_thread(void *arg)
{
    printf("dans le thread l'argument était : %s\n", (char *)arg);
    sleep(3);
    strcpy(buffer, "Nouveau message : Bye! bye");
    pthread_exit((void *)"Merci pour le temps CPU \n");
}

int main()
{
    int res;
    pthread_t un_thread;
    void *thread_result;

    // Création du thread
    res = pthread_create(&un_thread, NULL, ma_fonction_thread, (void *)buffer);
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Attente de la fin du thread...\n");

    // pthread_join
    res = pthread_join(un_thread, &thread_result);
    if (res != 0) {
        perror("Thread join a échoué");
        exit(EXIT_FAILURE);
    }
    printf("OK, valeur de retour du thread join : %s\n", (char *)thread_result);
    printf("Le buffer contient maintenant %s\n", buffer);
    exit(EXIT_SUCCESS);
}

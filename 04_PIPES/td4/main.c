#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


// handler affichage

void affichage(int sig) {

    static int cpt = 0;
    cpt++;
    printf("cpt : %d : signal %d recu par le processus %d\n", cpt, sig, getpid());
}

int main(int argc, char *argv[]) {
    int pid1;
    int pid2;
    int pid3;
    int retour, status, nb;
    int pipefd[2];
    char message[255];
    char buffer[255];

    pid1 = getpid();
    // redirection des signaux SIGUSR1 vers la fonction affichage
    signal(SIGUSR1, affichage);

    printf("Père   p1 pid = %d\n", pid1);
    status = pipe(pipefd); // Creation du tube
    if (status != 0) {
        printf("Probleme de création du tube\n");
        exit(1);
    }

    pid2 = fork();
    if (pid2 == 0) { // dans l'enfant P2
        pid3 = fork();
        if (pid3 == 0) {
            // dans l' enfant P3
            printf("Enfant p3 pid = %d ppid = %d\n", getpid(), getppid());
            sprintf(message, "%d\n", getpid());
            printf("message envoyé: %s", message);
            nb = write(pipefd[1], message, strlen(message));
            
            printf("Le processus P3 attend un signal\n");
            pause(); // attente d'un signal
            wait(&status);
            printf("Fin de processus  P3\n");

        } else {
            // Enfant P2
            printf("Enfant p2 pid = %d ppid = %d\n", getpid(), getppid());

            wait(&status);
            wait(&status);
            printf("Fin de processus P2\n");
        }
    } else { // Père P1


        sleep(1);

        //lecture du pipe
        nb = read(pipefd[0], buffer, 255); // Lecture du message dans le tube       
        printf("processus %d - %d octets recus : %s", getpid(), nb, buffer);
        sscanf(buffer, "%d", &pid3);
        retour = kill(pid2, SIGUSR1); // envoi d'un signal USR1 a enfant 2
        printf("P1 envoie un signal USR1 à P2\n");
        retour = kill(pid3, SIGUSR1); // envoi d'un signal USR1 a enfant 3
        printf("P1 envoie un signal USR1 à P3\n");

        wait(&status);
        printf("Fin de processus P1\n");
    }
    return EXIT_SUCCESS;
}



/***********************************************************************
   Correction du TP de programmation système UNIX
   TP Mini-shell

   Question 5 Redirection de la sortie standard

   Les parties modifiées par rapport à la question 4 sont identifiées
   par le symbole XXX.

   -   Dimanche 6 Novembre 2016
   -   Compilation : gcc minishell5.c -o minishell5 -Wall
 ************************************************************************/
// les couleurs
#define ROUGE  "\033[1;31m"
#define JAUNE  "\033[1;33m"
#define VERT   "\033[1;32m"
#define RESET  "\033[1;0m"
#define CYAN   "\033[1;36m"
#define BLEU   "\033[1;34m"


/* en-têtes standard */
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>   /* pour avoir wait & co. */
#include <ctype.h>      /* pour avoir isspace & co. */
#include <string.h>
#include <errno.h>      /* pour avoir errno */
#include <signal.h>
#include "signaux.h"    // la liste des signaux en clair
#include <fcntl.h>      // pour avoir open
char   ligne[4096];     /* contient la ligne d'entrée */

/* pointeurs sur les mots de ligne (voir decoupe) */
#define MAXELEMS 32
char* elems[MAXELEMS];

void affiche_invite()
{
<<<<<<< HEAD
    printf(BLEU);
    printf("SNIR shell5 > ");
    printf(RESET);
    fflush(stdout);
=======
  printf("SNIR shell5 > ");
  fflush(stdout);
>>>>>>> 8c3ecd6a579ee325953ba8fa602c4a7e59de74f6
}

void lit_ligne()
{
  if (!fgets(ligne,sizeof(ligne)-1,stdin)) {
    printf("\n");
    exit(0);
  }
}

/* découpe ligne en mots
   fait pointer chaque elems[i] sur un mot différent
   elems se termine par NULL
 */
void decoupe()
{
  char* debut = ligne;
  int i;
  for (i=0; i<MAXELEMS-1; i++) {

    /* saute les espaces */
    while (*debut && isspace(*debut)) debut++;

    /* fin de ligne ? */
    if (!*debut) break;

    /* on se souvient du début de ce mot */
    elems[i] = debut;

    /* cherche la fin du mot */
    while (*debut && !isspace(*debut)) debut++; /* saute le mot */

    /* termine le mot par un \0 et passe au suivant */
    if (*debut) { *debut = 0; debut++; }
  }
  elems[i] = NULL;
}

/* attent la fin du processus pid */
void attent(pid_t pid)
{
  /* il faut boucler car waitpid peut retourner avec une erreur non fatale
     quand le processus fils a été interrompu. errno = EINTR (erreur N°4) */
  while (1) {
    int status;
    int r = waitpid(pid,&status,0); /* attente bloquante pour le père*/
    if (r<0) {
      if (errno==EINTR) continue; /* le fils a été interrompu => on recommence à attendre */
      printf("erreur de waitpid (%s)\n",strerror(errno));
      break;
    }
    if (WIFEXITED(status)){                            /* renvoie vrai si le fils s'est terminé normalement */
        if (WEXITSTATUS(status) == 0)
            printf(VERT);
        else
            printf(JAUNE);
    printf("Pid = %d Terminaison normale, status %i\n", pid, WEXITSTATUS(status)); /* renvoie le code de sortie du fils. */
    }
    if (WIFSIGNALED(status)){                 /* renvoie vrai si le fils s'est terminé à cause d'un signal. */
      printf(ROUGE);
      printf("Pid = %d Terminaison par signal %d %s\n",
                pid,
                WTERMSIG(status),
                listeSignaux[WTERMSIG(status)]);/* renvoie le numéro du signal qui a causé la fin du fils.*/
    }
    printf(RESET);
    break;
  }
}

void execute()
{
  pid_t pid;
  struct sigaction sig;

  if (!elems[0]) return; /* ligne vide */

  if (strcmp(elems[0],"exit")==0){

        printf("bye bye à bientôt !\n");           // Primitive exit on dit bye bye
        exit(0);

  }
  if (strcmp(elems[0],"cd")==0){
    if (elems[1])
        chdir(elems[1]);                         // XXX primitive cd Change Directory
    else{
        chdir(getenv ("HOME"));                 // si cd seul on retourne dans home
    }
    return;
  }

  /* désactive l'interruption par Contrôle+C.   */
  sig.sa_flags = 0;
  sig.sa_handler = SIG_IGN;
  sigaction(SIGINT, &sig, NULL);

  pid = fork();
  if (pid < 0) {
    printf("fork a échoué (%s)\n",strerror(errno));
    return;
  }

  if (pid==0) {
    /* fils */
    int i;
    /* Réactive l'interruption par Contrôle+C */
    sig.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sig, NULL);

    /* XXX redirection cherche le dernier argument de la ligne */
    for (i=0;elems[i+1];i++);

    if (elems[i][0]=='>') {
        /* XXX redirection ouvre le fichier destination */
        int file = open(&elems[i][1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file==-1) {
	    printf("ne peut pas créer le fichier \"%s\" (%s)\n",
	       &elems[i][1],strerror(errno));
	    exit(1);
        }

        /* XXX redirection redirige la sortie standard sur file */
        close(1);     // ferme la sortie standard
        dup2(file,1); //duplique file avec 1 donc file devient la sortie standard

        /* XXX Q5 supprime le dernier argument */
        elems[i] = NULL;
    }

    execvp(elems[0], /* programme à exécuter */
	   elems     /* argv du programme à exécuter */
	   );
    printf("impossible d'éxecuter \"%s\" (%s)\n",elems[0],strerror(errno));
    exit(1);
  }// fin de fils
  else {
    /* père */
    printf("Pid = %d %s\n", pid, elems[0]);
    attent(pid);

    /* XXX réactive l'interruption par Contrôle+C */
    sig.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sig, NULL);
  }
}

int main()
{
  while (1) {
    affiche_invite();
    lit_ligne();
    decoupe();
    execute();
  }
  return 0;
}

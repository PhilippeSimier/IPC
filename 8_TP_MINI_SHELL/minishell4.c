/***********************************************************************
   Correction du TP de programmation système UNIX
   TP Mini-shell

   Question 4 Contrôle+C interrompe la commande et redonne la main au minishell

   Les parties modifiées par rapport à la question 2 sont identifiées
   par le symbole XXX.

   -   mardi 1 Novembre 2016
   -   Compilation : gcc minishell3.c -o minishell3 -Wall
 ************************************************************************/
// les couleurs
#define ROUGE  "\033[1;31m"
#define JAUNE  "\033[1;33m"
#define VERT   "\033[1;32m"
#define RESET  "\033[1;0m"


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

char   ligne[4096];     /* contient la ligne d'entrée */

/* pointeurs sur les mots de ligne (voir decoupe) */
#define MAXELEMS 32
char* elems[MAXELEMS];


void affiche_invite()
{
  printf("SNIR shell4 > ");
  fflush(stdout);
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
      printf("Terminaison normale, status %i\n",WEXITSTATUS(status)); /* renvoie le code de sortie du fils. */
    }
    if (WIFSIGNALED(status)){                 /* renvoie vrai si le fils s'est terminé à cause d'un signal. */
      printf(ROUGE);
      printf("\nTerminaison par signal %i\n",WTERMSIG(status));/* renvoie le numéro du signal qui a causé la fin du fils.*/
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
        printf("bye bye les SNIRS à bientôt !\n");           // XXX primitive exit on dit bye bye
        exit(0);

  }
  if (strcmp(elems[0],"cd")==0){
    if (elems[1])
        chdir (elems[1]);                         // XXX primitive cd Change Directory
    else{
        chdir (getenv ("HOME"));                 // si cd seul on retourne dans home
    }
    return;
  }

  /* XXX désactive l'interruption par Contrôle+C.
     Il faut appeler sigaction _avant_ fork pour éviter une 'race-condition':
     si sigaction est appelé après fork, il existe un laps de temps entre le
     fork et le sigaction où le père et le fils sont tous deux interruptibles
     par SIGINT.
   */
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

    /* XXX réactive l'interruption par Contrôle+C */
    sig.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sig, NULL);

    execvp(elems[0], /* programme à exécuter */
	   elems     /* argv du programme à exécuter */
	   );
    printf("impossible d'éxecuter \"%s\" (%s)\n",elems[0],strerror(errno));
    exit(1);
  }
  else {
    /* père */
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

/***********************************************************************
   Correction du TP de programmation système UNIX
   TP Mini-shell

   Question 8 Lecture de la ligne de commande avec readline
	      et Gestion de l'historique

   Les parties modifiées par rapport à la question 7 sont identifiées
   par le symbole XXX. Vous devez installez readline
   sudo apt-get install libreadline.dev

   -   Auteur  Philippe SIMIER (Touchard le Mans)
   -   Dimanche 13 Novembre 2016
   -   Compilation : gcc minishell8.c -o minishell8 -lreadline -Wall
*************************************************************************/

// les couleurs
#define ROUGE  "\033[1;31m"
#define JAUNE  "\033[1;33m"
#define VERT   "\033[1;32m"
#define RESET  "\033[1;0m"
#define CYAN   "\033[1;36m"
#define BLEU   "\033[1;34m"


// en-têtes standard
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>           // pour avoir wait & co.
#include <ctype.h>     	        // pour avoir isspace & co.
#include <string.h>
#include <errno.h>      	// pour avoir errno
#include <signal.h>
#include "signaux.h"    	// la liste des signaux en clair
#include <fcntl.h>      	// pour avoir open
#include <readline/readline.h>  // pour avoir readline
#include <readline/history.h>   // pour avoir l'historique des commandes

char   *ligne = (char *)NULL;   // XXX pointeur sur la ligne d'entrée

// Tableau de pointeurs sur les mots de ligne (voir decoupe)
#define MAXELEMS 32
char* elems[MAXELEMS];

// XXX Lit une chaine, et retourne un pointeur sur elle.  Retourne NULL pour EOF.
char *lit_ligne()
{
  // Si  return la mémoire est libérée
  if (ligne)
    {
      free (ligne);
      ligne = (char *)NULL;
    }

  // Affiche l'invite
  ligne = readline ("SNIR shell8 > ");

  /* Si la ligne contient du text, alors on la place dans l'historique. */
  if (ligne && *ligne)
    add_history (ligne);

  return (ligne);
}
// XXX fin des modifications

/* découpe la ligne en mots
   fait pointer chaque elems[i] sur un mot différent
   elems se termine par NULL
 */
void decoupe()
{
  char* debut = ligne;
  int i;
  for (i=0; i<MAXELEMS-1; i++) {

    // saute les espaces
    while (*debut && isspace(*debut)) debut++;

    // fin de ligne ?
    if (!*debut) break;

    // on se souvient du début de ce mot
    elems[i] = debut;

    // Cherche la fin du mot
    while (*debut && !isspace(*debut)) debut++; /* saute le mot */

    /* termine le mot par un \0 et passe au suivant */
    if (*debut) { *debut = 0; debut++; }
  }
  elems[i] = NULL;
}

/* fonction handler qui sera éxecutée pour les signaux HLD reçus */

volatile int front_pid = -1;  // variable globale

void handlerHLD(int signal)
{
  /* il faut boucler car waitpid peut retourner avec une erreur non fatale
     quand le processus fils a été interrompu. errno = EINTR (erreur N°4) */
    while (1) {
   	int status;
    	pid_t pid = waitpid(-1,&status,WNOHANG); /*-1 attend la fin de n'importe quel processus fils */
    	if (pid<0) {
      	   if (errno==EINTR) continue; /* le fils a été interrompu => on recommence à attendre */
           if (errno==ECHILD) break;   /* plus de fils terminé => on quitte */
           printf("erreur de waitpid (%s)\n",strerror(errno));
           break;
        }
        if (pid == 0) break; /*plus de fils on quitte*/

	/* XXX signale à la fonction execute() que front_pid s'est terminé */
	if (pid==front_pid) front_pid = -1;

        if (WIFEXITED(status)){  // renvoie vrai si le fils s'est terminé normalement
            if (WEXITSTATUS(status) == 0)
                printf(VERT); // Ecrit en vert si statut = 0 en jaune sinon
            else
                printf(JAUNE);
            printf("Pid = %d Terminaison normale, status %i\n", pid, WEXITSTATUS(status)); /* renvoie le code de sortie du fils. */
        }
        if (WIFSIGNALED(status)){  /* renvoie vrai si le fils s'est terminé à cause d'un signal. */
      	    printf(ROUGE);
            printf("Pid = %d Terminaison par signal %d %s\n",
                pid,
                WTERMSIG(status),
                listeSignaux[WTERMSIG(status)]);/* renvoie le numéro du signal qui a causé la fin du fils.*/
        }
        printf(RESET);
    }
}

void execute()
{
  sigset_t sigset;
  pid_t pid;
  int i;
  char en_fond; // Indique si la commande s'exécute en tache de fond

  if (!elems[0]) return; // ligne vide

  if (strcmp(elems[0],"exit")==0){
	printf(VERT);
        printf("bye bye à bientôt !\n");  // Primitive exit on dit bye bye
        printf(RESET);
        exit(0);

  }
  if (strcmp(elems[0],"cd")==0){
    if (elems[1])
        chdir(elems[1]);               // Primitive cd Change Directory
    else{
        chdir(getenv ("HOME"));        // Si cd seul on retourne dans home
    }
    return;
  }

  // Détection du & en fin de ligne
  for (i=0;elems[i+1];i++);
  if (elems[i][0]=='&') {
	elems[i] = NULL;
	en_fond = 1;
        printf(VERT);
	printf("Cette tache sera exécutée en arrière plan\n");
        printf(RESET);
  }
  else {
    /* bloque SIGCHLD jusqu'à  ce que le père ait placé le pid du
       fils dans front_pid
       sinon on risque de manquer la notification de fin du fils
       (race condition)
    */
    sigemptyset(&sigset);
    sigaddset(&sigset,SIGCHLD);
    sigprocmask(SIG_BLOCK,&sigset,NULL);
    en_fond = 0;
  }



  pid = fork();
  if (pid < 0) {
    printf("fork a échoué (%s)\n",strerror(errno));
    return;
  }

  if (pid==0) {
    // On est dans le fils

    if (en_fond) {
    	// Redirection de l'entrée standard sur /dev/null
    	int devnull = open("/dev/null",O_RDONLY);
    	if (devnull != -1) {
            close(0);
            dup2(devnull,0);
        }
    }
    else {
      // Réactivation de SIGINT & débloque SIGCLHD
      struct sigaction sig;
      sig.sa_flags = 0;
      sig.sa_handler = SIG_DFL;
      sigemptyset(&sig.sa_mask);
      sigaction(SIGINT,&sig,NULL);
      sigprocmask(SIG_UNBLOCK,&sigset,NULL);
    }

    // Redirection de la sortie cherche le dernier argument de la ligne
    for (i=0;elems[i+1];i++);

    if (elems[i][0]=='>') {
        // Redirection ouvre le fichier destination
        int file = open(&elems[i][1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (file==-1) {
	    printf("Ne peut pas créer le fichier \"%s\" (%s)\n",
	       &elems[i][1],strerror(errno));
	    exit(1);
        }

        // redirection de la sortie standard sur file
        close(1);     // ferme la sortie standard
        dup2(file,1); //duplique file avec 1 donc file devient la sortie standard

        //  supprime le dernier argument
        elems[i] = NULL;
    }

    execvp(elems[0], // programme à exécuter
	   elems     // argv du programme à exécuter
	   );
    printf("impossible d'éxecuter \"%s\" (%s)\n",elems[0],strerror(errno));
    exit(1);
  }// fin de fils
  else {
    //père
    if (!en_fond) {
       printf(VERT);
       printf("%s [Pid = %d]\033[1;0m\n", elems[0], pid);
       printf(RESET);
       front_pid = pid;
       sigprocmask(SIG_UNBLOCK,&sigset,NULL);
       /* attente bloquante jusqu'à ce que le handlerHLD signale que front_pid s'est terminé */
       while (front_pid!=-1) 
		pause();
    }
  }
}

int main()
{
  // Installation du handler pour le signal SIGCHLD
  struct sigaction sig;
  sig.sa_flags = 0;
  sig.sa_handler = handlerHLD;
  sigemptyset(&sig.sa_mask);
  sigaction(SIGCHLD, &sig, NULL);

  // Désactivation l'interruption par Controle+C
  sig.sa_handler = SIG_IGN;
  sigaction(SIGINT, &sig, NULL);

  while (1) {
    lit_ligne();
    decoupe();
    execute();
  }
  return 0;
}

/*************************************************************
       Correction du TP mini shell  programmation système UNIX
       TP Mini-shell
       Question 1
       mardi 1 Novembre 2016

	Compilation : gcc minishell1.c -o minishell1 -Wall
**************************************************************/

// en-têtes standard
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/wait.h>   // pour avoir wait & co.
#include <ctype.h>      // pour avoir isspace & co.
#include <string.h>
#include <errno.h>      // pour avoir errno

char ligne[4096];       //Variable globale contient la ligne d'entrée


void affiche_invite()
{
  printf("SNIR shell1 > ");
  fflush(stdout);       // pour forcer l'affichage
}

void lit_ligne()
{
  if (!fgets(ligne,sizeof(ligne)-1,stdin)) {
    /* ^D ou fin de fichier => on quittte */
    printf("\n");
    exit(0);
  }
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
    if (WIFEXITED(status))  			       /* renvoie vrai si le fils s'est terminé normalement */
      printf("terminaison normale, status %i\n",WEXITSTATUS(status)); /* renvoie le code de sortie du fils. */
    if (WIFSIGNALED(status))                  /* renvoie vrai si le fils s'est terminé à cause d'un signal. */
      printf("terminaison par signal %i\n",WTERMSIG(status));/* renvoie le numéro du signal qui a causé la fin du fils.*/
    break;
  }
}

/* execute la ligne */
void execute()
{
  pid_t pid;

  /* supprime le \n final */
  if (strchr(ligne,'\n'))
	*strchr(ligne,'\n') = 0;

  /* saute les lignes vides */
  if (!strcmp(ligne,""))
	return;

  pid = fork();
  if (pid < 0) {
    printf("fork a échoué (%s)\n",strerror(errno));
    return;
  }

  if (pid==0) {
    /* c'est le fils */
    execlp(ligne, /* programme à exécuter */
	   ligne, /* argv[0], par convention le nom de programme exécuté */
	   NULL   /* pas d'autre argument */
	   );

    /* on n'arrive ici que si le exec a échoué */
    printf("impossible d'éxecuter \"%s\" (%s)\n",ligne,strerror(errno));
    exit(1);
  }
  else {
    /* c'est le père qui attend la fin d'éxécution*/
    attent(pid);
  }
}

int main()
{
  /* boucle d'interaction */
  while (1) {
    affiche_invite();
    lit_ligne();
    execute();
  }
  return 0;
}

# Gestion des signaux

##Définition

Un **signal** est un **message envoyé par le noyau à un processus** pour indiquer l’occurrence d'un événement survenu au niveau du système.
Ce message ne comporte pas d'informations propres si ce n'est le nom du signal lui-même.
### Signaux classiques

| N° |   Nom  | Description |
|----|--------|------------------|
| 1  | SIGHUP | Terminaison du processus | 
| 2	 | SIGINT | Interruption du clavier (ctrl C)|
| 3	 | SIGQUIT|	Caractère (QUIT) frapper au clavier (ctrl \)|
| 4  | SIGILL |	Instruction illégale |
| 5  | SIGTRAP|	Point d'arrêt pour de débogage |
| 6	 | SIGABRT| Terminaison anormale |
| 7	 | SIGBUS | Erreur de Bus|
| 8  | SIGFPE |	Floating-point exception - Exception arithmétique
| 9	 | SIGKILL|	terminaison forcée du processus
|10 | SIGUSR1 |	Signal utilisateur 1
|11 | SIGSEGV |	Violation de mémoire
|12	| SIGUSR2 |	Signal utilisateur 2
|13 | SIGPIPE |	Ecriture dans un tube sans lecteur
|14	| SIGALRM |	Fin de temporisation
|15	| SIGTERM |	Terminaison de processus
|16	| SIGSTKFLT | Erreur de pile du coprocesseur
|17	| SIGCHLD |	Processus fils terminé
|18	| SIGCONT |	Reprise du processus stoppé
|19	| SIGSTOP |	Stop l'exécution du processus
|20	| SIGTSTP |	Demande de suspension depuis le clavier (ctrl Z)
|21	| SIGTTIN |	Lecture par un processus en arrière-plan
|22	| SIGTTOU |	Ecriture par un processus en arrière-plan
|23 | SIGURG  |	Données urgentes sur socket
|24	| SIGXCPU |	Limite de temps CPU dépassé
|25	| SIGXFSZ | Taille maximale de fichier atteinte
|26	| SIGVTALRM | Alarme virtuelle
|27	| SIGPROF |	Profiling alarm clock
|28	| SIGWINCH | Changement de taille de fenêtre
|29	| SIGIO | Arrivé de caractères à lire
|30	|SIGPWR	| Chute d'alimentation
|31	|SIGUNUSED | Non utilisé

La prise en compte du signal par le processus oblige celui-ci a exécuter une fonction de  gestion du signal appelée *handler  de signal*

###Envoi d'un signal
Un processus peut recevoir un signal de **deux façons différentes**:

 - Un signal lui est envoyé par **un autre processus** par l'intermédiaire de la primitive kill().  Exemple le shell envoie le signal SIGKILL si la commande **kill -9 pid** est saisie; autre exemple par programme envoie du signal SIGUSR1 au processus ayant pour pid : la valeur de la variable pid1:
```c
#include <unistd.h>
#include <signal.h>  // pour la primitive kill()
....
	retour = kill(pid1, SIGUSR1);
	if (retour !=0) {
				printf("Erreur avec kill");
	}
```
 - L'exécution du processus a levé une trappe et le gestionnaire d'exception associé positionne un signal pour signaler l'erreur détectée. (Par exemple l’occurrence d'une division par zéro lève le signal SIGFPE). 

### Prise en compte du signal
Pour la prise en compte d'un signal reçu par un processus, trois types d'actions peuvent être réalisées:

 - Ignorer le signal;
 - exécuter l'action par défaut; 
 - exécuter une fonction spécifique installé par le programmeur
Tout processus peut installer un traitement spécifique pour chacun des signaux, hormis le signal SIGKILL. Ce traitement spécifique remplace alors le traitement par défaut défini par le noyau.

**Exemple** Association d'un traitement spécifique affichage() pour le signal SIGUSR1
```c
#include <unistd.h>
#include <signal.h>

void affichage(int sig)
{   static int cpt = 0;
    cpt++;
    printf("%d : Signal %d recu par %d\n", cpt, sig, getpid());
}
...
(void) signal(SIGUSR1, affichage); // prise en compte des signaux SIGUSR1 par la fonction affichage
```
la primitive signal() permet d'attacher une fonction à un signal. le premier argument est le nom du signal. Le deuxième argument est un pointeur vers une fonction handler définie dans le code (dans l'exemple ci-dessus la fonction affichage) 

## Attendre un signal
La primitive pause() permet de se mettre en attente de la délivrance d'un signal. Son prototype est :
```c
#include<unistd.h>
int pause(void);
```
Cette primitive ne permet pas de spécifier ni l'attente d'un signal particulier, ni de savoir quel signal a réveillé le processus.

## Armer une temporisation

La primitive **alarm()** permet à un processus d'armer une temporisation. A l'issue de cette temporisation le signal SIGALRM est délivré au processus. Le comportement par défaut est d'arrêter l'exécution du processus.
```c
#include<unistd.h>
unsigned int alarm(unsigned int nb_sec);
```
Une utilisation courante de la fonction alarm() consiste à armer une temporisation avant la réalisation d'un appel système bloquant. Le délai écoulé, la levée du signal SIGALRM fait échouer l'appel système bloquant.

L'opération alarm(0) annule une temporisation précédemment armée.
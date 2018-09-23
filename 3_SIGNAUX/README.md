# Gestion des signaux

## Définition

Un **signal** est un **message envoyé par le noyau à un processus** pour indiquer l’occurrence d'un événement survenu au niveau du système.
Ce message ne comporte pas d'informations propres si ce n'est le nom du signal lui-même.

La prise en compte du signal par le processus oblige celui-ci a exécuter une fonction de  gestion du signal appelée *handler  de signal*

### Envoi d'un signal
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

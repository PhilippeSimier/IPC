# Les Threads

### notion de processus léger

Le processus léger (thread) constitue une extension du modèle traditionnel de processus. L’extension consiste à admettre plusieurs fils d'exécutions indépendants dans un même espace d'adressage. Chacun de ces fils d'exécution est appelé **thread** et l'entité contenant les différents fils d'exécution est appelé processus.

L'avantage lié à la notion de processus léger est un allègement des opérations de commutations de contexte, puisqu'elle consiste seulement à changer de pile et de valeur de compteur ordinal, le contexte mémoire reste le même. de même l'opération de création d'un nouveau thread est allégée puisqu'elle ne nécessite pas la duplication complète de l'espace mémoire du processus père.

### Implémentation au niveau noyau

Lorsque l'implémentation des threads est effectuées au niveau du noyau, alors celui-ci connaît leur existence et il attribue le processeur à chacun des threads de manière indépendante. Chaque descripteur de processus contient alors également la table des threads qui le composent avec pour chacun d'eux la sauvegarde de son contexte processeur.  

## Fonctions de gestion des threads

Elles sont définies dans la librairie **pthread**,  il faut donc inclure
```c
#include <pthread.h>
```
Il faut aussi ajouter l'option **-lpthread** lors de l'appel à l'éditeur de liens.
Chaque thread est identifié de manière unique par un type **pthread_t**. ce type joue un rôle analogue au type pid_t des processus.

### Création d'un thread
La primitive **pthread_create** permet la création  au sein d'un processus, d'un nouveau thread identifié par T. Le fil d'exécution démarre son exécution au début de la fonction spécifiée tache1 et disparait à la fin de l'exécution de celle-ci. 

```c
pthread_t T;
res = pthread_create(&T, NULL, tache1, (void *)&a);  
```
Le deuxième argument correspond aux attributs associés au thread. Cette argument est le plus souvent mis à NULL pour hériter des attributs standards. 
le troisième argument correspond à la fonction associée au thread.
le quatrième argument correspond est un argument passé à la fonction tache1.

### Terminaison d'un thread
La primitive **pthread_exit(void *ret)**  met fin au thread qui l'exécute. Elle retourne le paramètre ret qui peut être récupéré par un autre thread effectuant pour sa part un appel **pthread_join(T, &thread_result)**  ou T correspond à l'identifiant du thread attendu, et thread_result correspond à la valeur ret retournée lors de la terminaison.

### Attributs d'un thread
Les attributs d'un thread sont les suivants:
 - l'adresse de départ et la taille de la pile associée
 - la politique d'ordonnancement qui lui est associée
 - la priorité qui lui est associée
	 - son attachement ou son détachement. Un thread détaché se termine immédiatement sans pouvoir être pris en compte par un pthread_join.

## Mutex
Un mutex est une variable de type pthread_mutxt_t servant de verrou pour protéger l'accès à des zones de codes ou de données particulières. Ce verrou peut prendre deux états disponible ou verrouillé, et il ne peut être acquis que par un seul thread à la fois. Un thread demandant a verrouiller un mutex déjà acquis par un autre thread est mis en attente.
### Initialisation d'un mutex
```c
pthread_mutex_t verrou = PTHREAD_MUTEX_INITIALIZER; 
```
### Verrouillage d'un mutex
Le verrouillage s'effectue en appelant la fonction 
```c
pthread_mutex_lock(&verrou); 
```
###  Déverrouillage d'un mutex
La libération d'un mutex s'effectue en appelant la fonction
```c
pthread_mutex_unlock (&verrou);
```

### Destruction d'un mutex
```c
pthread_mutex_destroy (&verrou);
```

## Changelog

 **28/09/2018 :** Ajout du README 
 
 **10/09/2018 :** Ajout du TD fichier pdf
 
> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur **Philippe SIMIER Lycée Touchard Le Mans**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#




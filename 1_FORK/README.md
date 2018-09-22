# fork()  wait() waitpid()

Un processus est un programme en cours d'exécution auquel est associé un environnement processeur et un environnement mémoire appellés *contexe du processus.* Chaque processus possède une plage mémoire dont l'accès lui est réservé.

Tout processus Linux peut créer un autre processus. Le processus créateur est qualifié de *père* tandis que le processus créé est qualifié de *fils*. Il découle de cette possibilité la formation d'une arborescence de processus reliés entre eux par des liens de filiation.
Chaque processus Linux est caractérisé par un numéro unique PID qui est attribué par le système au moment de sa création.

## fork()
```c
#include <unistd.h>
```
la primitive fork() permet la création d'un nouveau processus qui est une copie exacte de lui même et qui s'exécute de manière concurrente avec le processus qui l'a créé.
A l'issue de l'exécution du fork(), chaque processus, le père et le fils reprend son exécution après le fork(), le code et les données sont strictement identique sauf pour le code de retour du fork().    c'est ce qui permettra de différentier le fils du père. 

 - Dans le processus fils le **code de retour** du fork() est toujours **0** ;
 - Dans le processus père le **code de retour** du fork() est **le pid du fils créé**.
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main ()
{
	int pid0, pid1;
	pid1 = fork();
	if ( pid1 == 0 )
	{       // Enfant P1
		printf ( "je suis le fils, mon pid est %d \n", getpid());
		printf ( "pid de mon père, %d\n", getppid());
	}
	else
	{       // Père
	    printf ( "je suis le père, mon pid est %d \n", getpid());
		printf ( "pid de mon fils, %d\n", pid1);
	}
}	
```

## Terminaison d'un processus exit()
Un processus termine normalement son exécution en achevant l'exécution du code qui lui est associé. 
Cette terminaison s'effectue aussi par le biais d'un appel à *exit(status)* ou status est le code de retour compris entre 0 et 255. Par convention une valeur de retour 0 caractérise une terminaison normale du processus. 
La terminaison d'un processus peut aussi être forcée par le système  d'exploitation en cas d'erreur comme par exemple une erreur de segmentation.. 

## Synchronisation wait()
Un processeur père peut se mettre en attente de la fin d'exécution de l'un de ces fils, par l'appel à la primitive wait(&status).  Cette fonction est bloquante. 
Si un processus fils est déjà dans l'état zombie (c'est à dire qui a terminé son exécution et attends la prise en compte de cette fin) au moment de l'appel de la fonction wait(&status), celle-ci retourne immédiatement le résultat à savoir le PID du fils terminé et le code de retour dans la variable status.  

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main ()
{
	int pid0, pid1;
	int status;
	
	pid1 = fork();
	if ( pid1 == 0 )
	{       // Enfant P1
		printf ( "je suis le fils, mon pid est %d \n", getpid());
		printf ( "pid de mon père, %d\n", getppid());
		exit(0);
	}
	else
	{       // Père
	    printf ( "je suis le père, mon pid est %d \n", getpid());
		printf ( "pid de mon fils, %d\n", pid1);
		fils_mort = wait(&status);
		printf ( "Le code de retour de mon fils est %d\n", %d);
	}
}	
```
Nota : seul le père peut attendre la fin d'exécution d'un de ses fils, l'inverse n'est pas possible, autrement dit, un fils ne peut pas attendre la fin d'exécution de son père.
Commande linux:
**ps** Lancer cette commande sans argument montrera uniquement les processus dont vous êtes l'initiateur et qui sont rattachés au Terminal que vous utilisez.
**pstree** La commande pstree affiche les processus sous forme d'arborescence et permet de les visualiser par leurs liens de parenté. 

```bash
pi@raspberrypi3:~ $ pstree -up 510
sshd(510)─┬─sshd(1265)───sshd(1295,pi)───bash(1297)───pstree(2558)
          └─sshd(2072)───sshd(2096,pi)───bash(2098)

```
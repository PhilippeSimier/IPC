# Files de message

### Introduction
 Les IPC *(inter processus communication)* Forme un groupe de trois outils de communication indépendant du système de gestion de fichiers. Ces outils sont :
 

 - Les fils de messages ou MSQ (*Messages Queues*)
 - Les mémoires partagées
 - Les Sémaphores
 
Un outil IPC est identifié de manière unique par un identifiant externe appelé la **clef**.
La commande **ipcs** permet de lister l'ensemble des outils IPC existant à un moment donné sur un système.
```bash
psimier@b106tu4p4 ~ $ ipcs

------ Files de messages --------
clef       msqid      propriétaire perms      octets utilisés messages    

------ Segment de mémoire partagée --------
clef       shmid      propriétaire perms      octets     nattch     états      
0x00000000 196608     psimier    600        524288     2          dest         
0x00000000 360449     psimier    600        524288     2          dest          
0x00000000 3899416    psimier    600        28672      2          dest         

------ Tableaux de sémaphores --------
clef       semid      propriétaire perms      nsems     

psimier@b106tu4p4 ~ $ 

```

## Les files de message

Le noyau Linux gère au maximum 128 files de messages pouvant contenir 4056 octets.

###  Accès à une file de messages
L'accès à un file de message s'effectue par l'intermédiaire de la primitive msget(). Cette primitive permet :

 - La création d'une nouvelle file de message;
 - l'accès à une file de message déjà existante;
 
```c
 /* Accès à la file pour la clé key */
 idFile = msgget( key , 0666|IPC_CREAT );
	if (idFile==-1){
		printf("pb creation file : %s\n",strerror(errno));
		exit(1);
	}
```
Le premier paramètre correspond à la clef, c'est l'identifiant externe de la file de messages. Le deuxième paramètre est une combinaison des constantes IPC_CREAT, IPC_EXCL et de droits d'accès définis comme dans le cadre des fichiers. En cas de succès la fonction renvoie l'identifiant interne de la file de message, cette identifiant joue le même rôle qu'un descripteur de fichier. 

La création d'un file de message est demandée en positionnant les constantes IPC_CREAT et IPC_EXCL. Une nouvelle file est alors créée avec les droits d'accès définis dans le paramètre option. 
si **IPC_CREAT et IPC_EXCL** sont utilisé conjointement et qu'une file existe déjà avec l'identifiant externe cle alors une erreur une erreur est générée.
si IPC_CREAT est positionné seul alors qu'un file d''identifiant externe cle existe déjà alors l'accès à cette file est retournée.
si **IPC_PRIVAT** est utilisé alors une file de massage est créée et seulement accessible par ce processus et ses descendants.

### Format des messages
Un message est toujours composée de deux parties :

 - La première partie constitue le type du message, c'est un entier long
 - la seconde partie est composée des données proprement dites.
```c
struct message{
	long type;
	char donnee[9];
};
```
### Envoi d'un message

L'envoi d'un message s'effectue par l'intermédiaire de la primitive **msgsnd()**. Par défaut msgsnd() est bloquant c'est à dire que le processus est suspendu lors d'un dépôt d'un message si la file est pleine. En positionnant le paramètre IPC_NOWAIT la primitive de dépôt devient non bloquante.
```c
msgsnd(idFile,(void*)&maFile,sizeof(maFile.texte),IPC_NOWAIT); 
```
### Réception d'un message

Un processus désirant prélever un message depuis une file de messages utilise la primitive **msgrcv()**
```c
ret=msgrcv(idFile,(void*)&maFile,9,2,IPC_NOWAIT);
```

 - idFile :  correspond à l'identifiant interne de la file
 - (void*)&maFile : adresse de la zone mémoire pour recevoir le message
 - 9 : longueur des données dans le message
 - 2 : permet de désigner le type à extraire. Le message le plus ancien dont le type est égal à 2 est extrait en premier. 
 - IPC_NOWAIT : la primitive devient non bloquante.
 - ret : la fonction renvoie la longueur du message prélevé en cas de success, -1 sinon.
 
 Par défaut la primitive  **msgrcv()** est bloquante, c'est à dire que le processus est suspendu lors du retrait d'un message si la file ne contient pas de message au type attendu.

### Destruction d'une file de message
La destruction d'une file de message s'effectue en utilisant la primitive **msgctl()** 
```c
msgctl( idFile, IPC_RMID, NULL);
```







## Changelog

 **02/10/2018 :** Ajout du README . 

 
> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur **Philippe SIMIER Lycée Touchard Le Mans**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#




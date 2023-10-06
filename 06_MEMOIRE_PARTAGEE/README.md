# Mémoire partagée

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

## Les régions de mémoire partagée
Par défaut, l'espace mémoire de chaque processus est privé, c'est à dire que cet espace ne peut pas être accédé par un autre processus. Les régions de mémoire partagées (*shared memory*) constituent une extension de l'espace d'adressage d'un processus qui peut être partagé par un autre processus.

Ce partage d'une zone mémoire par plusieurs processus implique de synchroniser les accès aux données afin que la cohérence soit respectée. Cette sécurisation sera réalisée à l'aide des sémaphores.

### Création d'une région de mémoire partagée

```c
#define CLE = 1234

// Création de l'identifiant key externe
if ((key = ftok("/tmp/bidon" , 1234 )) == -1){
        perror("ftok");
        exit(2);
}
shmid = shmget( key, sizeof(struct donnees), IPC_CREAT | 0600 );
```
le premier paramètre **key** correspond à l'identification externe de la mémoire partagée.
le deuxième paramètre correspond à la taille de la mémoire partagée que l'on désire.
Le troisième correspond aux droits d'accés. si IPC_CREAT est positionné alors la mémoire partagée est créée si elle n’existe pas sinon la mémoire partagée existante est retournée.
La fonction retourne l'identifiant interne de la mémoire partagée. -1 si erreur.

### Attachement d'une région de mémoire partagée 
```c
char *mem;
mem = shmat(shmid,NULL,SHM_W);
```
le premier argument est l'identifiant interne;
le deuxième argument spécifie l'adresse de la région partagée si NULL alors la région est attachée à la première adresse disponible déterminée par le système.
Le troisième paramètre définit les droit d'accès à la région. 
La primitive renvoie l'adresse de la région de mémoire partagée en cas de succès. -1 en cas d'erreur.

### détachement d'une région de mémoire partagée
```c
shmdt(mem);
```

### destruction d'une région de mémoire partagée
```c
shmctl(shmid, IPC_RMID, NULL); 
```
la destruction d'une région de mémoire partagée peut également être réalisé" depuis le shell par la commande 
```bash
ipcrm -Q cle
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




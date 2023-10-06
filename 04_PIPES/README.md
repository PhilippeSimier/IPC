# La communication par tubes

Le système Linux propose deux système de communication par tubes: les **tubes anonymes** et les **tubes nommés**.
Le tube est un tuyau dans lequel un processus peut écrire des données qu'un autre processus peut lire. La communication dans le tube est **unidirectionnelle**.

Les tubes sont gérés par le système au niveau du système de gestion de fichiers et correspondent à un fichier au sein de celui-ci. Lors de la création du tube, deux descripteurs sont créés, un pour lire et un autre pour écrire. Par ailleurs les lectures sont destructives c'est à dire que les données lues par un processus disparaissent du tube.

  



## Changelog

 **04/08/2018 :** Ajout du README . 
 **10/09/2018 :** Ajout du  script shell RaspbianOSsetup.sh
 
> **Notes :**


> - Licence : **licence publique générale** ![enter image description here](https://img.shields.io/badge/licence-GPL-green.svg)
> - Auteur **Philippe SIMIER Lycée Touchard Le Mans**
>  ![enter image description here](https://img.shields.io/badge/built-passing-green.svg)
<!-- TOOLBOX 

Génération des badges : https://shields.io/
Génération de ce fichier : https://stackedit.io/editor#




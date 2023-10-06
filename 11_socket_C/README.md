# socket_C

L'objet de ce support est l'initiation au développement réseau sur les
sockets en langage C à partir du code le plus minimaliste. Dans
ce but, on utilise les fonctions réseau des bibliothèques standard du
Langage C et on se limite à l'utilisation d'adresses IPv4 en couche
réseau.

1  sudp2222.c   est un serveur UDP sur le port 2222 qui retourne l'inverse d'un entier.
   cudp2222.c   est le client UDP pour interroger le serveur UDP.
   Ce client Envoie un entier au serveur udp et affiche la valeur retournée
   si aucune réponse du serveur n'a été reçue au bout de deux secondes
   on  considére que le datagramme émis a été perdu.        

2  serveur_tcp.c est un mini serveur web TCP en C qui renvoie l'heure du serveur 
   dans une page html. 
   
3  serveur_web  est un serveur web en C qui répond aux requètes GET des clients.
   vous trouverez dans le répertoire un mini site web pour effectuer les tests.    

/***************************************************************************
 *   Auteur: philippe SIMIER BTS SNIR Touchard Le Mans    		   *
 *   philaure@wanadoo.fr  						   *
 *   Sujet : Mini serveur web en C (protocole http methode GET)		   *
 *         serveur parallèle (fork)  minimaliste 			   *
 *         pour la mise en oeuvre des sockets en C  			   *
 *         le processus père accepte les connexions sur la socket d'écoute *
 *         Pour chaque connexion acceptée, le processus père crée un       *
 *         processus fils qui hérite naturellement de la socket de service *
 *         ouverte par son père. le fils traite la requête du client	   *
 *         le père libéré du service client retourne immédiatement accepter*
 *         de nouvelles connexions sur la socket d'écoute		   *
 *         Dans ce cas de figure on a autant de fils créés que de          *
 *         connexions acceptés.						   *
 * 									   *
 *   Compilation :  gcc serveur_web_fork.c -o serveur_web_fork -Wall	   *
 *   Exécution   :  ./serveur_web_fork -p 8888 				   *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <ctype.h>

/* Variables globales */
char *ROOT;

/* fonction pour répondre  erreur 404 */
void envoie_404(int fdSocketClient, char *url)
{
  char chaine[512];
  printf(">> Erreur 404: Fichier non trouvé\n");
  write(fdSocketClient, "HTTP/1.1 404 Not found\r\n", 24);
  write(fdSocketClient, "Connection: close\r\n", 19);
  write(fdSocketClient, "Content-type: text/html\r\n", 25);
  write(fdSocketClient, "\r\n", 2);
  write(fdSocketClient, "<html><head><title>Pas trouvé !</title><meta charset=\"UTF-8\"></head>", 68);
  write(fdSocketClient, "<body><p>Désolé, le fichier demandé n'a pas été trouvé : ", 63);
  sprintf(chaine, "<tt>%s</tt> </body></html>\r\n", url );
  write(fdSocketClient, chaine, strlen(chaine));
}

/*  Cette fonction  remplace chaque séquence d'échappement
    présente dans l'URI encodée avec le caractère correspondant.
    %20 devient un espace etc                                 */

int decodeURL (char *sSource) {
    int nLength;
    char *sDest;
    char val1, val2;

    sDest = sSource;
    for (nLength = 0; *sSource; nLength++) {
        if (*sSource == '%' && isxdigit(sSource[1]) && isxdigit(sSource[2])) {

            if (sSource[1] <= '9') val1 = sSource[1]-'0'; else
                if (sSource[1] <= 'F') val1 = 10 + sSource[1]-'A';
                        else val1 = 10 + sSource[1]-'a';
            if (sSource[2] <= '9') val2 = sSource[2]-'0'; else
                if (sSource[2] <= 'F') val2 = 10 + sSource[2]-'A';
                        else val2 = 10 + sSource[2]-'a';
            sDest[nLength] = 16 * val1 + val2;
            sSource += 3;
            continue;
        }
        sDest[nLength] = *sSource++;
    }
    sDest[nLength] = '\0';
    return nLength;
}

/* fonction qui remplit le service  (renvoie le fichier demandé au client) */
void service(int fdSocketClient, struct sockaddr_in adresseClient){

    char requete[512];  // la requete du client
    char path[512];     // le chemin d'accès du fichier demandé
    char all[1024];     // le buffer de lecture du fichier à envoyer
    int  retour;
    char *reqline[3];   // la requète du client découpée
    int fd;		// le file descriptor du fichier à envoyer
    struct stat sb;     // structure qui contiendra les informations sur le fichier demandé
    char str[255];

    memset(requete, '\0', 512);
    memset(all, '\0', 1024);
    retour=read(fdSocketClient,requete,512);
        if (retour == -1)
        {
            printf("pb lecture requete client : %s\n", strerror(errno));
            exit(1);
        }
    printf(">>  Requete du client %s/%d\n%s\n", inet_ntoa(adresseClient.sin_addr), adresseClient.sin_port, requete);

    reqline[0] = strtok (requete, " \t\n");       // découpage de la requéte du client suivant les espaces blancs
        if ( strncmp(reqline[0], "GET\0", 4)==0 ) // si la requète commence par GET
        {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");  // uniquement les 3 premiers mots de la requete client
            if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
            {
                write(fdSocketClient, "HTTP/1.0 400 Bad Request \nContent-Length: 0\nContent-type: text/html\n\n", 69);  // c'est pas le bon protocole
            }
            else
            {   decodeURL(reqline[1]);  // décode l'URL 
		int len = strlen(reqline[1]);
                if ( strncmp(reqline[1]+len-1, "/", 1)==0 ) //si le fichier n'est pas indiqué, index.html sera ouvert
                    strcat(reqline[1], "index.html");  //si le fichier n'est pas indiqué, index.html sera ouvert

                strcpy(path, ROOT);
                strcpy(&path[strlen(ROOT)], reqline[1]);
                printf("file: %s\n", path);

                if ( (fd=open(path, O_RDONLY))!=-1 )    //Fichier trouvé
                {
                    printf("ouverture file \n");
                    // lecture longueur du fichier
	            if (stat(path, &sb) == -1) {
               		perror("stat");
               		exit(EXIT_FAILURE);
           	    }
                    sprintf(str, "HTTP/1.1 200 OK\nContent-Length: %d\n\n", (int) sb.st_size );
                    write( fdSocketClient, str, strlen(str));
                    while ( (retour = read(fd, all, 1024))>0 )
                        write( fdSocketClient, all, retour);
                }
                else
		    envoie_404(fdSocketClient, reqline[1]);

            }
        }

    // CLOSE
    shutdown (fdSocketClient, SHUT_RDWR);
    close(fdSocketClient);
    //sleep(60); à décommenter pour simuler un temps de réponse
}

int main(int argc, char *argv[])
{
    int fdSocketServeur;
    int fdSocketClient;

    struct sockaddr_in adresseServeur;
    struct sockaddr_in adresseClient;
    int retour,c, pid = 0;
    socklen_t tailleClient;
    int serverPort = 8888; // port écouté par défaut

    ROOT = getenv("PWD"); // répertoire par défaut où seront lu les fichiers demandés

    while ((c = getopt (argc, argv, "p:r:")) != -1)
        switch (c)
	{
	    case 'r':
                ROOT = malloc(strlen(optarg));
                strcpy( ROOT ,optarg);
                break;

            case 'p':
                serverPort   = atoi(optarg);  // conversion en entier
                break;
            default:
		printf("usage: ./serveur_web_fork [-p port] \n");
                exit(1);
        }

    printf("Serveur web démarré sur le port no. %s%d%s\n", "\033[92m", serverPort, "\033[0m");
    printf("Repertoire racine  \033[92m%s\033[0m\n", ROOT);


    fdSocketServeur = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // AF_INET désigne la famille de protocole de couche réseau IPv4.
    if (fdSocketServeur == -1)
	{
		printf("pb socket : %s\n", strerror(errno));
		exit(0);
	}

    adresseServeur.sin_family = AF_INET;  // AF_INET désigne la famille de protocole de couche réseau IPv4.
    adresseServeur.sin_port = htons(serverPort);
    adresseServeur.sin_addr.s_addr = htonl(INADDR_ANY);
    retour = bind(fdSocketServeur, (struct sockaddr*)&adresseServeur, sizeof(adresseServeur));  //attachement ip-port
    if (retour == -1)
	{
		printf("pb bind : %s\n", strerror(errno));
		exit(0);
	}

    retour=listen(fdSocketServeur,10);	//10 clients maxi en file d'attente
    if (retour == -1)
	{
		printf("pb listen : %s\n", strerror(errno));
		exit(0);
	}
    tailleClient = sizeof(adresseClient);
    signal( SIGCHLD, SIG_IGN); 
    // le père ignore le signal mort du fils cela permet de détruire automatiquement les zombies
    while(1){

        printf("Attente de requête sur le port %d\n", serverPort);
    	 // ACCEPT acceptation de la connexion client et creation de la socket de communication
        fdSocketClient=accept(fdSocketServeur,(struct sockaddr *) &adresseClient, &tailleClient);
	// création du fils pour éxécuter la fonction de service (construction de la réponse)
	if ((pid = fork()) == -1){
	    printf("erreur de création du processus fils");
	}
	else {
	    if (pid == 0){ // je suis le fils je traite le service
            	service( fdSocketClient, adresseClient);
	    	return 0;
	    }
	}
    }

    return EXIT_SUCCESS;
}


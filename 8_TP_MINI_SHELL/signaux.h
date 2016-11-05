#ifndef SIGNAUX_H_INCLUDED
#define SIGNAUX_H_INCLUDED

char *listeSignaux[33]={"",
			"Instruction (HANG UP) - Fin de session",
                        "Interruption (Frappe de CTRL C",
                        "Instruction (QUIT)",
                        "Instruction illégale",
                        "Point d'arrêt",
                        "Terminaison anormale",
                        "Erreur de bus",
                        "Erreur mathématique",
			"Terminaison forcée (Kill)",
			"Signal utilisateur 1",
			"Référence mémoire invalide",
			"Signal utilisateur 2",
			"Erreur PIPE sans lecteur",
			"Fin de temporisation",
			"Terminaison du processus",
			"Erreur de pile",
			"Processus fils terminé",
			"Reprise de l'exécution du processus",
			"Arrêt de l'exécution du processus",
			"Demande de suspension (Ctrl Z)",
			"Lecture en arrière plan",
			"Ecriture en arrière plan",
			"Evènement urgent sur une socket",
			"Temps maximum CPU écoulé",
			"Taille maximale de fichier atteinte",
			"Alarme horloge virtuelle",
			"Alarme du profileur",
			"Changement de taille de fenêtre",
			"Occurence d'un évènement attendu",
			"I/O possible actuellement",
			"Chute d'alimentation",
			"Non utilisé"
                        };


#endif // SIGNAUX_H_INCLUDED

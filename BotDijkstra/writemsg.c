#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "writemsg.h"

typedef struct {
    long type;
	char texte[256];
} message_t;


//Ecriture dans la pile de message. En entrée : le fichier de clé, le message, l'id du canal
void writePile(char* keyFile, char* msg, char* id)
{
	key_t     key;
	message_t message;
	int       file;
	if ((key = ftok(keyFile, 0)) == -1) {
		perror("ftok, création clé écriture MSG");
		exit(EXIT_FAILURE);
	}
	if ((sscanf(id, "%ld", & (message.type)) != 1) || (message.type <= 0)) {
		fprintf(stderr, "Type de message invalide");
		exit(EXIT_FAILURE);
	}
	strncpy(message.texte, msg, 255);
	if ((file = msgget(key, IPC_CREAT | 0600)) == -1) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	if (msgsnd(file, (void *) & message, 256, 0) <0) {
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}
	fprintf(stdout,"Envoyé : %s\n", msg);
}

//Lecture de la pile, en entrée : le résultat renvoyé, le fichier de clé, l'id du canal à lire
char *readPile(char result[], char* keyFile, char* id)
{
	key_t     key;
	message_t message;
	int       file;
	long      type;
	if ((key = ftok(keyFile, 0)) == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}
	if (sscanf(id, "%ld", & type) != 1) {
		fprintf(stderr, "Type invalide");
		exit(EXIT_FAILURE);
	}
	if ((file = msgget(key, IPC_CREAT | 0600)) == -1) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}
	if (msgrcv(file, (void *) & message, 256, type, 0) >= 0)
		fprintf(stdout, "Reçut (%ld) %s\n", message.type, message.texte);
	else
		perror("msgrcv2");

    strcpy(result, message.texte);
	return result;
}

//Fonction d'attente des bots utilisée par le programme principale
void waitBot(int numBot, char* keyFile){
    char result[256];

    //on attend de recevoir les messages de tous les bots
    printf("On attend les bots\n");
    int nbBot = numBot;
	char* id = malloc(10 * sizeof(int));

    //pour chaque bot, on attend qu'ils soient ready
    for(int i = 1; i <= nbBot; i++){
        sprintf(id,"%d",i);
        readPile(result, "./keyfile", id);
        if(strcmp(result,"Ready") == 0){
            printf("Bot %d prêt\n", i);
            continue;

        }
    }
    id = malloc(10 * sizeof(int));
    char* msg = malloc(10 * sizeof(int));
    //chaque bot est ready !!
    for(int i = 1; i <= nbBot; i++){
	sprintf(id,"%d",i);
	sprintf(msg,"Go,%d",(10*i)); //on envoie sur le channel 10 et 20 (bot 1 et 2)
        writePile(keyFile, msg, id);
    }
    printf("Wait\n");


	id = malloc(10 * sizeof(int));
	int maxBot = 1;

	//pour chaque bot, on lit sur son canal et écrit sur le canal+1
	while(1){
		for(int i = maxBot; i <= nbBot; i++){
			sprintf(id,"%d",(10*i));
			readPile(result, keyFile, id);

			//condition stop
			if(strcmp(result,"Stop") == 0){
				if(i == 1) maxBot++;
				else if (i==2) nbBot--;
				printf("max %d nb %d\n", maxBot,nbBot);
			}
		}
		if(maxBot > nbBot){
			break;
		}

		for(int i = maxBot; i <= nbBot; i++){
			sprintf(id,"%d",(10*i)+ 1);
			writePile(keyFile, "Go", id);
		}
	}

    return (1);

}

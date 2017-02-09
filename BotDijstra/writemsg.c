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
	fprintf(stdout,"Message %s envoyé ! \n", id);
}

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
		fprintf(stdout, "Message (%ld) %s \n", message.type, message.texte);
	else
		perror("msgrcv2");

    strcpy(result, message.texte);
	return result;
}

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
	sprintf(msg,"Go,%d",(10*i)); //on envoie sur le channel 11 et 12 (bot 1 et 2)
        writePile(keyFile, msg, id);
    }
    printf("Wait\n");
	//waitBots(10);
	//waitBots(20);
	id = malloc(10 * sizeof(int));
	int maxBot = 1;
	while(1){
		for(int i = maxBot; i <= nbBot; i++){
			sprintf(id,"%d",(10*i));
			readPile(result, keyFile, id);
			//printf("reçut time %f\n", (double) clock());
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

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

void waitBot(int nbBot, char* keyFile){
    char result[256];

    //on attend de recevoir les messages de tous les bots
    for(int i=1; i<= nbBot; i++){
        readPile(result, keyFile, i);
    }

    //on envoie un message à chaque bot avec un id d'écriture spécifique


}

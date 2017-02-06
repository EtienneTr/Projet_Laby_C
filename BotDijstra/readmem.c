#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "readmem.h"

//clé en entrée
int *read(key_t key)
{
    int shmid;
    int *data = NULL;

    //accès à la mémoire partagée de la clé + récupération du data
    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    data = shmat(shmid, NULL, 0);
    if ( data == (void *)-1 ) {
        perror("shmat error");
        exit(EXIT_FAILURE);
    }

    return (data);
}

int getTabs(char* keyFile){

    key_t key1, key2, key3;
    int *dataGl = NULL, *dataH = NULL, *dataV = NULL;
    //génération des clés
    key1 = ftok(keyFile, 0);
    key2 = ftok(keyFile, 1);
    key3 = ftok(keyFile, 2);

    dataGl = read(key1);
    dataH = read(key2);
    dataV = read(key3);

    //construction des tableaux

    debutX = dataGl[0];
    debutY = dataGl[1];
    finX = 4;// dataGl[2];
    finY = 1;//dataGl[3];


    createTabsH(dataH);//horizontal
    createTabsV(dataV);//vertical

    printf("Fin création tableaux \n");

}

void createTabsH(int* data){

    int HE = data[0];//Horizontal
    int VE = data[1];//Vertical

    //malloc tableau
	mursH = malloc(HE * sizeof(*mursH));

	for(int i=0 ; i < HE ; i++){
	     mursH[i] = malloc(VE * sizeof(**mursH));
	}

    int index = data[2]; //index début des valeurs
    printf("index %d\n", index);
    for(int i=0; i < HE; i++){
        for(int j=0; j< VE; j++){
            mursH[i][j] = data[index];
            index++;
        }
    }

	for(int i=0; i < HE; i++){
        for(int j=0; j< VE; j++){
            printf("%d ",mursH[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");

}

void createTabsV(int* data){

    int HE = data[0];
    int VE = data[1];

    //malloc tableau
	mursV = malloc(HE * sizeof(*mursV));

	for(int i=0 ; i < HE ; i++){
	     mursV[i] = malloc(VE * sizeof(**mursV));
	}

    int index = data[2]; //index début des valeurs
    printf("index %d\n", index);
    for(int i=0; i < HE; i++){
        for(int j=0; j< VE; j++){
            mursV[i][j] = data[index];
            index++;
        }
    }

	for(int i=0; i < HE; i++){
        for(int j=0; j< VE; j++){
            printf("%d ",mursV[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");

}

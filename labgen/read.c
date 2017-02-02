#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

int read(key_t key) 
{
    int shmid;
    int *data = NULL;

    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    data = shmat(shmid, NULL, 0);
    if ( data == (void *)-1 ) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    /*Affichage du contenu de la memoire*/
    int i = 0;
    while(data[i]!=-1)
    {
        printf("   data[%i]: %i \n", i, data[i]);
        i++;
    }
    printf("_______________\n");
    
    return (1);
}

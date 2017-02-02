#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

int clear(key_t key) 
{
    int shmid;
    char *data = NULL;

    //key = ftok("./keyfile", 0);
    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    data = shmat(shmid, NULL, 0);
    if ( data == (void *)-1 ) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    shmctl(shmid, IPC_RMID, NULL);
    
    return EXIT_SUCCESS;
}
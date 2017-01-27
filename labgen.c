/*RBG*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

void initarray(int rows, int cols, int array[rows][cols]);

int main(int argc, char** argv) 
{
    key_t key;
    int shmid;
    int *data = NULL;
    
    key = 1101;
    shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    data = shmat(shmid, NULL, 0);
    if ( data == (void *)-1 ) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    /*Initialisation du tableau de test*/
    const int ROWS = 3;
    const int COLS = 2;
    int array[ROWS][COLS];
    initarray(ROWS, COLS, array);
    
    data[0] = 1;//x_entree
    data[1] = 1;//y_entree
    data[2] = 1;//x_sortie
    data[3] = 1;//y_sortie
    data[4] = ROWS;
    data[5] = COLS;
    
    const int dataoffset = 6;//1er index utilisable
    int i, j;
    int rowindex;
    for( i=0; i<ROWS; i++ )
    {
        rowindex = ROWS + COLS*i + 0 + dataoffset;
        data[i+dataoffset] = rowindex;
        //printf("data[%i] = %i\n", i+dataoffset, data[i+dataoffset]);
        for( j=0; j<COLS; j++ )
        {
            data[rowindex+j] = array[i][j];
            //printf("data[%i] = array[%i][%i] -> %i\n", rowindex+j, i, j, data[rowindex+j]);
        }
    }
    data[rowindex+COLS+1] = '\0';
    
    /*Affichage du contenu de la memoire*/
    i = dataoffset;
    while(data[i]!='\0')
    {
        printf("data[%i]: %i \n", i, data[i]);
        if(i==dataoffset+ROWS-1) printf("------------\n");
        i++;
    }
    
    return (1);
}

/*Initialise le tableau de 11 a ...*/
void initarray(int rows, int cols, int array[rows][cols])
{
    int i, j;
    int c = 11;
    for ( i=0; i<rows; i++ )
    {
        for ( j=0; j<cols; j++ )
        {
             array[i][j] = c;
             c++;
        }
    }
}
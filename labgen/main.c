#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "read.h"
#include "clear.h"

#define ROWS 4
#define COLS 4
#define ROWSH ROWS+1
#define COLSV COLS+1

void initarray(int rows, int cols, int array[rows][cols], int minv);
void inlineArray(int rows, int cols, int sourceArray[rows][cols], int flatArray[]);
//TODO: fonction qui prend le tableau a une dimension et le met en deux

int main(int argc, char** argv) 
{
    system("clear"); printf("==================\n");
    
    //TODO: gerer les erreurs
    key_t key1, key2, key3;
    int shmid1, shmid2, shmid3;
    int *data1 = NULL, *data2 = NULL, *data3 = NULL;
    
    key1 = ftok("./keyfile", 0);
    key2 = ftok("./keyfile", 1);
    key3 = ftok("./keyfile", 2);
    
    clear(key1); 
    clear(key2); 
    clear(key3);
    
    //TODO: mieux assigner la memoire
    shmid1 = shmget(key1, 1024, 0666 | IPC_CREAT);
    shmid2 = shmget(key2, 1024, 0666 | IPC_CREAT);
    shmid3 = shmget(key3, 1024, 0666 | IPC_CREAT);
    
    data1 = shmat(shmid1, NULL, 0);
    data2 = shmat(shmid2, NULL, 0);
    data3 = shmat(shmid3, NULL, 0);
    
    data1[0] = 404;//x_entree
    data1[1] = 404;//y_entree
    data1[2] = 404;//x_sortie
    data1[3] = 404;//y_sortie
    data1[4] = -1;
    
    /*Matrice horizontaux*/
    int math[ROWSH][COLS] = {
        {1, 1, 1, 1},
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {1, 1, 0, 1},
        {1, 0, 1, 1}
    };
    inlineArray(ROWSH, COLS, math, data2);
    
    /*Matrice verticaux*/
    int matv[ROWS][COLSV] = {
        {0, 0, 1, 0, 1},
        {1, 0, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 0, 0, 0, 1}
    };
    inlineArray(ROWS, COLSV, matv, data3);
    
    printf("1:\n"); read(key1);
    printf("2:\n"); read(key2);
    printf("3:\n"); read(key3);
    
    return (1);
}

/* Prend un tableau a deux dimensions et le met en une */
void inlineArray(int rows, int cols, int sourceArray[rows][cols], int flatArray[])
{
    flatArray[0] = rows;
    flatArray[1] = cols;
    
    int offset = 2;//1er index utilisable
    int i, j;
    int rowindex = offset;
    for( i=0; i<rows; i++ )
    {
        flatArray[i+offset] = rowindex;
        //printf("flatArray[%i] = %i\n", i+offset, flatArray[i+offset]);
        for( j=0; j<cols; j++ )
        {
            flatArray[rowindex+j] = sourceArray[i][j];
            //printf("data[%i] = sourceArray[%i][%i] -> %i\n", rowindex+j, i, j, flatArray[rowindex+j]);
        }
        rowindex += cols;
    }
    flatArray[rowindex+cols] = -1;
}

/*Initialise le tableau de minv a ...*/
void initarray(int rows, int cols, int array[rows][cols], int minv)
{
    int i, j;
    minv = 0;
    for ( i=0; i<rows; i++ )
    {
        for ( j=0; j<cols; j++ )
        {
             array[i][j] = minv;
             minv++;
        }
    }
    array[rows-1][cols] = -1;
}
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include "functions.h"

int **array;
int **math;
int **matv;

int rows = 5;
int cols = 5;

void tabinit();
void labgen();

int main(int argc, char** argv) 
{
    system("clear");
    printf("==START==\n");
    
    labgen();
    //displayarray(rows, cols, array);printf("\n");
    //displayarray(rows+1, cols, math);printf("\n");
    //displayarray(rows, cols+1, matv);printf("\n");
    
    
    key_t key1, key2;
    int shmid1, shmid2;
    int sizeToAllocH = (((rows)*(cols+1))+2) * sizeof(int);
    int sizeToAllocV = (((rows+1)*(cols))+2) * sizeof(int);
    
    key1 = ftok("./keyfile", 0);
    key2 = ftok("./keyfile", 1);
    
    clearshm(key1, sizeToAllocH);
    clearshm(key2, sizeToAllocV);
    
    shmid1 = shmget(key1, sizeToAllocH, 0666 | IPC_CREAT);
    if(shmid1 < 0){
        printf("shmid1 error.");
    }
    shmid2 = shmget(key2, sizeToAllocV, 0666 | IPC_CREAT);
    if(shmid2 < 0){
        printf("shmid2 error.");
    }
    
    int *data1 = shmat(shmid1, NULL, 0);
    int *data2 = shmat(shmid2, NULL, 0);
    
    flattenArray(rows+1, cols, math, data1);
    flattenArray(rows, cols+1, matv, data2);
    
    
    //displayarray(rows+1, cols, math);
    //readshm(key1, sizeToAllocH);
    
    //displayarray(rows, cols+1, matv);
    //readshm(key2, sizeToAllocV);
    
    return (1);
}

void tabinit()
{
    /* Initialise les tableaux*/
    int i;
    array = malloc(rows * sizeof(*array));
    for(int i=0 ; i < rows ; i++){
         array[i] = malloc(cols * sizeof(**array));
    }
    initarray(rows, cols, array, 0, 1);
    
    //Murs horizontaux
    math = malloc(sizeof *math * rows+1);
    for(int i=0 ; i < rows+1 ; i++){
         math[i] = malloc(sizeof *math[i] * cols);
    }
    initarray(rows+1, cols, math, 1, 0);
    
    //Murs verticaux
    matv = malloc(sizeof *matv * rows);
    for(int i=0 ; i < rows ; i++){
         matv[i] = malloc(sizeof *matv[i] * cols+1);
    }
    initarray(rows, cols+1, matv, 1, 0);
}

void labgen()
{
    srand(time(NULL));
    
    tabinit();
    
    int count = 0;
    while( isLabFinished(rows, cols, array, 0) != 1 )
    {
        count++;
        
        /* Choix d'une cell */
        int rrow1 = randmax(rows);
        int rcol1 = randmax(cols);
        int rrow2 = rrow1;
        int rcol2 = rcol1;
        
        /* Choix d'une celle adjacente a la première */
        int bool = -1;
        while( bool != 1 )
        {
            switch( randmax(4) )
            {
                case 0:
                    if(rrow2+1>=rows)
                        break;
                    else
                    {
                        rrow2++;
                        bool = 1;
                    }
                    break;
                case 1:
                    if(rrow2-1<0)
                        break;
                    else
                    {
                        rrow2--;
                        bool = 1;
                    }
                    break;
                case 2:
                    if(rcol2+1>=cols)
                        break;
                    else
                    {
                        rcol2++;
                        bool = 1;
                    }
                    break;
                case 3:
                    if(rcol2-1<0)
                        break;
                    else
                    {
                        rcol2--;
                        bool = 1;
                    }
                    break;
            }
        }
        
        /* Logique de creation du lab */
        /*printf("%i: rrow1:%i > %i\n", count, rrow1, array[rrow1][rcol1]);
        printf("   rcol1:%i\n", rcol1);
        printf("   rrow2:%i > %i\n", rrow2, array[rrow2][rcol2]);
        printf("   rcol2:%i\n", rcol2);*/
        int smallv, bigv;
        if( array[rrow1][rcol1] == array[rrow2][rcol2] )
        {
            continue;
        }
        else if( array[rrow1][rcol1] < array[rrow2][rcol2] )
        {
            smallv = array[rrow1][rcol1];
            bigv = array[rrow2][rcol2];
            floodfill(rows, cols, array, rrow2, rcol2, smallv, bigv);
        }
        else
        {
            smallv = array[rrow2][rcol2];
            bigv = array[rrow1][rcol1];
            floodfill(rows, cols, array, rrow1, rcol1, smallv, bigv);
        }
        
        /* Gestions matrices de murs */
        if ( rrow1 == rrow2 )
        {//Même ligne
            if( rcol1 < rcol2 )
            {
                matv[rrow1][rcol2] = 0;
            }
            else
            {
                matv[rrow1][rcol1] = 0;
            }
        }
        if ( rcol1 == rcol2 )
        {//Même colonne
            if( rrow1 < rrow2 )
            {
                math[rrow2][rcol1] = 0;
            }
            else
            {
                math[rrow1][rcol1] = 0;
            }
        }
    }
    createEntreeSortie(rows+1, cols, math, rows, cols+1, matv);
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>

void displayarray(int rows, int cols, int **array)
{
    int i, j;
    
    for ( i=0; i<rows; i++ )
    {
        for ( j=0; j<cols; j++ )
        {
             printf("%i ", array[i][j]);
        }
        printf("\n");
    }
}

void initarray(int rows, int cols, int **array, int minv, int increment)
{
    int i, j;
    for ( i=0; i<rows; i++ )
    {
        for ( j=0; j<cols; j++ )
        {
            array[i][j] = minv;
            minv+=increment;
        }
    }
}

void initmatmurs(int rows, int cols, int **array)
{
    int i, j;
    for ( i=0; i<rows; i++ )
    {
        for ( j=0; j<cols; j++ )
        {
             array[i][j] = 1;
        }
    }
}

int randmax(int max)
{
    int randomnumber;
    randomnumber = rand() % max;
    return randomnumber;
}

int isLabFinished(int rows, int cols, int **array, int minv)
{
    int i, j;
    for ( i=0; i<rows; i++)
    {
        for ( j=0; j<cols; j++)
        {
            if( array[i][j]  != minv )
            {
                return -1;
            }
        }
    }
    return 1;
}

void floodfill(int rows, int cols, int **array, int row, int col, int smallv, int bigv)
{
    if(row<0 || col<0 || row>=rows || col>=cols){
        return;
    }
    else if( array[row][col] != bigv ){
        return;
    } 
    else 
    {
        array[row][col] = smallv;
        floodfill(rows, cols, array, row,   col+1, smallv, bigv);
        floodfill(rows, cols, array, row+1, col,   smallv, bigv);
        floodfill(rows, cols, array, row,   col-1, smallv, bigv);
        floodfill(rows, cols, array, row-1, col,   smallv, bigv);
        return;
    }
}

void createEntreeSortie(int rowsh, int colsh, int **arrayh, int rowsv, int colsv, int **arrayv)
{
    int r;
    /* ouverture murs horizontaux */
    r = randmax(2);
    if( r == 0 )
    {
        arrayh[0][randmax(colsh)] = 0;
    }
    else
    {
        arrayh[rowsh-1][randmax(colsh)] = 0;
    }
    /* ouverture murs verticaux */
    r = randmax(2);
    if( r == 0 )
    {
        arrayv[randmax(rowsv)][0] = 0;
    }
    else
    {
        arrayv[randmax(rowsv)][colsv-1] = 0;
    }
}

void flattenArray(int rows, int cols, int **sourceArray, int *flatArray)
{
    flatArray[0] = rows;
    flatArray[1] = cols;
    int offset = 2; //1er index utilisable
    
    int i, j, k;
    k = offset;
    for ( i=0; i<rows; i++ )
    {
        for ( j=0; j<cols; j++ )
        {
            flatArray[k] = sourceArray[i][j];
            k++;
        }
    }
}

int clearshm(key_t key, int size) 
{
    int shmid;
    char *data = NULL;

    //key = ftok("./keyfile", 0);
    shmid = shmget(key, size, 0666 | IPC_CREAT);
    data = shmat(shmid, NULL, 0);
    if ( data == (void *)-1 ) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    shmctl(shmid, IPC_RMID, NULL);
    
    return EXIT_SUCCESS;
}

int readshm(key_t key, int size) 
{
    int shmid;
    int *data = NULL;

    shmid = shmget(key, size, 0666 | IPC_CREAT);
    data = shmat(shmid, NULL, 0);
    if ( data == (void *)-1 ) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    
    /*Affichage du contenu de la memoire*/
    int i = 0;
    for (i=0; i < size/sizeof(int); i++)
    {
        printf("   data[%i]: %i \n", i, data[i]);
        
    }
    printf("_______________\n");
    
    return (1);
}
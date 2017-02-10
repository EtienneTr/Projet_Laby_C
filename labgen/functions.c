#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>

/*Affiche un tableau, pour debug*/
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

/*Initialise un tableau de minv à ... avec un increment*/
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

/*Return un int entre 0 et max-1*/
int randmax(int max)
{
    int randomnumber;
    randomnumber = rand() % max;
    return randomnumber;
}

/*Return 1 si toutes les cases du tableau sont a minv*/
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

/*Check les cases adjacentes et change leurs valeurs si elles ont la même que bigv par recursivité*/
void floodfill(int rows, int cols, int **array, int row, int col, int smallv, int bigv)
{
	/*Condition d'arret: la case est hors limites du tableau*/
    if(row<0 || col<0 || row>=rows || col>=cols){
        return;
    }
	/*Condition d'arret: la case n'a pas la valeur que l'on veux modifier*/
    else if( array[row][col] != bigv ){
        return;
    } 
    else 
    {
		/*On change la valeur de la case est appele la fonction sur les 4 cases adjacentes*/
        array[row][col] = smallv;
        floodfill(rows, cols, array, row,   col+1, smallv, bigv);
        floodfill(rows, cols, array, row+1, col,   smallv, bigv);
        floodfill(rows, cols, array, row,   col-1, smallv, bigv);
        floodfill(rows, cols, array, row-1, col,   smallv, bigv);
        return;
    }
}

/*Casses 2 murs exterieur pour faire une entree et sortie*/
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

/*Prend un tableau a 2 dimensions sourceArray et le met en 1 dimension dans flatArray*/
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

/*Clear le segment memoire*/
int clearshm(key_t key, int size) 
{
    int shmid;
    char *data = NULL;

    shmid = shmget(key, size, 0666 | IPC_CREAT);
    data = shmat(shmid, NULL, 0);
    if ( data == (void *)-1 ) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    shmctl(shmid, IPC_RMID, NULL);
    
    return EXIT_SUCCESS;
}

/*Lit le segment de memoire*/
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

/*Trouve le mur ouvert sur la matrice horizontal et renvoie les coordonnees dans r_row et r_col*/
int getOpenWallH(int rows, int cols, int **array, int *r_row, int *r_col)
{
    int i;
    for ( i=0; i<cols; i++ )
    {
        if( array[0][i]==0 )
        {
            *r_row = -1;
            *r_col = i;
        }
        
        if( array[rows-1][i]==0 )
        {
            *r_row = rows-1;
            *r_col = i;
        }
    }
}

/*Trouve le mur ouvert sur la matrice vertical et renvoie les coordonnees dans r_row et r_col*/
int getOpenWallV(int rows, int cols, int **array, int *r_row, int *r_col)
{
    int i;
    for ( i=0; i<rows; i++ )
    {
        if( array[i][0]==0 )
        {
            *r_row = i;
            *r_col = -1;
        }
        
        if( array[i][cols-1]==0 )
        {
            *r_row = i;
            *r_col = cols-1;
        }
    }
}
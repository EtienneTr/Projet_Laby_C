#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/shm.h>
#include "functions.h"
#include "writemsg.h"

int **array;
int **math;
int **matv;

void tabinit(int rows, int cols);
void labgen(int rows, int cols);

int main(int argc, char** argv)
{
    system("clear");
    printf("==START==\n");
    
    int nbbot;
    printf("Numéro du bot: ");
    scanf("%d", &nbbot);
    printf("Bot num : %d\n", nbbot);
    
    /*On recup le nb de lignes et colonnes*/
    int rows, cols;
    printf("Nombre de lignes du labyrinthe: ");
    scanf("%d", &rows);
    printf("Nombre de colonnes du labyrinthe: ");
    scanf("%d", &cols);

    /*Generation du labyrinthe*/
    labgen(rows, cols);
    /*Affichage pour debug*/
    //displayarray(rows, cols, array);printf("\n");
    //displayarray(rows+1, cols, math);printf("\n");
    //displayarray(rows, cols+1, matv);printf("\n");
    
    /*Initialisation des segments de memoire partagés*/
    key_t key0, key1, key2;
    int shmid0, shmid1, shmid2;
    int sizeToAlloc0 = 4*sizeof(int);
    int sizeToAllocH = (((rows)*(cols+1))+2) * sizeof(int);
    int sizeToAllocV = (((rows+1)*(cols))+2) * sizeof(int);
    char* keyfile = "./keyfile";
    key0 = ftok(keyfile, 0);
    key1 = ftok(keyfile, 1);
    key2 = ftok(keyfile, 2);
    
    shmid0 = shmget(key0, sizeToAlloc0, 0666 | IPC_CREAT);
    if(shmid0 < 0){
        printf("shmid0 error.");
    }
    shmid1 = shmget(key1, sizeToAllocH, 0666 | IPC_CREAT);
    if(shmid1 < 0){
        printf("shmid1 error.");
    }
    shmid2 = shmget(key2, sizeToAllocV, 0666 | IPC_CREAT);
    if(shmid2 < 0){
        printf("shmid2 error.");
    }
    
    /*0 pour info en vrac*/
    int *data0 = shmat(shmid0, NULL, 0);
    /*1 pour la matrice de mur horizontaux*/
    int *data1 = shmat(shmid1, NULL, 0);
    /*2 pour la matrice de mur verticaux*/
    int *data2 = shmat(shmid2, NULL, 0);
    
    /* Data0 */
    data0[0] = rows;
    data0[1] = cols;
    
    int entree_row, entree_col;
    getOpenWallH(rows+1, cols, math, &entree_row, &entree_col);
    data0[2] = entree_row;
    data0[3] = entree_col;
    
    int sortie_row, sortie_col;
    getOpenWallV(rows, cols+1, matv, &sortie_row, &sortie_col);
    data0[4] = sortie_row;
    data0[5] = sortie_col;
    
    /* Data1 */
    flattenArray(rows+1, cols, math, data1);
    /* Data2 */
    flattenArray(rows, cols+1, matv, data2);
    
    /*Lecture memoire pour debug*/
    //readshm(key0, sizeToAlloc0);
    //readshm(key1, sizeToAllocH);
    //readshm(key2, sizeToAllocV);
    //displayarray(rows+1, cols, math);
    //displayarray(rows, cols+1, matv);
    
    /*Gestion des bots*/
    waitBot(nbbot, keyfile);
    
    /*Encore du debug*/
    //displayarray(rows+1, cols, math);
    //readshm(key1, sizeToAllocH);
    //printf("%i, %i\n", data0[2], data0[3]);
    
    //displayarray(rows, cols+1, matv);
    //readshm(key2, sizeToAllocV);
    //printf("%i, %i\n", data0[4], data0[5]);
    
    clearshm(key0, sizeToAlloc0);
    clearshm(key1, sizeToAllocH);
    clearshm(key2, sizeToAllocV);
    
    return (1);
}

void tabinit(int rows, int cols)
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

void labgen(int rows, int cols)
{
    srand(time(NULL));
    
    tabinit(rows, cols);
    
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
        
        /*Some debug*/
        //printf("%i: rrow1:%i > %i\n", count, rrow1, array[rrow1][rcol1]);
        //printf("   rcol1:%i\n", rcol1);
        //printf("   rrow2:%i > %i\n", rrow2, array[rrow2][rcol2]);
        //printf("   rcol2:%i\n", rcol2);
        
        /* Coeur de la logique de creation du lab */
        int smallv, bigv;
        if( array[rrow1][rcol1] == array[rrow2][rcol2] )
        {
            /*Si les deux cases ont la même valeur on boucle et rechoisi deux cases*/
            continue;
        }
        /*Sinon on gere les deux autre cas possible*/
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
    /*On casse deux murs exterieur pour faire une entrée et une sortie*/
    createEntreeSortie(rows+1, cols, math, rows, cols+1, matv);
}

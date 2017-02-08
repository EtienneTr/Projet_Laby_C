#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <time.h>

void displayarray(int rows, int cols, int **array);

void initarray(int rows, int cols, int **array, int minv, int increment);

void initmatmurs(int rows, int cols, int **array);

int randmax(int max);

int isLabFinished(int rows, int cols, int **array, int minv);

void floodfill(int rows, int cols, int array[rows][cols], int row, int col, int smallv, int bigv);

void createEntreeSortie(int rowsh, int colsh, int **arrayh, int rowsv, int colsv, int **arrayv);

void flattenArray(int rows, int cols, int **sourceArray, int *flatArray);

int clearshm(key_t key, int size);

int readshm(key_t key, int size);

int getOpenWallH(int rows, int cols, int **array, int *r_row, int *r_col);

int getOpenWallV(int rows, int cols, int **array, int *r_row, int *r_col);
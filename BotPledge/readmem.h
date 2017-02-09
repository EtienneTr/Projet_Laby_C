#ifndef READMEM_H_INCLUDED
#define READMEM_H_INCLUDED

extern int **mursH;
extern int **mursV;
extern int debutX;
extern int debutY;
extern int finX;
extern int finY;
extern int rows;
extern int cols;

int *read(key_t key, int taille);
int getTabs(char* keyFile);
void createTabsH(int* data);
void createTabsV(int* data);


#endif // READMEM_H_INCLUDED

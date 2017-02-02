#ifndef READMEM_H_INCLUDED
#define READMEM_H_INCLUDED

extern int **mursH;
extern int **mursV;
extern int debutX;
extern int debutY;
extern int finX;
extern int finY;

int *read(key_t key);
int getTabs();
void createTabsH(int* data);
void createTabsV(int* data);


#endif // READMEM_H_INCLUDED

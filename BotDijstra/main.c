#include <stdio.h>
#include <stdlib.h>

int mursH[5][4] = {{1,1,1,1}, //matrice mur horizontal
                  {1,0,0,0},
                  {0,1,0,0},
                  {1,1,0,1},
                  {1,0,1,1}};

int mursV[4][5] = {{0,0,1,0,1}, //matrice mur vertical
                  {1,0,1,1,1},
                  {1,0,0,0,1},
                  {1,0,0,0,1}};

typedef struct {
    int presX; //d'ou on vient en x
    int presY; //d'ou on vient en Y
    int valeur; //valeur de la case infini (-1)
}t_case;


t_case Parcours[4][4];

int precX;
int precY;
int finX = 4;
int finY = 1;

int main()
{
    printf("Bot Dijstra\n");

    //lecture tableau d'adjacence en mémoire partagée TODO
    getTabAdjasence();

    //première case
    int x = 0;
    int y = 0;
    parcours(x, y);

    return 0;
}

void getTabAdjasence(){
    //blabla

}


void parcours(int x, int y){
    int end = 25;//fin
    int i=0; int j=0;//indexs
    int currIndex = 0; //numéro cases

    Parcours[x][y].valeur = 0;//première case
    int presX = -1;
    int presY = -1;

    int next[12] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int nIndex = -1;

    int nextP[12] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    int nIndexP = 0;

    printf("Debut algo\n");

    while(end != 1){

        //si fin
        if(i == finX && j == finY){
            printf("FIN\n");
            break;
        }

        //TODO
        printf("i:%d, j:%d, presX:%d presY:%d\n",i,j,presX,presY);
        //test droite = mur vertical droite
        if(mursV[i][j+1] == 0 && (j+1 != presY) && j-1 <= 4){
            printf("Droite : %d %d\n", i, j+1);
            if(Parcours[i][j+1].valeur == NULL || Parcours[i][j+1].valeur > currIndex + 1){
                printf("Index\n");
                Parcours[i][j+1].valeur = currIndex + 1;
                Parcours[i][j+1].presX = i;
                Parcours[i][j+1].presY = j;
            }
            nextP[nIndexP] = i;
            nextP[nIndexP + 1] = j+1;
            nIndexP += 2;
        }

        //test gauche = mur vertical gauche
        if(mursV[i][j] == 0 && (j-1 != presY) && j-1 >= 0){
            printf("Gauche : %d %d\n", i, j-1);
            if(Parcours[i][j-1].valeur == NULL || Parcours[i][j-1].valeur > currIndex + 1){
                printf("Index\n");
                Parcours[i][j-1].valeur = currIndex + 1;
                Parcours[i][j-1].presX = i;
                Parcours[i][j-1].presY = j;
            }
            nextP[nIndexP] = i;
            nextP[nIndexP + 1] = j-1;
            nIndexP += 2;
        }

        //test haut = mur horizontal haut
        if(mursH[i][j] == 0 && (i-1 != presX) && i-1 >= 0){
            printf("Haut : %d %d\n", i-1, j);
            if(Parcours[i-1][j].valeur == NULL || Parcours[i-1][j].valeur > currIndex + 1){
                printf("Index\n");
                Parcours[i-1][j].valeur = currIndex + 1;
                Parcours[i-1][j].presX = i;
                Parcours[i-1][j].presY = j;
            }
            nextP[nIndexP] = i-1;
            nextP[nIndexP + 1] = j;
            nIndexP += 2;
        }

        //test bas = mur horizontal bas
        if(mursH[i+1][j] == 0 && (i+1 != presX) && i+1 <= 4){
            printf("Bas : %d %d\n", i+1, j);
            if(Parcours[i+1][j].valeur == NULL || Parcours[i+1][j].valeur > currIndex + 1){
                printf("Index %d\n", currIndex + 1);
                Parcours[i+1][j].valeur = currIndex + 1;
                Parcours[i+1][j].presX = i;
                Parcours[i+1][j].presY = j;
            }
            nextP[nIndexP] = i+1;
            nextP[nIndexP + 1] = j;
            nIndexP += 2;
        }

        //on doit se déplacer sur les cases en currIndex++
        presX = i;
        presY = j;

        printTab(nextP);
        nIndex -= 2;
        if(nIndex < 0){
            printf("Reset next \n");
            for(int y=0; y < 12; y++){
                next[y] = nextP[y];
            }
            resetTabNext(nextP);
            nIndex = nIndexP - 2;
            nIndexP = 0;
        }

        printf("Reset all\n");
        i = next[nIndex];
        j = next[nIndex + 1];
        next[nIndex] = -1;
        next[nIndex + 1] = -1;

        //pres
        presX = Parcours[i][j].presX;
        presY = Parcours[i][j].presY;

        printTab(next);
        currIndex++;
        end--;
        printf("\n\nNext\n");
    }

    int sI; int sY;
    //On a trouvé la fin, plus qu'à créerle chemin.
    //On repart à l'inverse
    while(i + j > 0) {
        printf("Case %d %d numéro %d & %d %d\n", i, j, Parcours[i][j].valeur, Parcours[i][j].presX, Parcours[i][j].presY);
        sI = Parcours[i][j].presX;
        sY = Parcours[i][j].presY;
        i = sI; j = sY;
    }

}

void resetTabNext(int tab[12]){
    for(int y=0; y < sizeof(tab); y++){
        tab[y] = -1;
    }
}

void printTab(int tab[12]){
    for(int y=0; y < sizeof(tab); y++){
        printf("%d ", tab[y]);
    }
    printf("\n");
}

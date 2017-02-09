#include <stdio.h>
#include <stdlib.h>

#include "readmem.h"
#include "writemsg.h"

#define DEBUG 0

int **mursH; /*[5][4] = {{1,1,1,1}, //matrice mur horizontal
                  {1,0,0,0},
                  {0,1,0,0},
                  {1,1,0,1},
                  {1,0,1,1}};*/

int **mursV; /*[4][5] = {{0,0,1,0,1}, //matrice mur vertical
                  {1,0,1,1,1},
                  {1,0,0,0,1},
                  {1,0,0,0,1}};*/

typedef struct {
    int presX; //d'ou on vient en x
    int presY; //d'ou on vient en Y
    int valeur; //valeur de la case infini (-1)
}t_case;

int precX;
int precY;

int debutX = -1;
int debutY = -1;
int finX = 4;
int finY = 1;
int rows;
int cols;
int tailleSave;
int nbChemin;

int main(int argc, char *argv[])
{
    printf("Bot Dijstra\n");

    char* keyFile = "";
    if (argc < 2) {
        //pas de clés
        keyFile = "../labGen/keyfile";
    } else {
        //sinon paramètres
        keyFile = argv[1];
        printf("Key : %s\n", keyFile);
    }

    int numBot = 0;
    if(argc < 2){
        numBot = 1;
    } else {
        numBot = atoi(argv[2]);
        printf("Bot num : %d\n", numBot);
    }

    getTabs(keyFile);
    printf("debut %d fin %d\n", debutX, debutY);
    //première case
    int x = firstCase(debutX, rows);
    int y = firstCase(debutY, cols);

    int chemin[rows*cols*2];
    //algo Dijstra
    parcours(x, y, chemin);

    printGraph();
    tellMsg(keyFile, chemin, numBot);

    return 0;
}

int firstCase(int x, int max){
    if(x < 0){
        x = 0;
    } else if (x > max -1){
        x = max -1;
    }

    return x;
}

void parcours(int x, int y, int chemin[rows*cols*2]){
    printf("Rows %d cols %d x %d y %d\n", rows, cols, x, y);
    t_case **Parcours;
    //on initialise le tableau qui sauvegarde les cases utilisés (avec coordonnées case précédente)
    Parcours = malloc(rows * sizeof(*Parcours));
	for(int i=0 ; i < rows ; i++){
	     Parcours[i] = malloc(cols * sizeof(**Parcours));
	}

    int end = rows*cols*2;//fin max
    int i=x; int j=y;//indexs
    int currIndex = 0; //numéro cases

    Parcours[x][y].valeur = 0;//première case
    int presX = debutX;
    int presY = debutY;

    tailleSave = rows*cols/4;
    int next[tailleSave];
    resetTabNext(next);
    int nIndex = -1;

    int nextP[tailleSave];
    resetTabNext(nextP);
    int nIndexP = 0;

    printf("Debut algo\n");

    while(end != 1){

        //TODO
        printf("i:%d, j:%d, presX:%d presY:%d\n",i,j,presX,presY);
        //fprintf(fichier,"i:%d, j:%d, presX:%d presY:%d\n",i,j,presX,presY);

        if(i == finX && j == finY){
            printf("FIN\n");
        }else{
        //test droite = mur vertical droite
        if(mursV[i][j+1] == 0 && (j+1 != presY) && j-1 <= cols-1){
            printf("Droite : %d %d\n", i, j+1);
            //fprintf(fichier,"Droite : %d %d\n", i, j+1);
            if(Parcours[i][j+1].valeur == NULL || Parcours[i][j+1].valeur > currIndex + 1){
                printf("Index %d\n", currIndex + 1);
                Parcours[i][j+1].valeur = currIndex + 1;
                Parcours[i][j+1].presX = i;
                Parcours[i][j+1].presY = j;
            }
            nextP[nIndexP] = i;
            nextP[nIndexP + 1] = j+1;
            nIndexP += 2;
        }

        //test gauche = mur vertical gauche
        if(mursV[i][j] == 0 && (j-1 != presY) && (j-1 >= 0 || (j-1 == finY && i == finX))){
            printf("Gauche : %d %d\n", i, j-1);
            //fprintf(fichier,"Gauche : %d %d\n", i, j-1);
            if(Parcours[i][j-1].valeur == NULL || Parcours[i][j-1].valeur > currIndex + 1){
                printf("Index %d\n", currIndex + 1);
                Parcours[i][j-1].valeur = currIndex + 1;
                Parcours[i][j-1].presX = i;
                Parcours[i][j-1].presY = j;
            }
            nextP[nIndexP] = i;
            nextP[nIndexP + 1] = j-1;
            nIndexP += 2;
        }

        //test haut = mur horizontal haut
        if(mursH[i][j] == 0 && (i-1 != presX) && (i-1 >= 0 || (i-1 == finX && j == finY))){
            printf("Haut : %d %d parc %d\n", i-1, j, Parcours[i-1][j].valeur);
            //fprintf(fichier,"Haut : %d %d parc %d\n", i-1, j, Parcours[i-1][j].valeur);
            if(Parcours[i-1][j].valeur == NULL || Parcours[i-1][j].valeur > currIndex + 1){
                printf("Index %d\n", currIndex + 1);
                Parcours[i-1][j].valeur = currIndex + 1;
                Parcours[i-1][j].presX = i;
                Parcours[i-1][j].presY = j;
            }
            nextP[nIndexP] = i-1;
            nextP[nIndexP + 1] = j;
            nIndexP += 2;
        }

        //test bas = mur horizontal bas
        if(mursH[i+1][j] == 0 && (i+1 != presX) && i+1 <= rows - 1){
            printf("Bas : %d %d %d\n", i+1, j, Parcours[i+1][j].valeur);
            //fprintf(fichier,"Bas : %d %d parc %d\n", i-1, j, Parcours[i+1][j].valeur);
            if(Parcours[i+1][j].valeur == NULL || Parcours[i+1][j].valeur > currIndex + 1){
                printf("Index %d\n", currIndex + 1);
                Parcours[i+1][j].valeur = currIndex + 1;
                Parcours[i+1][j].presX = i;
                Parcours[i+1][j].presY = j;
            }
            printf("nextp index \n");
            nextP[nIndexP] = i+1;
            nextP[nIndexP + 1] = j;
            nIndexP += 2;
        }
        }

        //on doit se déplacer sur les cases en currIndex++
        presX = i;
        presY = j;

        printTab(nextP);
        printTab(next);
        nIndex -= 2;
        if(nIndex < 0){
            printf("Reset next \n");
            for(int y=0; y < tailleSave; y++){
                next[y] = nextP[y];
            }
            resetTabNext(nextP);
            nIndex = nIndexP - 2;
            nIndexP = 0;
        }

        printf("Reset all %d nI %d nIP %d\n", currIndex, nIndex, nIndexP);
        //fprintf(fichier,"Reset all %d nI %d nIP %d\n", currIndex, nIndex, nIndexP);

        if(currIndex + 1 > rows*cols && nIndexP == 0){
            break;
        }
        printTab(next);

        while(next[nIndex] == -1 && nIndex > 0){
            nIndex -= 2;
        }
        if(nextP[0] == -1 && next[0] == -1){
            break;
        }

        i = next[nIndex];
        j = next[nIndex + 1];
        next[nIndex] = -1;
        next[nIndex + 1] = -1;

        printf("i %d j %d\n", i, j);
        //pres
        presX = Parcours[i][j].presX;
        presY = Parcours[i][j].presY;

        //printTab(next);
        currIndex++;

        end--;
        printf("\n\nNext\n");
        //fprintf(fichier,"\n ------ \n");

    }

    //fclose(fichier);

    i = finX;
    j = finY;
    int sI; int sY;
    int ind = 0;
    //On a trouvé la fin, plus qu'à créerle chemin.
    //On repart à l'inverse
    while(Parcours[i][j].valeur != 0) {
        printf("Case %d %d numéro %d & %d %d\n", i, j, Parcours[i][j].valeur, Parcours[i][j].presX, Parcours[i][j].presY);
        sI = Parcours[i][j].presX;
        sY = Parcours[i][j].presY;
        chemin[ind] = i;
        chemin[ind + 1] = j;
        ind += 2;
        i = sI; j = sY;
    }
    chemin[ind] = i;
    chemin[ind + 1] = j;
    nbChemin = ind + 1;

    printf("Case %d %d numéro %d & %d %d\n", i, j, Parcours[i][j].valeur, Parcours[i][j].presX, Parcours[i][j].presY);

}

void tellMsg(char * keyFile, int chemin[rows*cols*2], int nbBot){

    char* id = malloc(10 * sizeof(int));
    char* id2 = malloc(10 * sizeof(int));
    sprintf(id,"%d",nbBot);

    writePile(keyFile, "Ready", id);
    char *result= malloc(256);
    int msg = readPile(result, keyFile, id);
    char *p;
    char *saveptr1;
    p = strtok_r(result, ",", &saveptr1);
    printf("Affichage P %s\n",p);

    if(strcmp(p,"Go") == 0){
        //récup du numéro d'écriture
        int numPile = atoi(strtok_r(NULL, ",", &saveptr1));
        printf("Le bot envoie les positions, numPile %d\n", numPile);
        char* msg;
        msg = malloc(10 * sizeof(int));
        for(int i = nbChemin; i >= 0; i -= 2){
            sprintf(msg,"%d,%d",chemin[i-1], chemin[i]); // i,j
            sprintf(id,"%d",numPile);
            printf("Envoie %s\n", msg);
            writePile(keyFile, msg, id);
            sprintf(id2,"%d",numPile + 1);
            int msg = readPile(result, keyFile, id2);
            if(strcmp(result,"Go") == 0){
                continue;
            }
        }
        writePile(keyFile, "Stop", "11");

    }
}

void resetTabNext(int tab[tailleSave]){
    for(int y=0; y < tailleSave; y++){
        tab[y] = -1;
    }
}

void printTab(int tab[tailleSave]){
    for(int y=0; y < tailleSave; y++){
        if(tab[y] == -1){
            break;
        }
        printf("%d ", tab[y]);
    }
    printf("\n");
}

void printGraph(){
    int entre = 0;
    int sortie = 0;

    printf("   ");
    for(int y = 0; y < rows; y++){
        if(y < 10){
            printf(" 0");
        }else{
            printf(" ");
        }
        printf("%d", y);
    }
    printf("\n");
    int z = 0;

    for(int i=0; i< rows * 2 + 1; i++){
        if(i%2 == 0){
            printf("   ");
        }else{
            if(z < 10){
                printf(" 0");
            } else{
                printf(" ");
            }
            printf("%d", z);
            z++;
        }
        for(int j=0; j< cols * 2 + 1; j++){

            if(i/2 == debutX && (j-1)/2 == debutY && entre == 0){
                printf("E");
                entre = 1;
                continue;
            }
            if((i-1)/2 == finX && j/2 == finY && sortie == 0){
                printf("S");
                sortie = 1;
                continue;
            }

            if (i % 2 == 0){//pair
                if(j % 2 != 0){ //impair
                    if(mursH[i/2][j/2] == 1){
                        printf("──");
                    } else {
                        printf("  "); //intersection
                    }
                } else {//pair
                    printf("•");
                }
            } else { //impair
                if(j % 2 == 0){ //impair
                    if(mursV[i/2][j/2] == 1){
                        printf("│");
                    } else {
                        printf(" "); //intersection
                    }
                } else {//pair
                    printf("  ");
                }
            }
        }
        printf("\n");
    }

    /*for(int i=0; i< 11; i++){
        for(int j=0; j< 11; j++){
                if(tab[i][j] == "x"){
                    if(tab[i][j+1] != " " && tab[i+1][j] != " " && tab[i-1][j] != " " && tab[i][j-1] != " "){
                        tab[i][j] = "┼";
                    }
                }
        }
    }

    printf("test %s",tab[1]);*/
    /*for(int i=0; i< 11; i++){
        for(int j=0; j< 11; j++){
                printf("%c",tab[i][j]);
        }
        printf("\n");
    }*/
}

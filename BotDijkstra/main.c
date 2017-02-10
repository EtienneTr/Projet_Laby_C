#include <stdio.h>
#include <stdlib.h>

#include "readmem.h"
#include "writemsg.h"

#define DEBUG 0

int **mursH; //murs horizontaux

int **mursV; //murs verticaux

typedef struct {
    int presX; //d'ou on vient en x
    int presY; //d'ou on vient en Y
    int valeur; //valeur de la case infini (-1)
}t_case;

int debutX = -1; //coordonée X entrée
int debutY = -1; //coordonnée Y entrée
int finX = 4;    //coordonéne X sortie
int finY = 1;    //coordonnée Y sortie
int rows;        //nombre de lignes du laby
int cols;        //nombre de colonnes du laby
int tailleSave;
int nbChemin;

int main(int argc, char *argv[])
{
    printf("Bot Dijstra\n");

    //Argument : changer le keyfile
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
    //demande du numéro du bot
    printf("Veuillez indiquer un numéro unique du bot (ex: 1) : ");
    scanf("%d", &numBot);
    printf("Bot numéro : %d\n", numBot);

    getTabs(keyFile);
    printf("Coordonnées entrée : %d %d, de sortie : %d %d\n", debutX, debutY, finX, finY);
    //première case
    int x = firstCase(debutX, rows);
    int y = firstCase(debutY, cols);

    //tableau contenant le chemin à parcourir pour aller du début à la fin du laby
    int chemin[rows*cols*2];
    //algo Dijstra
    parcours(x, y, chemin);

    //affichage du labyrinthe, optimisé pour les petit laby (taille < 40)
    printGraph();

    //Début de la communication avec le programme principal
    tellMsg(keyFile, chemin, numBot);

    return 0;
}

//placement de la première case dans le labyrinthe (entrée)
int firstCase(int x, int max){
    if(x < 0){
        x = 0;
    } else if (x > max -1){
        x = max -1;
    }

    return x;
}

//Fonction qui calcul le plus cours chemin et l'insère dans le tableau chemin
void parcours(int x, int y, int chemin[rows*cols*2]){

    if( DEBUG == 1) printf("Rows %d cols %d x %d y %d\n", rows, cols, x, y);

    t_case **Parcours;
    //on initialise le tableau qui sauvegarde les cases utilisés (avec coordonnées case précédente)
    Parcours = malloc(rows * sizeof(*Parcours));
	for(int i=0 ; i < rows ; i++){
	     Parcours[i] = malloc(cols * sizeof(**Parcours));
	}

    int end = rows*cols*2;//fin max, sécurité
    int i=x; int j=y;//indexs
    int currIndex = 0; //numéro cases

    Parcours[x][y].valeur = 0;//première case
    int presX = debutX;
    int presY = debutY;

    //tableaux qui sauvegardes les cases à visiter, i+1 et i+2
    tailleSave = rows*cols/2;
    int next[tailleSave];
    resetTabNext(next);
    int nIndex = -1;

    int nextP[tailleSave];
    resetTabNext(nextP);
    int nIndexP = 0;

    if( DEBUG == 1) printf("Debut algo\n");

    while(end != 1){

        if( DEBUG == 1) printf("i:%d, j:%d, presX:%d presY:%d\n",i,j,presX,presY);

        //si on est sur la case de fin, on ne se déplace pas et on passe à la case enregistrée suivante
        if(i == finX && j == finY){
            if( DEBUG == 1) printf("case fin\n");
        }else{

        //test droite = mur vertical droite
        if(mursV[i][j+1] == 0 && (j+1 != presY) && j-1 <= cols-1){

            if( DEBUG == 1) printf("Droite : %d %d\n", i, j+1);

            if(Parcours[i][j+1].valeur == NULL || Parcours[i][j+1].valeur > currIndex + 1){
                if( DEBUG == 1) printf("Index %d\n", currIndex + 1);
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

            if( DEBUG == 1) printf("Gauche : %d %d\n", i, j-1);

            if(Parcours[i][j-1].valeur == NULL || Parcours[i][j-1].valeur > currIndex + 1){
                if( DEBUG == 1) printf("Index %d\n", currIndex + 1);
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

            if( DEBUG == 1) printf("Haut : %d %d parc %d\n", i-1, j, Parcours[i-1][j].valeur);

            if(Parcours[i-1][j].valeur == NULL || Parcours[i-1][j].valeur > currIndex + 1){
                if( DEBUG == 1) printf("Index %d\n", currIndex + 1);
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

            if( DEBUG == 1) printf("Bas : %d %d %d\n", i+1, j, Parcours[i+1][j].valeur);

            if(Parcours[i+1][j].valeur == NULL || Parcours[i+1][j].valeur > currIndex + 1){
                if( DEBUG == 1) printf("Index %d\n", currIndex + 1);
                Parcours[i+1][j].valeur = currIndex + 1;
                Parcours[i+1][j].presX = i;
                Parcours[i+1][j].presY = j;
            }
            nextP[nIndexP] = i+1;
            nextP[nIndexP + 1] = j;
            nIndexP += 2;
        }
        }

        //on doit se déplacer sur les cases en currIndex++
        presX = i;
        presY = j;

        if( DEBUG == 1) printTab(nextP);
        if( DEBUG == 1) printTab(next);

        //Gestion des tableaux d'enregistrement des cases à visiter
        nIndex -= 2;
        if(nIndex < 0){
            if( DEBUG == 1) printf("Reset next \n");
            for(int y=0; y < tailleSave; y++){
                next[y] = nextP[y];
            }
            resetTabNext(nextP);
            nIndex = nIndexP - 2;
            nIndexP = 0;
        }

        if( DEBUG == 1) printf("Reset all %d nI %d nIP %d\n", currIndex, nIndex, nIndexP);
        //fprintf(fichier,"Reset all %d nI %d nIP %d\n", currIndex, nIndex, nIndexP);

        //Condition de fin : on a visiter toutes les cases
        if(currIndex + 1 > rows*cols && nIndexP == 0){
            break;
        }
        if( DEBUG == 1) printTab(next);

        //Gestion cas ou l'index est mal placé
        while(next[nIndex] == -1 && nIndex > 0){
            nIndex -= 2;
        }
        //Gestion tableaux de grande taille qui plante
        if(nextP[0] == -1 && next[0] == -1){
            break;
        }

        i = next[nIndex];
        j = next[nIndex + 1];
        next[nIndex] = -1;
        next[nIndex + 1] = -1;

        if( DEBUG == 1) printf("i %d j %d\n", i, j);
        //Set de la case précédente
        presX = Parcours[i][j].presX;
        presY = Parcours[i][j].presY;

        currIndex++;
        end--;

        if( DEBUG == 1) printf("\n\nNext\n");

    }

    //on se place sur la case de sortie et on parcourt le laby à l'inverse
    i = finX;
    j = finY;
    int sI; int sY;
    int ind = 0;
    //on insère le chemin dans le tableau chemin
    while(Parcours[i][j].valeur != 0) {
        printf("Déplacement case %d %d\n", i, j, Parcours[i][j].valeur, Parcours[i][j].presX, Parcours[i][j].presY);
        sI = Parcours[i][j].presX;
        sY = Parcours[i][j].presY;
        chemin[ind] = i;
        chemin[ind + 1] = j;
        ind += 2;
        i = sI; j = sY;
    }
    //sortie
    chemin[ind] = i;
    chemin[ind + 1] = j;
    nbChemin = ind + 1;

    printf("Sortie case %d %d\n", i, j, Parcours[i][j].valeur, Parcours[i][j].presX, Parcours[i][j].presY);

}

//Communication en pile de message
void tellMsg(char * keyFile, int chemin[rows*cols*2], int nbBot){

    char* id = malloc(10 * sizeof(int)); //id du premier canal d'écriture
    char* id2 = malloc(10 * sizeof(int)); //id du canal de réponse, en écoute
    sprintf(id,"%d",nbBot);

    //on indique qu'on est prêt pour la communication
    writePile(keyFile, "Ready", id);
    char *result= malloc(256);
    //en retour on a un message de type : Go,id avec 'id' le canal sur lequel il faut écrire
    int msg = readPile(result, keyFile, id);
    char *p;
    char *saveptr1;
    p = strtok_r(result, ",", &saveptr1);

    if(strcmp(p,"Go") == 0){
        //récup du canal d'écriture, lecture = canal écriture + 1
        int numPile = atoi(strtok_r(NULL, ",", &saveptr1));
        printf("Le bot envoie les positions, canal n° %d\n", numPile);
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
        //on stop
        writePile(keyFile, "Stop", id);

    }
}

//Vide d'un tableaux
void resetTabNext(int tab[tailleSave]){
    for(int y=0; y < tailleSave; y++){
        tab[y] = -1;
    }
}

//Affichage pour débug
void printTab(int tab[tailleSave]){
    for(int y=0; y < tailleSave; y++){
        if(tab[y] == -1){
            break;
        }
        printf("%d ", tab[y]);
    }
    printf("\n");
}

//affichage laby en fonction des matrices de murs
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
}

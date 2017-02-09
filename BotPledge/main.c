#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include "readmem.h"
#include "writemsg.h"
//#include "functionsMoove.h"

#define LG_CHAINE 256
enum direction{
	nord,
	sud,
	ouest,
	est
};

typedef struct Robots{
	int cols;
	int rows;
	int caseTab;
	int compteur;
	enum direction d;
} robot;

int mursHo[5][4] = {{1,1,1,1}, //matrice mur horizontal
                  {1,0,0,0},
                  {0,1,0,0},
                  {1,1,0,1},
                  {1,0,1,1}};
int mursVe[4][5] = {{0,0,1,0,1}, //matrice mur vertical
                  {1,0,1,1,1},
                  {1,0,0,0,1},
                  {1,0,0,0,1}};
                  
int **mursV;
int **mursH;
//int compteur=0;
int taille=4;

int debutX = -1;
int debutY = -1;
int finX = 4;
int finY = 1;
int rows;
int cols;



struct Robots deplacement(struct Robots robot){
	switch(robot.d){
		//direction precedente vers le sud
		case sud :
			if(mursVe[robot.rows][robot.cols]==0){//vers ouest
				robot.cols--;
				//robot.caseTab-=1;
				robot.d=ouest;
				robot.compteur--;
			}else if(mursHo[robot.rows+1][robot.cols]==0){//vers le sud
				robot.rows++;
				//robot.caseTab+=taille;
				robot.d=sud;
				robot.compteur = robot.compteur;
			}else if(mursVe[robot.rows][robot.cols+1]==0){//vers est
				robot.cols++;
				//robot.caseTab+=1;
				robot.d=est;
				robot.compteur++;
			}else if(mursHo[robot.rows][robot.cols]==0){//vers le nord
				robot.rows--;
				//robot.caseTab-=taille;
				robot.d=nord;
				robot.compteur+=2;
			}
			printf("1 position atcuel sud: Coll:%d, Row:%d direction: %d compteur: %d\n", robot.cols, robot.rows, robot.d, robot.compteur);
			break;
		//direction precedente vers le nord
		case nord :
			if(mursVe[robot.rows][robot.cols+1]==0){//vers est
				robot.cols++;
				//robot.caseTab+=1;
				robot.d=est;
				robot.compteur--;
			}else if(mursHo[robot.rows][robot.cols]==0){//vers le nord
				robot.rows--;
				//robot.caseTab-=taille;
				robot.d=nord;
				robot.compteur = robot.compteur;
			}else if(mursVe[robot.rows][robot.cols]==0){//vers ouest
				robot.cols--;
				//robot.caseTab-=1;
				robot.d=ouest;
				robot.compteur++;
			}else if(mursHo[robot.rows+1][robot.cols]==0){//vers le sud
				robot.rows++;
				//robot.caseTab+=taille;
				robot.d=sud;
				robot.compteur+=2;
			}
			printf("2 position atcuel nord: Coll:%d, Row:%d direction: %d compteur: %d\n", robot.cols, robot.rows, robot.d, robot.compteur);
			break;
		//direction precedente vers l'est
		case est :
			if(mursHo[robot.rows+1][robot.cols]==0){//vers le sud
				robot.rows++;
				//robot.caseTab+=taille;
				robot.d=sud;
				robot.compteur--;
			}else if(mursVe[robot.rows][robot.cols+1]==0){//vers est
				robot.cols++;
				//robot.caseTab+=1;
				robot.d=est;
				robot.compteur = robot.compteur;
			}else if(mursHo[robot.rows][robot.cols]==0){//vers le nord
				robot.rows--;
				//robot.caseTab-=taille;
				robot.d=nord;
				robot.compteur++;
			}else if(mursVe[robot.rows][robot.cols]==0){//vers ouest
				robot.cols--;
				//robot.caseTab-=1;
				robot.d=ouest;
				robot.compteur+=2;
			}
			printf("3 position atcuel est : Coll:%d, Row:%d direction: %d compteur: %d\n", robot.cols, robot.rows, robot.d, robot.compteur);
			break;
		//direction precedente vers l'ouest
		case ouest :
			if(mursHo[robot.rows][robot.cols]==0){//vers le nord
				robot.rows--;
				//robot.caseTab-=taille;
				robot.d=nord;
				robot.compteur--;
			}else if(mursVe[robot.rows][robot.cols]==0 && robot.cols!=0){//vers ouest
				robot.cols--;
				//robot.caseTab-=1;
				robot.d=ouest;
				robot.compteur = robot.compteur;
			}else if(mursHo[robot.rows+1][robot.cols]==0){//vers le sud
				robot.rows++;
				//robot.caseTab+=taille;
				robot.d=sud;
				robot.compteur++;
			}else if(mursVe[robot.rows][robot.cols+1]==0){//vers est
				robot.cols++;
				//robot.caseTab+=1;
				robot.d=est;
				robot.compteur+=2;
			}
			printf("4 position atcuel ouest: Coll:%d, Row:%d direction: %d compteur: %d\n", robot.cols, robot.rows, robot.d, robot.compteur);
			break;
		}
	return robot;
}

struct Robots compteurZero(struct Robots robot){
	while(1){
		if(robot.d == ouest && mursVe[robot.rows][robot.cols]==0){
			robot.cols--;
			robot.caseTab-=1;
		}else if(robot.d == nord && mursHo[robot.rows][robot.cols]==0){
			robot.rows--;
		}else if(robot.d == est && mursVe[robot.rows][robot.cols+1]==0){
			robot.cols++;
			robot.caseTab+=1;
		}else if(robot.d == sud && mursHo[robot.rows+1][robot.cols]==0){
			robot.rows++;
		}
		else{
			break;
		}
	}
	return robot;		
}

struct Robots firstMoov(struct Robots robot){
	if(mursVe[robot.rows][robot.cols+1]==0){
		robot.cols++;
		robot.rows=0;
		//robot.caseTab=(taille*robot.rows)+robot.cols;
		robot.d=est;
	}else if(mursHo[robot.rows+1][robot.cols]==0){
		robot.rows--;
		robot.cols=0;
		//robot.caseTab=(taille*robot.rows)+robot.cols;
		robot.d=nord;
	}else if(mursVe[robot.rows][robot.cols]==0){
		robot.rows++;
		robot.cols=0;
		//robot.caseTab=(taille*robot.rows)+robot.cols;
		robot.d=sud;
	}else if(mursHo[robot.rows][robot.cols]==0){
		robot.cols++;
		robot.rows=0;
		//robot.caseTab=(taille*robot.rows)+robot.cols;
		robot.d=ouest;
	}
	return robot;
}

// +1 quand il tourne à gauche -1 quand il torune à droite, quand 0 refait etape 1

int main(int argc, char **argv)
{
	printf("Bot Pledge\n");
	// x=Coll et y=Row inverse de Dijtra
    char* keyFile = "";
    if (argc != 2) {
        //pas de clés
        keyFile = "../labGen/keyfile";
    } else {
        //sinon paramètres
        keyFile = argv[2];
    }

    getTabs(keyFile);
    printf("debut %d fin %d\n", debutX, debutY);
    //première case
    int posRows = firstCase(debutX, rows);
    int posCols = firstCase(debutY, cols);
    
	struct Robots bot;
	bot.cols=0;
	bot.rows=0;
	bot.compteur=0;
	int sortie=17;
	bot = firstMoov(bot);
	printf("position atcuel: x:%d, y:%d direction: %d\n", bot.cols, bot.rows, bot.d);
	//compteurZero(robot);
	while (1){
		if(bot.rows==3 && bot.cols==1){
			break;
		}
		if(bot.compteur==0){
			bot = compteurZero(bot);
			bot = deplacement(bot);
		}else{
			bot = deplacement(bot);
		}
	}
	printf("position atcuel: x:%d, y:%d direction: %d FIN DU LABYRINTHE\n", bot.cols, bot.rows, bot.d);
	
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

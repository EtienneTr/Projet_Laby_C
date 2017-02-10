#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <stdio.h>
#include "readmem.h"
#include "writemsg.h"

enum direction{
	nord,
	sud,
	ouest,
	est
};

typedef struct Robots{
	int cols;
	int rows;
	int compteur;
	enum direction d;
} robot;
/*matrice de test
int mursHo[5][4] = {{1,1,1,1}, //matrice mur horizontal
                  {1,0,0,0},
                  {0,1,0,0},
                  {1,1,0,1},
                  {1,0,1,1}};
int mursVe[4][5] = {{0,0,1,0,1}, //matrice mur vertical
                  {1,0,1,1,1},
                  {1,0,0,0,1},
                  {1,0,0,0,1}};
*/                  
int **mursV;
int **mursH;
int debutX=0;
int debutY=0;
int finX = 4;
int finY = 1;
int rows;
int cols;
int ind=0;
int nbChemin;
    
struct Robots deplacement(struct Robots robot){
	switch(robot.d){
		//direction precedente vers le sud
		case sud :
			if(mursV[robot.rows][robot.cols]==0 && entreeLab(robot, ouest)!=0){//vers ouest
				robot.cols--;
				robot.d=ouest;
				robot.compteur--;
			}else if(mursH[robot.rows+1][robot.cols]==0 && entreeLab(robot, sud)!=0){//vers le sud
				robot.rows++;
				robot.d=sud;
				robot.compteur = robot.compteur;
			}else if(mursV[robot.rows][robot.cols+1]==0 && entreeLab(robot, est)!=0){//vers est
				robot.cols++;
				robot.d=est;
				robot.compteur++;
			}else if(mursH[robot.rows][robot.cols]==0 && robot.rows!=0 && entreeLab(robot, nord)!=0){//vers le nord
				robot.rows--;
				robot.d=nord;
				robot.compteur+=2;
			}
			printf("1 position atcuel sud: Coll:%d, Row:%d direction: %d compteur: %d\n", robot.cols, robot.rows, robot.d, robot.compteur);
			break;
		//direction precedente vers le nord
		case nord :
			if(mursV[robot.rows][robot.cols+1]==0 && entreeLab(robot, est)!=0){//vers est
				robot.cols++;
				robot.d=est;
				robot.compteur--;
			}else if(mursH[robot.rows][robot.cols]==0 && entreeLab(robot, est)!=0){//vers le nord
				robot.rows--;
				robot.d=nord;
				robot.compteur = robot.compteur;
			}else if(mursV[robot.rows][robot.cols]==0 && entreeLab(robot, est)!=0){//vers ouest
				robot.cols--;
				robot.d=ouest;
				robot.compteur++;
			}else if(mursH[robot.rows+1][robot.cols]==0 && entreeLab(robot, sud)!=0){//vers le sud
				robot.rows++;
				robot.d=sud;
				robot.compteur+=2;
			}
			printf("2 position atcuel nord: Coll:%d, Row:%d direction: %d compteur: %d\n", robot.cols, robot.rows, robot.d, robot.compteur);
			break;
		//direction precedente vers l'est
		case est :
			if(mursH[robot.rows+1][robot.cols]==0 && entreeLab(robot, sud)!=0){//vers le sud
				robot.rows++;
				robot.d=sud;
				robot.compteur--;
			}else if(mursV[robot.rows][robot.cols+1]==0 && entreeLab(robot, est)!=0){//vers est
				robot.cols++;
				robot.d=est;
				robot.compteur = robot.compteur;
			}else if(mursH[robot.rows][robot.cols]==0 && robot.rows!=0 && entreeLab(robot, nord)!=0){//vers le nord
				robot.rows--;
				robot.d=nord;
				robot.compteur++;
			}else if(mursV[robot.rows][robot.cols]==0 && robot.cols!=0 && entreeLab(robot, ouest)!=0){//vers ouest
				robot.cols--;
				robot.d=ouest;
				robot.compteur+=2;
			}
			printf("3 position atcuel est : Coll:%d, Row:%d direction: %d compteur: %d\n", robot.cols, robot.rows, robot.d, robot.compteur);
			break;
		//direction precedente vers l'ouest
		case ouest :
			if(mursH[robot.rows][robot.cols]==0 && entreeLab(robot, nord)!=0){//vers le nord
				robot.rows--;
				robot.d=nord;
				robot.compteur--;
			}else if(mursV[robot.rows][robot.cols]==0 && entreeLab(robot, ouest)!=0){//vers ouest
				robot.cols--;
				robot.d=ouest;
				robot.compteur = robot.compteur;
			}else if(mursH[robot.rows+1][robot.cols]==0 && entreeLab(robot, sud)!=0){//vers le sud
				robot.rows++;
				robot.d=sud;
				robot.compteur++;
			}else if(mursV[robot.rows][robot.cols+1]==0 && entreeLab(robot, est)!=0){//vers est
				robot.cols++;
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
		if(robot.d == ouest && mursV[robot.rows][robot.cols]==0){
			robot.cols--;
		}else if(robot.d == nord && mursH[robot.rows][robot.cols]==0){
			robot.rows--;
		}else if(robot.d == est && mursV[robot.rows][robot.cols+1]==0){
			robot.cols++;
		}else if(robot.d == sud && mursH[robot.rows+1][robot.cols]==0){
			robot.rows++;
		}
		else{
			break;
		}
	}
	return robot;		
}

struct Robots firstMoov(struct Robots robot){
	if(mursV[robot.rows][robot.cols+1]==0 && entreeLab(robot, est)!=0){//deplacement vers est
		robot.cols++;
		robot.rows=robot.rows;
		robot.d=est;		
	}else if(mursH[robot.rows+1][robot.cols]==0 && entreeLab(robot, sud)!=0){//deplacemnt vers le sud
		robot.rows++;
		robot.cols=robot.cols;
		robot.d=sud;
	}else if(mursV[robot.rows][robot.cols-1]==0 && entreeLab(robot, ouest)!=0){//deplacement vers l'ouest
		robot.cols--;
		robot.rows=robot.rows;
		robot.d=ouest;
	}else if(mursH[robot.rows-1][robot.cols]==0 && entreeLab(robot, nord)!=0){//deplacement vers le nord
		robot.rows--;
		robot.cols=robot.cols;
		robot.d=nord;
	}
	return robot;
}

int entreeLab(struct Robots robot, enum direction direct){
	if(direct==ouest){
		int colsD=robot.cols--;
		if(robot.rows == debutX && colsD == debutY){
			return 0;
		}
	}else if(direct==nord){
		int rowsD=robot.rows--;
		if(rowsD == debutX && robot.cols == debutY){
			return 0;
		}
	}else if(direct==est){
		int colsD=robot.cols+1;
		if(robot.rows == debutX && colsD == debutY){
			return 0;
		}
	}else if(direct==sud){
		int rowsD=robot.rows++;
		if(rowsD == debutX && robot.cols == debutY){
			return 0;
		}
	}
	return 1;	
}
// +1 quand il tourne à gauche -1 quand il torune à droite, quand 0 refait etape 1

int main(int argc, char **argv)
{
	struct Robots bot;//declaration du bot
	bot.cols=0;
	bot.rows=0;
	bot.compteur=0;
	
	printf("Bot Pledge\n");
    char* keyFile = "";
    if (argc != 2) {
        //pas de clés
        keyFile = "../labGen/keyfile";
    } else {
        //sinon paramètres
        keyFile = argv[2];
    }

    int numBot = 0;
    //demande du numéro du bot
    printf("Veuillez indiquer un numéro unique du bot (ex: 1) : ");
    scanf("%d", &numBot);
    printf("Bot numéro : %d\n", numBot);
    
	int debutX = 0;
	int debutY = -1;
    getTabs(keyFile);
    printf("debut %d fin %d\n", debutX, debutY);   
	int chemin[rows*cols*2];
	
    //première case
    int posRows = firstCase(debutX, rows);
    int posCols = firstCase(debutY, cols);

	bot = firstMoov(bot);// premier deplacement
	chemin[ind] = bot.rows;
	chemin[ind + 1] = bot.cols;
	ind += 2;
	printf("position atcuel: x:%d, y:%d direction: %d\n", bot.cols, bot.rows, bot.d);
	while (1){
		if(bot.rows==finX && bot.cols==finY){//si la fin
			break;
		}
		if(bot.compteur==0){
			bot = compteurZero(bot);//deplacement en ligne droite
			chemin[ind] = bot.rows;
			chemin[ind + 1] = bot.cols;
			ind += 2;
			bot = deplacement(bot);//un deplacement apres le compteur à 0
			chemin[ind] = bot.rows;
			chemin[ind + 1] = bot.cols;
			ind += 2;
		}else{
			bot = deplacement(bot);// deplacement normal vers la droite
			chemin[ind] = bot.rows;
			chemin[ind + 1] = bot.cols;
			ind += 2;
		}
	}
	printf("position atcuel: x:%d, y:%d direction: %d FIN DU LABYRINTHE\n", bot.cols, bot.rows, bot.d);
	
	//envoie pile
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
        for(int i = 0; i != nbChemin; i -= 2){
            sprintf(msg,"%d,%d",chemin[i+1], chemin[i]); // i,j
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

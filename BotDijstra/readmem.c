#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
  key_t key;             // cle d'accès à la structure IPC
  int sem;               // identifiant du semaphore
  int shm;               // identifiant de la memoire partagee
  struct sembuf sembuf;  // gestion des operations semaphores
  char *chaine = NULL;   // pointeur d'attachement shared memory

  if (argc != 2) {
    fprintf(stderr, "Syntaxe : %s fichier_clé \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // generation de la cle
  if ((key = ftok(argv[1], 0)) == -1) {
    perror("ftok");
    exit(EXIT_FAILURE);
  }

  // acces a l'ensemble semaphore et a la memoire partagee
  if (((sem = semget(key, 1, 0)) == -1) || ((shm = shmget(key, 0, 0)) == -1)) {
    perror("semget/shmget");
    exit(EXIT_FAILURE);
  }

  // attachement de la memoire partagee au pointeur *chaine
  if ((chaine = shmat(shm, NULL, SHM_RDONLY)) == (void *)-1) {
    perror("shmat");
    exit(EXIT_FAILURE);
  }

  // preparation et execution de l'action P()
  sembuf.sem_num = 0;
  sembuf.sem_op = -1;
  sembuf.sem_flg = 0;
  if (semop(sem, &sembuf, 1) < 0) {
    perror("semop");
    exit(EXIT_FAILURE);
  }

  // entree en section critique
  // affichage du contenu de la memoire partagee

  fprintf(stdout, "%s\n", chaine);

  // preparation et execution de l'action V()
  sembuf.sem_op = 1;
  if (semop(sem, &sembuf, 1) < 0) {
    perror("semop");
    exit(EXIT_FAILURE);
  }

  // sortie de section critique
  return EXIT_SUCCESS;
}

#ifndef WRITEMSG_H_INCLUDED
#define WRITEMSG_H_INCLUDED
void writePile(char* keyFile, char* msg, char* id);
char *readPile(char result[], char* keyFile, char* id);


void waitBot(int numBot, char* keyFile);

#endif // WRITEMSG_H_INCLUDED

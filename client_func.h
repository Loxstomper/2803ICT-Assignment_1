#ifndef CLIENT_FUNC_H
#define CLIENT_FUNC_H
#include "common.h"

void put(int sock, char** args);
void get(int sock, char message[BUFFER_SIZE]);
void run(int sock, char message[BUFFER_SIZE], char** args);



#endif

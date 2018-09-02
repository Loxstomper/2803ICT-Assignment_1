#ifndef CLIENT_FUNC_H
#define CLIENT_FUNC_H
#include "common.h"

void put_client(int sock, char message[BUFFER_SIZE], char** args);
void get_client(int sock, char message[BUFFER_SIZE]);
void run_prog_client(int sock, char message[BUFFER_SIZE], char** args);
void read_data(int sock);

#endif

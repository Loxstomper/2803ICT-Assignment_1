#ifndef SERVER_FUNC_H
#define SERVER_FUNC_H

#include "common.h"

void list(int client_sock, char** args);
void get(int client_sock, char** args);
void put(int client_sock, char** args);
void run(int client_sock, char** args);
void sys(int client_sock);
void print_args(char** args);

#endif

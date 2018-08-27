#include "client_func.h"
#include "common.h"
#include <stdio.h>

void put(int sock, char** args)
{
    printf("PUT FUNCTION ON CLIENT\n");

    free(args);
}

void get(int sock, char message[BUFFER_SIZE])
{
    printf("GET FUNCTION ON CLIENT\n");
}

void run(int sock, char message[BUFFER_SIZE], char** args)
{
    printf("RUN FUNCTION ON CLIENT\n");

    free(args);
}

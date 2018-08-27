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

    /* figure out if going to print the output or save to file */
    int count = 0;
    int save_to_file = 0;

    while (args[count] != NULL)
    {
        count ++;
    }

    if (strcmp(args[count - 1], "-f") == 0)
    {
        save_to_file = 1;   
    }






    free(args);
}

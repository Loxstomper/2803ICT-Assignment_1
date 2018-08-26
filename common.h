#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <signal.h>

#define PORT 18284 
#define BUFFER_SIZE 1024
#define ARG_BUFFER_SIZE 64


#define DELIM " \t\n\r"
char** get_args(char line[BUFFER_SIZE])
{
    int position = 0;

    // memory leak? never freeing
    char** args = malloc(ARG_BUFFER_SIZE * sizeof(char*));
    char* arg;

    if (!args)
    {
        printf("Failed to dynamically allocate args char**\n");
        exit(1);
    }

    arg = strtok(line, DELIM);

    while (arg != NULL)
    {
        args[position] = arg;
        printf("%d: %s\n", position, args[position]);
        position ++;

        arg = strtok(NULL, DELIM);
    }

    args[position] = NULL;

    return args;
}

#endif

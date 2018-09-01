#include "common.h"

char** get_args(char line[BUFFER_SIZE])
{
    int position = 0;
    // memory leak? never freeing
    char** args = malloc(ARG_BUFFER_SIZE * sizeof(char*));
    char* arg;

    char* temp = malloc(BUFFER_SIZE * sizeof(char));

    strcpy(temp, line);


    if (!args)
    {
        printf("Failed to dynamically allocate args char**\n");
        exit(1);
    }
    arg = strtok(temp, DELIM);
    while (arg != NULL)
    {
        /* args[position] = arg; */
        args[position] = malloc(ARG_BUFFER_SIZE * sizeof(char));
        strcpy(args[position], arg);
        /* printf("%d: %s\n", position, args[position]); */
        position ++;
        arg = strtok(NULL, DELIM);
    }
    args[position] = NULL;

    free(temp);

    return args;
}

void free_args(char** args)
{
    int i = 0;

    while (args[i] != NULL)
    {
        free(args[i]);
        i ++;
    }

    free(args);
}

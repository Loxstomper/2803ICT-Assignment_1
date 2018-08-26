#include "server_func.h"
#define ARG_BUFFER_SIZE 64

void list(int client_sock, char** args) 
{
    char* output_buffer = malloc(BUFFER_SIZE * sizeof(char));
    const char* base_command = "ls ./programs/";
    char* command;

    if (!output_buffer)
    {
        printf("Failed to dynamically create buffer\n");
        exit(1);
    }

    printf("RUNNING LIST COMMAND\n");
    printf("ARGS ARE: \n");

    int arg_length = 0;

    while (args[arg_length] != NULL)
    {
        printf("%d : %s \n", arg_length, args[arg_length]);
        arg_length ++;
    }

    printf("\n");

     

    /* long list */
    printf("LONG LIST? %d\n", strcmp(args[1], "-l") == 0);
    if (strcmp(args[1], "-l") == 0)
    {
        /* long list no program specified */
        //getting some garbage data
        if (args[2] == NULL)
        {
            printf("LONG LIST AND NO PROGRAM SPECIFIED\n");

            command = (char*) malloc(sizeof(base_command) + 4);
            strcpy(command, base_command);
            strcat(command, " -l");
 
        }
        /* long list program name specified */
        else
        {
            printf("LONG LIST AND PROGRAM SPECIFIED\n");

            command = (char*) malloc(sizeof(base_command) + 5 + sizeof(args[2]));
            strcpy(command, base_command);
            strcat(command, args[2]);
            strcat(command, " ");
            strcat(command, " -l");
        }
    }
    else
    {
        printf("NORMAL LIST\n");

        /* list all programs */
        if (args[1] == NULL)
        {
            command = malloc(sizeof(base_command));
            strcpy(command, base_command);
        }
        /* list program name specified */
        else
        {
            command = (char*) malloc(sizeof(base_command) + 2 + sizeof(args[1]));
            strcpy(command, base_command);
            strcat(command, args[1]);
            strcat(command, " ");
        }
    }

    printf("COMMAND: %s\n", command);
    FILE* f = popen(command, "r");

    free(command);
    
    if (!f)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    while (fgets(output_buffer, BUFFER_SIZE, f) != NULL)
    {
        send(client_sock, output_buffer, strlen(output_buffer), 0);
        printf("%s", output_buffer);
        memset(output_buffer, '\0', strlen(output_buffer));
    }

}

void get(int client_sock, char buffer[BUFFER_SIZE])
{
    /* change this just to read the file for cross platform */ 
    FILE* f = popen("cat client.c", "r");

    while (fgets(buffer, BUFFER_SIZE, f) != NULL)
    {
        send(client_sock, buffer, strlen(buffer), 0);
        memset(buffer, '\0', strlen(buffer));
    }
}

void sys(int client_sock, char buffer[BUFFER_SIZE])
{
    FILE* f = popen("uname -sr && cat /proc/cpuinfo | grep 'model name' | uniq && uname -p", "r");

    while (fgets(buffer, BUFFER_SIZE, f) != NULL)
    {
        send(client_sock, buffer, strlen(buffer), 0);
        memset(buffer, '\0', strlen(buffer));
    }
}

#define DELIM " \t\n\r"
char** get_args(char line[BUFFER_SIZE])
{
    int position = 0;

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

void print_args(char** args)
{
    int pos = 0;

    while (args[pos] != NULL)
    {
        printf("%s ", args[pos]);
        pos ++;
    }
    printf("\n");
}

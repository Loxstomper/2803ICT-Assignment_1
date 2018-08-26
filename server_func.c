#include "server_func.h"

void list(int client_sock, char** args) 
{
    char* buffer = malloc(BUFFER_SIZE * sizeof(char));
    const char* base_command = "ls ./programs/";
    char* command;

    if (!buffer)
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

    FILE* f; 

    if (arg_length > 1)
    {
        /* long list */
        if (strcmp(args[1], "-l") == 0)
        {
            /* long list no program specified */
            if (args[2] == NULL)
            {
                char* new;

                if (new = malloc((sizeof(base_command) + sizeof(char) * 3) + 1) == NULL)
                {
                    printf("Failed malloc\n");
                    exit(1);
                }

                new[0] = '\0';
                strcat(new, base_command);
                strcat(new, " -l");
                
                strcpy(command, new);

                free(new);
            }
            /* long list program name specified */
            else
            {
                char* new = malloc((sizeof(base_command) + sizeof(char) * 3 + sizeof(args[2])) + 1);
                new[0] = '\0';
                strcat(new, base_command);
                strcat(new, " -l");
                strcat(new, args[2]);
                
                strcpy(command, new);

                free(new);
            }
        }
        else
        {
            /* list all programs */
            if (args[2] == NULL)
            {
                command = malloc(sizeof(base_command));
                strcpy(command, base_command);
            }
            /* list program name specified */
            else
            {

            }
        }
    }

    printf("COMMAND: %s", command);
    f = popen(command, "r");

    free(command);
    
    if (!f)
    {
        printf("Failed to run command\n");
        exit(1);
    }

    while (fgets(buffer, BUFFER_SIZE, f) != NULL)
    {
        send(client_sock, buffer, strlen(buffer), 0);
        /* printf("%s", buffer) */
        memset(buffer, '\0', strlen(buffer));
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

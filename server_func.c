#include "server_func.h"

#define ARG_BUFFER_SIZE 64

void list(int client_sock, char** args) 
{
    char* output_buffer = malloc(BUFFER_SIZE * sizeof(char));
    static const char* base_command = "ls ./programs/";
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

    free(args);
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

void get(int client_sock, char** args)
{
    printf("IN GET FUNCTION\n");

    char* output_buffer = malloc(BUFFER_SIZE * sizeof(char));
    static const char* base_dir = "./programs/";
    char* path;

    path = (char*) malloc(sizeof(base_dir) + 1 + sizeof(args[1]) + 1 + sizeof(args[2]) + 1 + 10);
    strcpy(path, base_dir);
    strcat(path, args[1]);
    strcat(path, "/");
    strcat(path, args[2]);

    printf("PATH: %s\n", path);
    /* change this just to read the file for cross platform */ 
    FILE* f = fopen(path, "r");

    int line_count = 0;

    while (fgets(output_buffer, BUFFER_SIZE, f) != NULL)
    {
        line_count ++;
        printf("%s", output_buffer);
        send(client_sock, output_buffer, strlen(output_buffer), 0);

        if (line_count % 40 == 0)
        {
            printf("\nWAITING\n\n");
        }

        memset(output_buffer, '\0', strlen(output_buffer));
    }


    printf("\n");
    fclose(f);
    free(output_buffer);
    free(args);
}


// do i need to close f?
void put(int client_sock, char ** args)
{
    // program name, source file, -f

    // this is lame but oh well
    static char* base_path = "./programs/";
    static char* mkdir = "mkdir -p ./programs/";

    char* input_buffer = malloc(BUFFER_SIZE * sizeof(char));
    char* command;
    char* path;

    command = (char*) malloc(sizeof(mkdir) + sizeof(args[1]) + 4);
    strcpy(command, mkdir);
    strcat(command, args[1]);

    FILE* f = popen(command, "r");

    path = (char*) malloc(sizeof(base_path) + sizeof(args[1]) + sizeof(args[2]) + 8);
    strcpy(path, base_path);
    strcat(path, args[1]);
    strcat(path, "/");
    strcat(path, args[2]);

    // check if file exists
    int file_exits = (access(path, F_OK) != -1);
    int forced     = (strcmp(args[2], "-f") == 0);

    if (file_exits && !forced)
    {
        printf("FILE EXISTS AND NOT FORCED MODE: %s \n", path);
    }
    // file doesnt exist, or it does and force mode is on
    else
    {
        FILE* fp = fopen(path, "w");

        // read until eof and write to file
        recv(client_sock, input_buffer, BUFFER_SIZE, 0);

        while (input_buffer[strlen(input_buffer)] != EOF)
        {
            fprintf(fp, input_buffer);
            memset(&input_buffer, '\0', strlen(input_buffer));
        }

        fclose(fp);

    }

    free(input_buffer);
    free(command);
    free(path);
    free(args);
}

void sys(int client_sock)
{
    printf("INSIDE SYS FUNCTION\n\n");

    char* output_buffer = malloc(BUFFER_SIZE * sizeof(char));
    static const char* command = "uname -sr && cat /proc/cpuinfo | grep 'model name' | uniq && uname -p";

    FILE* f = popen(command, "r");

    while (fgets(output_buffer, BUFFER_SIZE, f) != NULL)
    {
        send(client_sock, output_buffer, strlen(output_buffer), 0);
        printf("%s", output_buffer);
        memset(output_buffer, '\0', strlen(output_buffer));
    }

    free(output_buffer);
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

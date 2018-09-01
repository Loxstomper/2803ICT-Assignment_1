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

     

    /* long list */
    // printf("LONG LIST? %d\n", strcmp(args[1], "-l") == 0);
    if (args[1] != NULL && strcmp(args[1], "-l") == 0)
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

    free_args(args);
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

    send(client_sock, "`", strlen("`"), 0);

}

void get(int client_sock, char** args)
{
    printf("IN GET FUNCTION\n");

    // check if command is valid before doing the other stuff
    if (args[2] == NULL)
    {
        send(client_sock, "Invalid usage`", sizeof("Invalid usage`"), 0);
        free_args(args);
        return;
    }

    char* output_buffer = malloc(BUFFER_SIZE * sizeof(char));
    static const char* base_dir = "./programs/";
    char* path;

    char* input_buffer = malloc(16 * sizeof(char));

    printf("PROGRAM: %s | FILE: %s\n", args[1], args[2]);

    path = (char*) malloc(sizeof(base_dir) + 1 + sizeof(args[1]) + 1 + sizeof(args[2]) + 1 + 10);
    strcpy(path, base_dir);
    strcat(path, args[1]);
    strcat(path, "/");
    strcat(path, args[2]);

    printf("PATH: %s\n", path);
    FILE* f = fopen(path, "r");

    if (f == NULL)
    {
        printf("File not found\n");
        send(client_sock, "File not found`", sizeof("File not found`"), 0);
        return;
    }

    int line_count = 1;

    while (fgets(output_buffer, BUFFER_SIZE, f) != NULL)
    {
        printf("%s", output_buffer);
        int z = send(client_sock, output_buffer, strlen(output_buffer), 0);
        usleep(200);


        if (line_count % 40 == 0)
        {
            printf("\nWAITING\n\n");
            /* put a recv call because thats blocking */
            /* get client to wait for the 40 lines and then do a scanf or something */
            /* dont even use the data though */
            recv(client_sock, input_buffer, 16, 0);
        }


        memset(output_buffer, '\0', strlen(output_buffer));
        line_count ++;
    }

    send(client_sock, "`", strlen("`"), 0);


    printf("\n");
    fclose(f);
    free(input_buffer);
    free(output_buffer);
    free(args);

    /* printf("FINISHED\n"); */
}


// do i need to close f?
void put_prog(int client_sock, char ** args)
{
    // first look at the args the client sent

    int arg_count;
    int file_count;
    int forced;
    static char* base_path = "./programs/";
    char* prog_path;
    char* file_path;


    for (arg_count = 0; args[arg_count] != NULL; arg_count ++)
    {
        printf("[%d] %s\n", arg_count, args[arg_count]);
    }

    forced = (strcmp(args[arg_count -1], "-f") == 0);
    file_count = arg_count - 2 - forced;

    printf("Number of files: %d\n", file_count);

    prog_path = (char*) malloc(sizeof(base_path) + sizeof(args[1]) + 5);
    strcpy(prog_path, base_path);
    strcat(prog_path, args[1]);
    strcat(prog_path, "/");

    printf("Prog path: %s\n", prog_path);

    // check if the program directory exists, if it does and not forced end
    DIR* d = opendir(prog_path);
    int exists = 0;
    printf("EXISTS %d\n", exists);

    if (d)
    {
        exists = 1;
        printf("Directory exists, force mode?: %d\n", forced);

        if (forced)
        {
            char* command = malloc(sizeof(char) * 100);
            strcpy(command, "rm -r ");
            strcat(command, prog_path);

            printf("DELETE %s\n", command);
            // FILE* f = popen(command);
            // fclose(f);
            free(command);
        }
    }
    else if (ENOENT == errno)
    {
        exists = 0;
        printf("Directory does not exist\n");

        char* command = malloc(sizeof(char) * 100);
        strcpy(command, "mkdir ");
        strcat(command, prog_path);

        printf("MAKE %s\n", command);
        // FILE* f = popen(command);
        // fclose(f);
        free(command);
    }
    else
    {
        printf("Problem opening directory\n");
        send(client_sock, "Error opening directory", sizeof("Error opening directory"), 0);
        return;
    }

    send(client_sock, "ok", sizeof("client"), 0);

    file_path = (char*) malloc(sizeof(prog_path) + 100);

    for (int i = 0; i < file_count; i ++)
    {

        // read in the file - filename is args[2 + i]
        printf("READING file [%d/%d] %s \n", i + 1, file_count, args[2 + i]);
    }

    return;












    // this is lame but oh well
    static char* mkdir = "mkdir ./programs/";

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
            recv(client_sock, input_buffer, BUFFER_SIZE, 0);

            // might need some sleeps in here
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
    char* output_buffer = malloc(BUFFER_SIZE * sizeof(char));
    static const char* command = "uname -sr && cat /proc/cpuinfo | grep 'model name' | uniq && uname -p";

    FILE* f = popen(command, "r");

    output_buffer[0] = '\0';

    while (fgets(output_buffer, BUFFER_SIZE, f) != NULL)
    {
        send(client_sock, output_buffer, strlen(output_buffer), 0);
        printf("%s", output_buffer);
        memset(output_buffer, '\0', strlen(output_buffer));
    }

    send(client_sock, "`", sizeof("`"), 0);

    free(output_buffer);
    printf("FINISHED\n");
}

void run_prog(int client_sock, char** args)
{
    // check for valid command before doing other stuff
    if (args[1] == NULL)
    {
        send(client_sock, "Invalid usage`", sizeof("Invalid usage`"), 0);
        free_args(args);
        return;
    }

    printf("IN RUN_PROG FUNCTION\n");
    static char* base_dir = "./programs/";
    // check if cc works
    static char* base_command = "cc *.c -o ";
    char* program_dir = malloc(sizeof(base_dir) + sizeof(args[1]) + 2);
    strcpy(program_dir, base_dir);
    strcat(program_dir, args[1]);

    printf("FIRST MALLOC?\n");

    char* executable = malloc(sizeof(program_dir) + sizeof(args[1]) + 2);
    strcpy(executable, program_dir);
    strcat(executable, "/");
    strcat(executable, args[1]);
    
    printf("SECOND MALLOC?\n");

    printf("IMBEtWEEN 2 printf\n");

    // sanity checking
    printf("EXECUTABLE PATH: %s\n", executable);


    // going to remove the -f from the args by making it null on the client side
    // count the number of args
    int arg_count = 0; 
    while (args[arg_count] != NULL)
    {
        arg_count ++;
    }

    arg_count -=2; // first 2 args are run, progname 

    char* program_args = malloc(sizeof(ARG_BUFFER_SIZE) * arg_count + arg_count); // + arg count for spaces

    for (int i = 2; i < arg_count; i ++)
    {
        strcat(program_args, args[i]);
        strcat(program_args, " ");
        // remember the null character - might need to remove it
    }


    int executable_exists = 0;
    // CHANGE THIS to 0 maybe
    int compile_needed = 1;
    
    // check if program needs to be compiled
    // could just do this with grep, but doing the C way for the assignment :)
    struct dirent *de;

    DIR *dr = opendir(program_dir);

    if (dr == NULL)
    {
        printf("PROGRAM FOLDER DOESNT EXIST\n");
        send(client_sock, "Program does not exist`", sizeof("Program does not exists`"), 0);
        return;
    }

    // iterate through the directory and see if an executable name the same as the program exists
    while ((de = readdir(dr)) != NULL)
    {
        if (strcmp(de->d_name, args[1]) == 0)
        {
            executable_exists = 1;
            break;
        }
    }

    // need to check the date of the executable vs the source files
    if (executable_exists)
    {
        // check if its newer than any of the source files
        compile_needed = 1;
        // we having 1 as the default so check the opposite and set to 0
    }

    if (compile_needed)
    {
        char* command = malloc(sizeof(base_command) + sizeof(args[1]) + 2);
        strcpy(command, base_command);
        strcat(command, args[1]);

        FILE* f = popen(command, "r");
        fclose(f);

        // maybe check if it worked?
    }

    // now actually run the program
    printf("\nYO bIG NIBBA IM RUNNING THE FILE NOW\n");
    // FILE* f = popen(executable, "r");

    // fclose(f);

    // need to use exec or something

// sonmething like this    exec(program_dir, program_args);
// actually maybe popen so we can the output and send back to user


    // do the line by line and finish with the terminator character, dont forget to sleep

    closedir(dr);


    free(program_dir);
    free(executable);
    free_args(args);
}


void print_args(char** args)
{
    int pos = 0;

    while (args[pos] != NULL)
    {
        printf("[%d] %s\n", pos, args[pos]);
        pos ++;
    }
    printf("\n");
}

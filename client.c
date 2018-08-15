#include <stdio.h>
#include <string.h>

#define LINE_LENGTH 1024
#define N_ARGS 10
#define ARG_LENGTH 1024
#define PORT 80
#define IP "localhost"

int read_line(char user_input[N_ARGS][ARG_LENGTH], char* line)
{
    int arg_count = 0;
    char* token;

    token = strtok(line, " ");

    while (token != NULL)
    {
        strcpy(user_input[arg_count], token);

        printf("%s\n", token);
        token = strtok(NULL, " ");
        arg_count ++;
    }

    return arg_count;
}

void print_args(char user_input[N_ARGS][ARG_LENGTH], int arg_count)
{
    for (int i = 0; i < arg_count; i ++)
    {
        printf("%s\n", user_input[i]);
    }
}

int main(int argc, char ** argv)
{

    /* 5 args of 1024 long */
    char user_input[N_ARGS][ARG_LENGTH];
    char line[LINE_LENGTH];
    int arg_count = 0;

    while (1)
    {
        fgets(line, LINE_LENGTH, stdin);
        arg_count = read_line(user_input, line);
        /* print_args(user_input, arg_count); */

        /* need to use these ugly way, otherwise can hash the string and do switch case */

        if (strcmp(user_input[0], "quit") == 0)
        {
            return 0;
        }
        else if (strcmp(user_input[0], "put") == 0)
        {
            printf("PUT: \n");
        }
        else if (strcmp(user_input[0], "get") == 0)
        {
            printf("GET: \n");
        }
        else if (strcmp(user_input[0], "run") == 0)
        {
            printf("RUN: \n");
        }
        else if (strcmp(user_input[0], "list") == 0)
        {
            printf("LIST: \n");
        }
        else if (strcmp(user_input[0], "test") == 0)
        {
            printf("TEST: \n");
        }


    }

    printf("%s\n", line);



    return 0;
}

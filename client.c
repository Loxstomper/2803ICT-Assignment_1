#include "common.h"
#include "client.h"
#include "client_func.h"


/* use signals to check for ctrl+c and run quit function */


int main(int argc, char ** argv)
{
    
    /* if (argc != 2) */
    /* { */
    /*     printf("./client IP\n"); */
    /*     return 1; */
    /* } */


    char   message[BUFFER_SIZE];
    char   reply[BUFFER_SIZE];
    int    sock;
    char** args;

    struct sockaddr_in server;
    size_t reply_length;
    pid_t child_pid;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 1)
    {
        perror("Could not create socket\n");
        exit(1);
    }

    printf("Created socket\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* server.sin_addr.s_addr = inet_addr(argv[1]); */
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // ----- parent connecting to see if valid server -------- //
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        perror("Connection Failed!\n");
        exit(1);
    }

    printf("Connected\n");

    /* need to fork on after each submission */
    while (1)
    {
        printf("\nRequest: ");
        fgets(message, BUFFER_SIZE, stdin);

        /* printf("BUFFER: %s\n", message); */
        args = get_args(message);

        // user just pressed enter, no command
        if (args[0] == NULL)
        {
            continue;
        }

        /* fork here */
        // create new connect

        // some of the functions require the client to do stuff
        if (strcmp(args[0], "put") == 0)
        {
            printf("{PUT CLIENT\n");
            put(sock, args);
        }
        else if (strcmp(args[0], "get") == 0)
        {
            printf("GET CLIENT\n");
            get(sock, message);
            free(args);
        }
        else if (strcmp(args[0], "run") == 0)
        {
            printf("RUN CLIENT");
            run(sock, message, args);
        }
        else if (strcmp(args[0], "quit") == 0)
        {
            free(args);
            send(sock, "quit", strlen("quit"), 0);
            exit(1);
        }
        /* commands that are purely server sided */
        /* e.g. sys */
        else
        {
            /* free(args); */
            free_args(args);

            if (send(sock, message, strlen(message), 0) < 0)
            {
                perror("Send Failed!\n");
                exit(1);
            }

            /* ----------------- getting output -------------------------- */
            /* usleep(1000); */
            read_data(sock);
        }

        // output finished
        memset(&message, '\0', strlen(message));
        memset(&reply, '\0', strlen(reply));

        
    }

    // close the socket once the client has exited
    close(sock);

    return 0;
}

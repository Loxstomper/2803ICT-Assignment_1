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

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        printf("Could not create socket\n");
        return -1;
    }

    printf("Created socket\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    /* server.sin_addr.s_addr = inet_addr(argv[1]); */
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        printf("Connection Failed!\n");
        return -1;
    }

    printf("Connected\n");

    /* need to fork on after each submission */
    while (1)
    {
        printf("\nRequest: ");
        fgets(message, BUFFER_SIZE, stdin);

        args = get_args(message);

        // some of the functions require the client to do stuff
        if (strcmp(args[0], "put") == 0)
        {
            put(sock, args);
        }
        else if (strcmp(args[0], "get") == 0)
        {
            get(sock, message);
            free(args);
        }
        else if (strcmp(args[0], "run") == 0)
        {
            run(sock, message, args);
        }
        else if (strcmp(args[0], "quit") == 0)
        {
            free(args);
            break;
        }
        /* commands that are purely server sided */
        /* e.g. sys */
        else
        {
            free(args);

            if (send(sock, message, strlen(message), 0) < 0)
            {
                printf("Send Failed!\n");
                exit(1);
            }

            reply_length = recv(sock, reply, BUFFER_SIZE, 0);

            if (reply_length < 0)
            {
                printf("Recv Failed!");
                exit(1);
            }

            printf("\n");

            while (reply_length > 0)
            {
                printf("%s", reply);
                memset(&reply, '\0', strlen(reply));
                reply_length = recv(sock, reply, BUFFER_SIZE, 0);
            }
        }

        // output finished
        memset(&message, '\0', strlen(message));
        memset(&reply, '\0', strlen(reply));
    }

    //clean up zombies?

    close(sock);

    return 0;
}

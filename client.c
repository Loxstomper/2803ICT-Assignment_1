#include "common.h"
#include "client.h"


/* use signals to check for ctrl+c and run quit function */


int main(int argc, char ** argv)
{
    
    /* if (argc != 2) */
    /* { */
    /*     printf("./client IP\n"); */
    /*     return 1; */
    /* } */


    char message[BUFFER_SIZE];
    char reply[BUFFER_SIZE];
    int sock;

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

        if (send(sock, message, strlen(message), 0) < 0)
        {
            printf("Send Failed!\n");
            return -1;
        }

        memset(&message, '\0', strlen(message));

        if ((strcmp(message, "quit\n")) == 0)
        {
            break;
            printf("IM QUITTING");
            /* clean up zombies */
        }

        reply_length = recv(sock, reply, BUFFER_SIZE, 0);

        if (recv(sock, reply, BUFFER_SIZE, 0) == -1)
        {
            printf("Recv Failed!\n");
            exit(1);
        }

        do {
            printf("%s", reply);
            memset(&reply, '\0', strlen(reply));
            reply_length = recv(sock, reply, BUFFER_SIZE, 0);
        }
        while (reply_length > 0);

        printf("\n");

        /* printf("\n%s\n", reply); */
        /* memset(&reply, '\0', strlen(reply)); */
        /* memset(&message, '\0', strlen(message)); */
    }

    close(sock);

    return 0;
}

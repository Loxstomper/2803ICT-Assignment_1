#include "common.h"
#include "client.h"
#include "client_func.h"

int main(int argc, char ** argv)
{
    
    if (argc != 2) 
    { 
         printf("./Client IP\n");
         return 1;
    } 

    char    message[BUFFER_SIZE], reply[BUFFER_SIZE];
    int     sock;
    char**  args;
    clock_t start_time, end_time;
    double  duration;
    struct  timeval before, after;

    
    struct sockaddr_in server;
    size_t reply_length;
    pid_t child_pid;

    /* no zombies */
    signal(SIGCHLD, SIG_IGN);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 1)
    {
        perror("Could not create socket\n");
        exit(1);
    }

    printf("Created socket\n");

    // server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // ----- parent connecting to see if valid server -------- //
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        perror("Connection Failed!\n");
        exit(1);
    }

    printf("Connected\n");

    printf("\nRequest: ");
    while (1)
    {
        // printf("\nRequest: ");
        fgets(message, BUFFER_SIZE, stdin);

        /* printf("BUFFER: %s\n", message); */
        args = get_args(message);

        // user just pressed enter, no command
        if (args[0] == NULL)
        {
            continue;
        }
        else if (strcmp(args[0], "quit") == 0)
        {
            send(sock, "quit", strlen("quit"), 0);
            exit(1);
        }


        /* fork here */
        if (child_pid = fork() == 0)
        {
            // new socket descriptor - so the parent can stay connected
            int new_sock;
            if ((new_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            {
                perror("Socket descriptor error\n");
                exit(1);
            }

            // new child connection
            if (connect(new_sock, (struct sockaddr*)&server, sizeof(server)) < 0)
            {
                perror("Connection Failed!\n");
                exit(1);
            }

            start_time = clock();
            gettimeofday(&before, NULL);

            if (strcmp(args[0], "put") == 0)
            {
                // printf("{PUT CLIENT\n");
                put_client(new_sock, message, args);
            }
            else if (strcmp(args[0], "get") == 0)
            {
                // printf("GET CLIENT\n");
                get_client(new_sock, message);
                free_args(args);
            }
            else if (strcmp(args[0], "run") == 0)
            {
                // printf("RUN CLIENT");
                printf("Just a heads up it will save output to file, but wont print to the screen this is explained in server_func.c in the prog_run server\n\
                just type the next command you want\n");
                run_prog_client(new_sock, message, args);
            }

            /* commands that are purely server sided */
            /* e.g. sys  and list */
            else
            {
                free_args(args);

                if (send(new_sock, message, strlen(message), 0) < 0)
                {
                    perror("Send Failed!\n");
                    exit(1);
                }

                /* ----------------- getting output -------------------------- */
                /* usleep(1000); */
                read_data(new_sock);
            }

            end_time = clock();
            gettimeofday(&after, NULL);
            duration = (double) (end_time - start_time) / CLOCKS_PER_SEC;
            printf("\nCPU Time: %lf \n", duration);

            printf("Time    : %f\n",
            (double) (after.tv_usec - before.tv_usec) / 1000000 +
            (double) (after.tv_sec - before.tv_sec));

            // output finished - this isnt needed because its a child
            memset(&message, '\0', strlen(message));
            memset(&reply, '\0', strlen(reply));

            send(sock, "quit", strlen("quit"), 0);
            close(sock);

            // fixing up ui
            printf("\nRequest: ");

            exit(1);
        }
    }

    // close the socket once the client has exited
    close(sock);

    return 0;
}

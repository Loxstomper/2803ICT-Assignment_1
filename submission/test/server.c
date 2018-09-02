#include "common.h"
#include "server_func.h"

/* change error printf to perror */

/* global variable :( */
/* have to for the sig handler */
int server_sock;

void sigintHandler(int sig_num)
{
    printf("\nI HAVE BEEN CLOSED\n");

    /* close the socket, clients are still connected though */
    close(server_sock);
    exit(1);
}

int main(int argc, char ** argv)
{
    /* ctrl+c */
    signal(SIGINT, sigintHandler);

    /* no zombies */
    signal(SIGCHLD, SIG_IGN);
    /* look at rubens code slide 57 */

	struct sockaddr_in server_addr;

	int client_sock;
	struct sockaddr_in client_addr;

	socklen_t addr_size;

	char buffer[BUFFER_SIZE];
    char** args;
	pid_t childpid;


	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	if (server_sock < 0)
    {
		perror("Server socket creation error.\n");
		exit(1);
	}
	printf("Server socket created.\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* allows binding to already open port */
    int yes=1;
    if (setsockopt(server_sock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1) 
    {
        perror("setsockopt");
        exit(1);
    } 


    int test;
    test = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)); 

    if(test < 0)
    {
		perror("Bind error.\n"); 
		exit(1);
	}

	printf("Binded to port %d\n", PORT);

	if (listen(server_sock, 10) == 0)
    {
		printf("Listening...\n\n");
	}
    else
    {
		perror("Listen error.\n");
        exit(1);
	}


    // constantly accept new connections
	while (1)
    {
		client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);

        // no connectiong
		if(client_sock < 0)
        {
            // perror("client connection error\n");
			/* exit(1); */
            continue;
		}

		printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));



        // fork, wait for data and send response then quit
        if((childpid = fork()) == 0)
        {
            close(server_sock);

            recv(client_sock, buffer, BUFFER_SIZE, 0);

            /* print what the user sent */
            printf("%s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

            /* split into args */
            args = get_args(buffer);

            if (DEBUG)
            {
                print_args(args);
            }

            if(strcmp(args[0], "quit") == 0)
            {
                printf("%s:%d disconnected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                // exit(1);
                break;
            }


            // command list
            if (strcmp(args[0], "list") == 0)
            {
                printf("LIST\n");
                list(client_sock, args);
            }
            else if (strcmp(args[0], "put") == 0)
            {
                printf("PUT\n");
                put_prog(client_sock, args);
            }
            else if (strcmp(args[0], "run") == 0)
            {
                printf("RUN\n");
                run_prog(client_sock, args);
            }
            else if (strcmp(args[0], "test") == 0)
            {
                printf("TEST CODE\n");
                strcpy(buffer, "test response from server`");
                send(client_sock, buffer, strlen(buffer), 0);
            }
            else if (strcmp(args[0], "get") == 0)
            {
                get(client_sock, args);
            }
            else if (strcmp(args[0], "sys") == 0)
            {
                sys(client_sock);
            }
            else if (strcmp(args[0], "loop") == 0)
            {
                strcpy(buffer, "loop\n\0");
                while (1)
                {
                    send(client_sock, buffer, strlen(buffer), 0);
                }
            }
            // was not in the command list so it is unknown
            else
            {
                printf("INVALID COMMAND\n");
                send(client_sock, "Invalid command`", strlen("Invalid command`"), 0);
            }                    
            
            // clean up the input buffer
            memset(&buffer, '\0', strlen(buffer));
            close(client_sock);
            exit(1);
        }
    }
    close(client_sock);

	return 0;
}

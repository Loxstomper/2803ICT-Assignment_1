#include "common.h"
#include "server_func.h"


/* change error printf to perror */

/* global variable :( */
/* ave to for the sig handler */
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
    signal(SIGINT, sigintHandler);


	//int server_sock;
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
		printf("Server socket creation error.\n");
		exit(1);
	}
	printf("Server socket created.\n");

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

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
		printf("Bind error.\n"); 
		exit(1);
	}

	printf("Binded to port %d\n", PORT);

	if (listen(server_sock, 10) == 0)
    {
		printf("Listning...\n\n");
	}
    else
    {
		printf("Bind error.\n");
        exit(1);
	}


	while (1)
    {
		client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);

		if(client_sock < 0)
        {
			exit(1);
		}

		printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        memset(&buffer, '\0', 0);

        /* no zombies */
        signal(SIGCHLD, SIG_IGN);

        /* this whole if used to be in a while loop */
        while (1)
        {
        /* no need to have inner loop, the forked process only does one thing */
            /* i think ^ this is wrong, and I need the loop */
            if((childpid = fork()) == 0)
            {
                close(server_sock);

                recv(client_sock, buffer, BUFFER_SIZE, 0);

                /* print what the user sent */
                printf("%s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

                /* split into args */
                args = get_args(buffer);


                if(strcmp(args[0], "quit") == 0)
                {
                    printf("%s:%d disconnected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    break;
                }


                if (strcmp(args[0], "list") == 0)
                {
                    printf("LIST\n");
                    list(client_sock, args);
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
                else
                {
                    char end = 4;
                    strcpy(buffer, "unknown command");
                    buffer[strlen(buffer)] = end;
                    buffer[strlen(buffer)] = '\0';

                    send(client_sock, buffer, strlen(buffer), 0);
                }                    
                
                memset(&buffer, '\0', strlen(buffer));

                close(client_sock);
            }

        }
    }

	return 0;
}

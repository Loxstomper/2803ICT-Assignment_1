#include "common.h"
#include "server_func.h"

int server_sock;

void sigintHandler(int sig_num)
{
    printf("\nI HAVE BEEN CLOSED\n");
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

    int test;

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

		if((childpid = fork()) == 0)
        {
			close(server_sock);

			while(1)
            {
				recv(client_sock, buffer, BUFFER_SIZE, 0);
                args = get_args(buffer);

				if(strcmp(args[0], "quit") == 0)
                {
					printf("%s:%d disconnected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    break;
				}

                printf("%s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

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
                    while (1)
                    {
                        strcpy(buffer, "loop");
                        send(client_sock, buffer, strlen(buffer), 0);
                    }
                }
                else
                {
                    strcpy(buffer, "unknown command");
                    send(client_sock, buffer, strlen(buffer), 0);
                }                    
                
                memset(&buffer, '\0', strlen(buffer));
			}

            close(client_sock);
		}

	}

	return 0;
}

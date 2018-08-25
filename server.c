#include "common.h"

/* make buffer size in the common */

/* put this in seperate file */
void list(int client_sock, char buffer[1024])
{
    FILE* f = popen("ls", "r");

    while (fgets(buffer, 1024, f) != NULL)
    {
        send(client_sock, buffer, strlen(buffer), 0);
    }
}

void get(int client_sock, char buffer[1024])
{
    FILE* f = popen("cat client.c", "r");

    while (fgets(buffer, 1024, f) != NULL)
    {
        send(client_sock, buffer, strlen(buffer), 0);
    }
}

void sys(int client_sock, char buffer[1024])
{
    FILE* f = popen("uname -sr && cat /proc/cpuinfo | grep 'model name' | uniq && uname -p", "r");

    while (fgets(buffer, 1024, f) != NULL)
    {
        send(client_sock, buffer, strlen(buffer), 0);
    }
}

int main(int argc, char ** argv)
{

	int server_sock;
	struct sockaddr_in server_addr;

	int client_sock;
	struct sockaddr_in client_addr;

	socklen_t addr_size;

	char buffer[1024];
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

    test = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)); 

    /* if(test < 0); */
    /* { */
		/* printf("Bind error.\n"); */
		/* exit(1); */
	/* } */

	printf("Binded to port %d\n", PORT);

	if (listen(server_sock, 10) == 0)
    {
		printf("Listning...\n");
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

		printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        memset(&buffer, '\0', 0);

		if((childpid = fork()) == 0)
        {
			close(server_sock);

			while(1)
            {
				recv(client_sock, buffer, 1024, 0);

				if(strcmp(buffer, "quit\n") == 0)
                {
					printf("Disconnected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    break;
				}
                else
                {
					printf("%s:%d: %s\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buffer);

                    if (strcmp(buffer, "list\n") == 0)
                    {
                        list(client_sock, buffer);
                    }
					else if (strcmp(buffer, "get\n") == 0)
                    {
                        get(client_sock, buffer);
                    }
                    else if (strcmp(buffer, "sys\n") == 0)
                    {
                        sys(client_sock, buffer);
                    }
                    else
                    {
                        send(client_sock, buffer, strlen(buffer), 0);
                    }                    
                    
                    memset(&buffer, '\0', strlen(buffer));
				}
			}

            close(client_sock);
		}

	}

	return 0;
}

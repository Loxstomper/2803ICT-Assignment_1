#include "server_func.h"

void list(int client_sock, char buffer[BUFFER_SIZE])
{
    FILE* f = popen("ls", "r");

    while (fgets(buffer, 1024, f) != NULL)
    {
        send(client_sock, buffer, strlen(buffer), 0);
        memset(buffer, '\0', strlen(buffer));
    }
}

void get(int client_sock, char buffer[BUFFER_SIZE])
{
    /* change this just to read the file for cross platform */ 
    FILE* f = popen("cat client.c", "r");

    while (fgets(buffer, BUFFER_SIZE, f) != NULL)
    {
        send(client_sock, buffer, strlen(buffer), 0);
        memset(buffer, '\0', strlen(buffer));
    }
}

void sys(int client_sock, char buffer[BUFFER_SIZE])
{
    FILE* f = popen("uname -sr && cat /proc/cpuinfo | grep 'model name' | uniq && uname -p", "r");

    while (fgets(buffer, BUFFER_SIZE, f) != NULL)
    {
        send(client_sock, buffer, strlen(buffer), 0);
        memset(buffer, '\0', strlen(buffer));
    }
}

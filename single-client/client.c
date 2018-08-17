#include "common.h"

#include <stdlib.h>
#include <math.h>

void TimerStart(struct timespec* before)
{
    clock_gettime(CLOCK_MONOTONIC, before);
}

unsigned long TimerStop(struct timespec* before)
{
    struct timespec after;
    
    clock_gettime(CLOCK_MONOTONIC, &after);

    /* unsigned long ms; */
    /* time_t s; */

    /* s = (after.tv_sec - before->tv_sec); */
    /* ms = ((after.tv_nsec - before->tv_nsec) / 1.0e6); */

    /* if (ms > 999) */
    /* { */
    /*     ms = 0; */
    /* } */

    /* return ms; */

    return ((after.tv_nsec - before->tv_nsec) / 1.0e6);
}

int main(int argc, char ** argv)
{
    struct timespec before;
    unsigned long delay;
    char message[1024];
    char reply[2048];
    int sock;

    struct sockaddr_in server;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock == -1)
    {
        printf("Could not create socket\n");
        return -1;
    }

    printf("Created socket\n");

    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons(9999);

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        printf("Connection Failed!\n");
        return -1;
    }

    printf("Connected\n");

    while (1)
    {
        printf("\nRequest: ");
        scanf("%s", message);

        /* start timer */
        TimerStart(&before);

        if (send(sock, message, strlen(message), 0) < 0)
        {
            printf("Send Failed!\n");
            return -1;
        }

        if (recv(sock, reply, 2048, 0) < 0)
        {
            printf("Recv Failed!\n");
            return -1;
        }

        /* stop timer */
        delay = TimerStop(&before);

        printf("Reply: %s \n", reply);
        printf("Delay: %lu \n", delay);

        memset(&reply, '\0', strlen(reply));
    }

    close(sock);

    return 0;
}

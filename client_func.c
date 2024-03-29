#include "client_func.h"
#include "common.h"
#include <stdio.h>
#include <dirent.h>

// will read data from server until the special terminating character is present
void read_data(int sock)
{
    printf("IN READ DATA FUNCTION\n");

    char* buffer = malloc(BUFFER_SIZE * sizeof(char));
    static int reply_length;

    // memset(buffer, '\0', 0);

    reply_length = recv(sock, buffer, BUFFER_SIZE, 0);
    printf("%s", buffer);
    memset(buffer, '\0', strlen(buffer));

    while (reply_length > 0 && buffer[strlen(buffer) -1] != '`')
    {
        // this is really bad

        int i = 0;

        for (int i = 0; i < BUFFER_SIZE && buffer[i] != '\0'; i ++)
        {
            if (buffer[i] == '`')
            {
                printf("\nEND\n");
            }
        }





        reply_length = recv(sock, buffer, BUFFER_SIZE, 0);


        /* check if < 0 */


        if (reply_length < 2 && buffer[reply_length -1] == '`')
        {
            buffer[reply_length -1] = '\0';
            printf("%s", buffer);
            break;
        }
        else
        {
            printf("%s", buffer);
        }

        memset(buffer, '\0', strlen(buffer));

    }

    free(buffer);
}

// put function
void put(int sock, char** args)
{
    printf("PUT FUNCTION ON CLIENT\n");

    free(args);
}

// get function
void get(int sock, char message[BUFFER_SIZE])
{
    printf("GET FUNCTION ON CLIENT\n");
    int line_count = 1;
    int reply_length = 0;

    char* buffer = malloc(BUFFER_SIZE * sizeof(char));

    char tmp;


    /* send that we want to get the file contents */
    send(sock, message, strlen(message), 0);

    reply_length = recv(sock, buffer, BUFFER_SIZE, 0);

    /* check if < 0 */

    printf("\n%s", buffer);
    /* memset(&buffer, '\0', strlen(buffer)); */

    /* read the content */
    /* while (reply_length == BUFFER_SIZE - 1) */
    while (reply_length > 0 && buffer[strlen(buffer) -1] != '`')
    {
        if (line_count % 40 == 0)
        {
            printf("[MORE]\n");
            /* tmp = getchar(); */
            getchar();
            /* scanf(""); */
            send(sock, "^", sizeof("^"), 0);
        }

        reply_length = recv(sock, buffer, BUFFER_SIZE, 0);


        /* check if < 0 */


        if (reply_length < 2 && buffer[reply_length -1] == '`')
        {
            buffer[reply_length -1] = '\0';
            printf("%s", buffer);
            break;
        }
        else
        {
            printf("%s", buffer);
        }

        memset(buffer, '\0', strlen(buffer));

        line_count ++;
    }

    free(buffer);


}

// run function
void run(int sock, char message[BUFFER_SIZE], char** args)
{
    printf("RUN FUNCTION ON CLIENT\n");

    /* figure out if going to print the output or save to file */
    int count = 0;
    int save_to_file = 0;


    /* find position of last argument */
    while (args[count] != NULL)
    {
        count ++;
    }

    if (strcmp(args[count - 1], "-f") == 0)
    {
        save_to_file = 1;   
    }

    free(args);
}

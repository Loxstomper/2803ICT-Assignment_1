#include "client_func.h"
#include "common.h"
#include <dirent.h>

// will read data from server until the special terminating character is present
void read_data(int sock)
{
    // printf("IN READ DATA FUNCTION\n");

    char* buffer = malloc(BUFFER_SIZE * sizeof(char));
    static int reply_length;

    memset(buffer, '\0', BUFFER_SIZE - 1);

    printf("\n");

    reply_length = recv(sock, buffer, BUFFER_SIZE, 0);

    // check if this was the whole message
    if (buffer[strlen(buffer) - 1] == '`')
    {
        // printf("ONLY A SHORT MESSAGE\n");
        buffer[strlen(buffer) - 1] = '\0';
        printf("%s\n", buffer);
        free(buffer);
        return;
    }

    while (reply_length > 0)
    {
        if (buffer[strlen(buffer) - 1] == '`')
        {
            buffer[strlen(buffer) - 1] = '\0';
            printf("%s", buffer);
            break;
        }

        printf("%s", buffer);
        memset(buffer, '\0', strlen(buffer));

        reply_length = recv(sock, buffer, BUFFER_SIZE, 0);
    }

    free(buffer);
}

// put function
void put_client(int sock, char message[BUFFER_SIZE], char** args)
{
    printf("PUT FUNCTION ON CLIENT\n");

    char* buffer = malloc(sizeof(char) * BUFFER_SIZE);
    int arg_count;
    int file_count;
    int forced = 0;

    for (arg_count = 0; args[arg_count] != NULL; arg_count ++)
    {
        printf("[%d] %s\n", arg_count, args[arg_count]);
    }

    forced = (strcmp(args[arg_count - 1], "-f") == 0);
    file_count = arg_count - 2 - forced;

    printf("FILE COUNT %d\n", file_count);


    if (file_count < 1)
    {
        printf("Invalid usage\n");
        return;
    }

    FILE* fp;
    // should also check to make sure all local files exists
    for (int i = 0; i < file_count; i ++)
    {
        // args[i + 2]
        fp = fopen(args[i + 2], "r");

        if (fp == NULL)
        {
            printf("%s does not exist\n", args[i + 2]);
            free(buffer);
            return;
        }

        fclose(fp);
    }

    // everything seems good
    send(sock, message, strlen(message), 0);
    recv(sock, buffer, BUFFER_SIZE, 0);

    if (strcmp(buffer, "ok") != 0)
    {
        printf("Program directory already exists\n");
        free(buffer);
        return;
    }

    printf("BUFFER: %s\n", buffer);

    // now send contents to server


    free_args(args);
}
// get function
// void get_client(int sock, char message[BUFFER_SIZE])
// {
//     printf("GET FUNCTION ON CLIENT\n");
//     int line_count = 1;
//     int reply_length = 0;

//     char* buffer = malloc(BUFFER_SIZE * sizeof(char));

//     char tmp;


//     /* send that we want to get the file contents */
//     send(sock, message, strlen(message), 0);

//     reply_length = recv(sock, buffer, BUFFER_SIZE, 0);

//     /* check if < 0 */

//     printf("\n%s", buffer);
//     /* memset(&buffer, '\0', strlen(buffer)); */

//     /* read the content */
//     /* while (reply_length == BUFFER_SIZE - 1) */
//     while (reply_length > 0 && buffer[strlen(buffer) -1] != '`')
//     {
//         if (line_count % 40 == 0)
//         {
//             printf("[MORE]\n");
//             /* tmp = getchar(); */
//             getchar();
//             /* scanf(""); */
//             send(sock, "^", sizeof("^"), 0);
//         }

//         reply_length = recv(sock, buffer, BUFFER_SIZE, 0);


//         /* check if < 0 */


//         if (reply_length < 2 && buffer[reply_length -1] == '`')
//         {
//             buffer[reply_length -1] = '\0';
//             printf("%s", buffer);
//             break;
//         }
//         else
//         {
//             printf("%s", buffer);
//         }

//         memset(buffer, '\0', strlen(buffer));

//         line_count ++;
//     }

//     free(buffer);


// }


// get function
void get_client(int sock, char message[BUFFER_SIZE])
{
    printf("GET FUNCTION ON CLIENT\n");
    int line_count = 1;
    int reply_length = 0;

    char* buffer = malloc(BUFFER_SIZE * sizeof(char));

    /* send that we want to get the file contents */
    send(sock, message, strlen(message), 0);    
    

    reply_length = recv(sock, buffer, BUFFER_SIZE, 0);

    // check if this was the whole message
    if (buffer[strlen(buffer) - 1] == '`')
    {
        // printf("ONLY A SHORT MESSAGE\n");
        buffer[strlen(buffer) - 1] = '\0';
        printf("%s\n", buffer);
        free(buffer);
        return;
    }

    /* read the content */
    while (reply_length > 0)
    {
        // end of message
        if (buffer[strlen(buffer) - 1] == '`')
        {
            // printf("Looks like we are done here bois\n");
            buffer[strlen(buffer) - 1] = '\0';
            printf("%s", buffer);
            break;
        }
        
        // wait every 40 lines
        printf("[%d] ", line_count % 40);
        if (line_count % 40 == 0)
        {
            printf("[MORE]\n");
            // blocking functions
            getchar();

            // remove the more from the screen

            for(int i = 0; i < 6; i ++)
            {
                printf("\b");
            }

            // tell the server to resume
            send(sock, "^", sizeof("^"), 0);
        }

        printf("%s", buffer);
        memset(buffer, '\0', strlen(buffer));

        reply_length = recv(sock, buffer, BUFFER_SIZE, 0);
        line_count ++;
    }

    free(buffer);
}

// run function
void run_prog_client(int sock, char message[BUFFER_SIZE], char** args)
{
    printf("RUN FUNCTION ON CLIENT\n");

    char* buffer = malloc(sizeof(char) * BUFFER_SIZE);

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
        // removing the -f arg from the message
        // assuming no trailing whitespace
        message[strlen(message) - 3] = '\0';
    }

    // send message the the server
    send(sock, message, strlen(message), 0);

    // recv(sock, buffer, BUFFER_SIZE, 0);
    if (!save_to_file)
    {
        read_data(sock);
    }

    printf("%s\n", buffer);


    free(args);
}

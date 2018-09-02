#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{

    printf("ARGV checker\n");

    /* sleep(5); */

    for (int i = 0; i < argc; i ++)
    {
        printf("[%d] %s\n", i, argv[i]);
    }

    printf("\n");

    return 0;
}

#include "common.h"

int main(int argc, char ** argv)
{
    char output[1024];

    system("ls");

    fgets(output, 1024, stdout);

    printf("OUTPUT: %s\n", output);


    return 0;
}

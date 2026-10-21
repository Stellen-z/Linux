#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    printf("program start..\n");
    execl("/usr/bin/ls","ls","-l",NULL);
    printf("program end...\n");
    exit(1);
    // return 0;
}

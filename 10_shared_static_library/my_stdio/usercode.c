#include "mystdio.h"
#include <string.h>
#include <unistd.h>
#include "mystrlen.h"

int main()
{
    MyFile *filep = MyFopen("./log.txt", "a");
    if(!filep)
    {
        printf("fopen error!\n");
        return 1;
    }

    int cnt = 10;
    while(cnt--)
    {
        char *msg = (char*)"hello myfile!!!\n";
        MyFwrite(filep, msg, strlen(msg));
        MyFFlush(filep);
        printf("buffer: %s", filep->outbuffer);
        sleep(1);
    }

    printf("mystrlen:%d\n",mystrlen("hello world!\n"));

    MyFclose(filep); // FILE *fp
    return 0;
}


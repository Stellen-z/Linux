#include "myprocess.h"
#include <string.h>
#include <unistd.h>

#define NUM 101
#define STYLE '#'

void FlushProcess(double total,double cur)
{
    char buffer[NUM];
    memset(buffer,0,sizeof(buffer));
    
    const char* lable = "|/-\\";
    int len = strlen(lable);
    
    //填充#
    int real = (int)(cur*100 / total);
    for(int i = 0;i < real;i++)
    {
        buffer[i] = STYLE;
    }
    
    double rate = cur/total;
    static int cnt = 0;

    printf("[%-100s][%.1f%%][%c]\r",buffer,rate*100,lable[cnt++%len]);
    fflush(stdout);
    usleep(5000);

    
}

void Processbar()
{
    char buffer[NUM];
    memset(buffer,0,sizeof(buffer));

    const char* lable = "|/-\\";
    int len = strlen(lable);

    int cnt = 0;
    while(cnt <= 100)
    {
        printf("[%-100s][%d%%][%c]\r",buffer,cnt,lable[cnt%len]);
        fflush(stdout);
        buffer[cnt] = STYLE;
        ++cnt;
        usleep(50000);
    }
    printf("\n");
}

#include "process.h"

#define NUM 101

const static double total = 1024.0;
static double speed = 1.0;

static int cnt = 0;

void Process_v1()
{
    char buffer[NUM+1];
    memset(buffer, 0, sizeof(buffer));
    const char label[] = "/|-\\";
    int len = strlen(label);

    while(cnt <= 100)
    {
        printf("[%-100s][%d%%][%c]\r",buffer,cnt,label[cnt%len]);
        buffer[cnt++] = '#';
        fflush(stdout);
        // usleep(50000);
        sleep(1);
    }
    printf("\n");
}

void Process_v2(double total,double current)
{
    char buffer[NUM+1];
    memset(buffer, 0, sizeof(buffer));
    const char label[] = "/|-\\";
    int len = strlen(label);

    int rate = (int)current * 100 / total;
    for(int i = 0;i < rate;i++)
    {
        buffer[i] = '#';
    }

    cnt %= len;
    printf("[%-100s][%d%%][%c]\r",buffer,rate,label[cnt++]);
    fflush(stdout);
}

void DownLoad()
{
    cnt = 0;
    double current = 0;
    while(current <= total)
    {
        Process_v2(total,current);

        usleep(5000);
        current += speed;
    }
    printf("\ndownload %.2lfMB done\n",current);
}

void UpLoad()
{
    cnt = 0;
    speed = 2.0;
    double current = 0;
    while(current <= total)
    {
        Process_v2(total,current);

        usleep(5000);
        current += speed;
    }
    printf("\nupload %.2lfMB done\n",current);
}

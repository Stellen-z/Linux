#include "myprocess.h"

double total = 1024.0;
double speed = 1.0;

void DownLoad()
{
    double cur = 0;
    while(cur <= total)
    {
        FlushProcess(total,cur);
        //Processbar(total,cur);
        cur += speed;
    }
    printf("\n");
}

int main()
{
    DownLoad();
    return 0;
}

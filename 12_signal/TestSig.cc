#include <iostream>
#include <unistd.h>
#include <signal.h>

void HanderSig(int sig)
{
    std::cout << "收到：" << sig << std::endl;
    // exit(13);
}

int main()
{
    for(int i = 1;i < 32;i++)
        signal(i,HanderSig);

    for(int i = 1;i < 32;i++)
    {
        if(i == 9 || i == 19) continue;

        raise(i);
    }

    int cnt = 0;
    while(1)
    {
        std::cout << "hello world, " << cnt << " ,pid: " << getpid() << std::endl;
        ++cnt;
        sleep(1);
    }
    return 0;
}
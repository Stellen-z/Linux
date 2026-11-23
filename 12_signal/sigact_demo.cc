#include <iostream>
#include <signal.h>
#include <unistd.h>

void handler(int sig)
{
    std::cout << "捕捉2号信息" << std::endl;
    while (1)
    {
        sigset_t pending;
        sigpending(&pending);
        for (int i = 31; i >= 1; i--)
        {
            if (sigismember(&pending,i))
                std::cout << "1";
            else 
                std::cout << "0";
        }
        std::cout << std::endl;
        sleep(1);
    }
    exit(0);
}

int main()
{
    struct sigaction act, oact; 
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT, &act, &oact);

    while (1)
    {
        std::cout << "hello world: " << getpid() << std::endl;
        sleep(1);
    }
    return 0;
}
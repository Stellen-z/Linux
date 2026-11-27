#include <iostream>
#include <signal.h>
#include <sys/syscall.h>


int main()
{
    sigset_t block,oblock;
    sigemptyset(&block);
    sigemptyset(&oblock);

    // 1.加入block信号集
    sigaddset(&block,SIGINT);
    sigaddset(&block,40);

    // 2.加入block表
    sigprocmask(SIG_BLOCK,&block,&oblock);

    std::cout << "进程pid: " << getpid() << std::endl;

    while(1)
    {
        pause();
    }

    return 0;
}

// void sigcb(int sig)
// {
//     std::cout << "收到信号: " << sig << std::endl;
// }   

// int main()
// {
//     struct sigaction act,oact;
//     act.sa_handler = sigcb;
//     sigemptyset(&act.sa_mask);
//     act.sa_flags = 0;

//     sigaction(SIGINT,&act,&oact);

//     while(1)
//     {
//         pause();
//     }
//     return 0;
// }


// void sigcb(int sig)
// {
//     std::cout << "收到信号: " << sig << std::endl;
// }   

// int main()
// {
//     signal(SIGINT,sigcb);

//     while(1)
//     {
//         pause();
//     }
//     return 0;
// }
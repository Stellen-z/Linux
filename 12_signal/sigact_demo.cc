#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void WaitAll(int sig)
{
    while (1)
    {
        pid_t n = waitpid(-1, nullptr, WNOHANG);
        if (n == 0)
        {
            // 当前所有退出的子进程全部回收
            break;
        }
        else if (n < 0)
        {
            std::cerr << "wait error" << std::endl;
            break;
        }
    }
    std::cout << "收到信号: " << sig << std::endl;
}

// 用信号回收子进程
int main()
{
    // signal(SIGCHLD, WaitAll);
    signal(SIGCHLD, SIG_IGN);

    for (int i = 1; i <= 10; i++)
    {
        pid_t id = fork();
        if (id == 0)
        {
            // child
            std::cout << "I am child" << std::endl;
            sleep(3);
            exit(1);
            // if(i <= 6) exit(1); // 退出其中几个，如果阻塞等待，父进程就会暂停
            // else pause();
        }
    }

    while (1)
    {
        sleep(1);

        std::cout << "I am father" << std::endl;
    }
    std::cout << "father exit" << std::endl;

    return 0;
}

// void handler(int sig)
// {
//     std::cout << "捕捉2号信息" << std::endl;
//     while (1)
//     {
//         sigset_t pending;
//         sigpending(&pending);
//         for (int i = 31; i >= 1; i--)
//         {
//             if (sigismember(&pending,i))
//                 std::cout << "1";
//             else
//                 std::cout << "0";
//         }
//         std::cout << std::endl;
//         sleep(1);
//     }
//     exit(0);
// }

// int main()
// {
//     struct sigaction act, oact;
//     act.sa_handler = handler;
//     sigemptyset(&act.sa_mask);
//     act.sa_flags = 0;

//     sigaction(SIGINT, &act, &oact);

//     while (1)
//     {
//         std::cout << "hello world: " << getpid() << std::endl;
//         sleep(1);
//     }
//     return 0;
// }
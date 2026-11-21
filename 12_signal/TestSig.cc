#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <functional>
#include <vector>
#include <sys/syscall.h>
#include <sys/wait.h>

// int main()
// {
//     pid_t id = fork();
//     if (id == 0)
//     {
//         sleep(2);
//         printf("hello bit\n");
//         printf("hello bit\n");
//         printf("hello bit\n");
//         printf("hello bit\n");
//         printf("hello bit\n");
//         int a = 10;
//         a /= 0;
//         printf("hello bit\n");

//         exit(1);
//     }
//     int status = 0;
//     waitpid(id, &status, 0);
//     printf("signal: %d, exit code: %d, core dump: %d\n",
//             (status)&0x7F,(status>>8)&0xFF,(status>>7)&0x1);
// }

int cnt = 0;
void PrintPending(sigset_t &pending)
{
    printf("我是一个进程：%d,cnt: %d\n", getpid(),cnt);
    for (int i = 31; i >= 1; i--)
    {
        if (sigismember(&pending, i))
            std::cout << "1";
        else
            std::cout << "0";
    }
    std::cout << std::endl;
}

void HandlerSig(int sig)
{

    std::cout << "*********************" << std::endl;
    std::cout << "递达" << sig << "信号" << std::endl;
    sigset_t pending;
    int n = sigpending(&pending);
    PrintPending(pending);
    std::cout << "*********************" << std::endl;
}

int main()
{
    // 1.屏蔽2号信号
    signal(SIGINT, HandlerSig);

    sigset_t block, oblock; // 必须初始化为有效状态
    sigemptyset(&block);
    sigemptyset(&oblock);

    sigaddset(&block, SIGINT); // 此时并没有成功设置block[2]
    sigprocmask(SIG_SETMASK, &block, &oblock);

    // 4.重复获取打印过程

    while (1)
    {
        // 2.获取pending信号集
        sigset_t pending;
        int m = sigpending(&pending);

        // 3.打印
        PrintPending(pending);

        if (cnt == 10)
        {
            // 恢复SIGINT的行为
            std::cout << "解除SIGINT的信号屏蔽" << std::endl;
            sigprocmask(SIG_SETMASK, &oblock, nullptr); //能收到
        }

        sleep(1);
        ++cnt;
    }

    return 0;
}

// void HandlerSig(int sig)
// {
//     std::cout << "hello sig: " << sig << std::endl;
//     // 解除限制
//     signal(2,SIG_DFL);
//     std::cout << "恢复处理动作!" << std::endl;
// }

// int main()
// {
//     signal(2,HandlerSig);
//     // signal(2,SIG_IGN);

//     while(1)
//     {
//         std::cout << "." << " pid: " << getpid() << std::endl;
//         sleep(1);
//     }

//     return 0;
// }

// using func_t = std::function<void()>;

// std::vector<func_t> funcs;

// /////////tasks/////////
// void Sched()
// {
//     std::cout << "我是进程调度" << std::endl;
// }
// void MemManger()
// {
//     std::cout << "我是周期性的内存管理，正在检查有没有内存问题" << std::endl;
// }
// void Fflush()
// {
//     std::cout << "我是刷新程序，我在定期刷新内存数据，到磁盘" << std::endl;
// }
// /////////////////////////

// // 每隔一秒，完成一些任务
// void HandlerSig(int sig)
// {
//     std::cout << "##############################" << std::endl;
//     for (auto f : funcs)
//         f();
//     std::cout << "##############################" << std::endl;
//     int n = alarm(1);
// }

// // alarm模拟操作系统
// int main()
// {
//     funcs.push_back(Sched);
//     funcs.push_back(MemManger);
//     funcs.push_back(Fflush);

//     signal(SIGALRM, HandlerSig);
//     alarm(1);

//     while (1)
//     {
//         pause();
//     }

//     return 0;
// }

// void HanderSig(int sig)
// {
//     // std::cout << "收到：" << sig << "cnt: " << cnt << std::endl;
//     std::cout << "收到：" << sig << std::endl;
//     exit(13);
// }

// int main()
// {
//     for (int i = 1; i < 32; i++)
//         signal(i, HanderSig);

//     alarm(1);

//     int cnt = 0;
//     while (1)
//     {
//         // std::cout << "cnt: " << cnt++ << std::endl;
//         // printf("cnt: %d\n",cnt++);
//         cnt++;
//     }
// }

// int main()
// {
//     for(int i = 1;i < 32;i++)
//         signal(i,HanderSig);

//     for(int i = 1;i < 32;i++)
//     {
//         if(i == 9 || i == 19) continue;

//         raise(i);
//     }

//     int cnt = 0;
//     while(1)
//     {
//         std::cout << "hello world, " << cnt << " ,pid: " << getpid() << std::endl;
//         ++cnt;
//     }
//     return 0;
// }
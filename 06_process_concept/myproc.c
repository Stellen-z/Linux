#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int main()
{
    printf("父进程开始运行, pid: %d\n", getpid());

    pid_t id = fork();

    if (id < 0)
    {
        perror("fork");
        return 1;
    }
    else if (id == 0)
    {
        // child
        while (1)
        {
            sleep(1);
            printf("我是一个子进程 !, 我的 pid: %d, 我的父进程 id: %d\n", getpid(), getppid());
        }
    }
    else
    {
        // father
        while (1)
        {
            sleep(1);
            printf("我是一个父进程 !, 我的 pid: %d, 我的父进程 id: %d\n", getpid(), getppid());
        }
    }

    return 0;
}

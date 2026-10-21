#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

//int main()
//{
//    printf("hello dsbjny\n");
//    return 0;
//}

int main()
{
    pid_t id = fork();
    
    if(id == 0)
    {
        int cnt = 5;
        while(cnt--)
        {
            printf("我是一个子进程，pid:%d,ppid:%d\n",getpid(),getppid());
            sleep(1);
        }
        exit(1);
        // printf("我是一个子进程，pid:%d,ppid%d\n",getpid(),getppid());
    }
    
    int status = 0;

    //pid_t rid = wait(NULL);
    pid_t rid = waitpid(-1,&status,0);
    printf("我是一个父进程，pid:%d,ppid:%d\n",getpid(),getppid());
    if(rid > 0)
    {
        // printf("wait success!,pid:%d,exit code:%d,exit signal:%d\n",rid,(status>>8)&0xff,status&0x7f);
        if(WIFEXITED(status))
            printf("wait success!,pid:%d,exit code:%d,exit signal:%d\n",rid,WEXITSTATUS(status),status&0x7f);
        else 
            printf("exit exception\n");
    }
   else 
   {
       printf("wait fail!:%d:%s\n",errno,strerror(errno));
   }
    sleep(5);

    return 0;
}

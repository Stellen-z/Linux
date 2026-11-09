#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>

void ChildWrite(int wfd)
{
    char buffer[1024] = {0};
    int cnt = 0;
    while(1)
    {
        snprintf(buffer,sizeof(buffer),"I am child, pid: %d, cnt: %d",getpid(),cnt++);
        write(wfd,buffer,strlen(buffer));
        sleep(1);
    }
}

void FatherRead(int rfd)
{
    char buffer[1024];
    while(1)
    {
        buffer[0] = 0;
        ssize_t n = read(rfd,buffer,sizeof(buffer));
        if(n > 0)
        {
            // n就是真实写入的字节数
            buffer[n] = 0;
            std::cout << "child say:" << buffer << std::endl;
        }
    }
}

int main()
{
    // 1.创建管道 
    int fds[2] = {0}; // fds[0] 读端 fds[1] 写端
    int n = pipe(fds);
    if(n < 0) std::cerr << "pipe error" << std::endl; 

    std::cout << "fds[0]:" << fds[0] << std::endl;
    std::cout << "fds[1]:" << fds[1] << std::endl;
    
    // 2.创建子进程
    pid_t id = fork();
    if(id == 0)
    {
        // child
        close(fds[0]);
        ChildWrite(fds[1]);
        close(fds[1]);
        exit(1);
    }

    // father
    close(fds[1]);
    FatherRead(fds[0]);
    close(fds[0]);
    waitpid(id,nullptr,0);

    return 0;
}
#ifndef __PROCESS_POOL_HPP__
#define __PROCESS_POOL_HPP__

#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>

class Channel
{
public:
    Channel() {};
    ~Channel() {};

private:
    int wfd;
};

class ChannerManger
{
public:
    ChannerManger() {};
    ~ChannerManger() {};

private:
    std::vector<Channel> _channels;
};

class ProcessPool
{
public:
    ProcessPool(int num) : _process_num(num)
    {
    }

    bool creat()
    {
        // 创建单个通道

        // 1.创建管道
        int pipefd[2] = {0};
        int n = pipe(pipefd);
        if(n < 0) 
        {
            std::cerr << "pipe fail!" << std::endl;
            return false;
        }

        pid_t subid = fork();
        if(subid < 0) 
        {
            std::cerr << "fork fail!" << std::endl;
            return false;
        }
        else if(subid == 0)
        {
            // child
            close(pipefd[1]);

            // ...
            close(pipefd[1]);
            exit(1);
        }
        else 
        {
            // father
            close(pipefd[0]);

            // ...
            int status = 0;
            int ret = waitpid(subid,&status,0);


        }
    }

    ~ProcessPool() {};

private:
    ChannerManger _cn;
    int _process_num;
};

#endif

#ifndef __PROCESS_POOL_HPP__
#define __PROCESS_POOL_HPP__

#include <iostream>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <string>
#include "Task.hpp"

class Channel
{
public:
    Channel(int fd, pid_t id)
        : _wfd(fd), _subid(id)
    {
        _name = "channel-" + std::to_string(_wfd) + "-" + std::to_string(_subid);
    }

    void Send(int code)
    {
        ssize_t n = write(_wfd, &code, sizeof(code));
        if (n != sizeof(code))
            std::cerr << "写入错误" << std::endl;
    }

    bool Close()
    {
        int n = close(_wfd);
        return n == 0;
    }

    void Wait()
    {
        pid_t pid = waitpid(_subid, nullptr, 0);
        (void)pid;
    }

    int GetFd() { return _wfd; }
    pid_t GetSubid() { return _subid; }
    std::string GetName() { return _name; }

    ~Channel() {}

private:
    int _wfd;
    pid_t _subid;
    std::string _name;
};

const int gdefaultnum = 5;

class ChannelManger
{
public:
    ChannelManger() : _next(0)
    {
    }

    void InsertChannel(int wfd, pid_t subid)
    {
        _channels.emplace_back(wfd, subid);

        // Channel ch(fd,subid);
        // _channels.push_back(ch);
    }

    Channel &Select()
    {
        // 轮询
        auto &chl = _channels[_next++];
        _next %= _channels.size();

        return chl;
    }

    void PrintChannel()
    {
        for (auto &channel : _channels)
        {
            std::cout << channel.GetName() << std::endl;
        }
    }

    void CloseAll()
    {
        for (auto &channel : _channels)
        {
            channel.Close();
        }
    }

    void StopProcess()
    {
        for (auto &channel : _channels)
        {
            channel.Close();
            std::cout << "关闭: " << channel.GetName() << std::endl;
        }
    }

    void WaitProcess()
    {
        for (auto &channel : _channels)
        {
            channel.Wait();
            std::cout << "回收：" << channel.GetName() << std::endl;
        }
    }

    void StopAndWait()
    {
        for (auto &channel : _channels)
        {
            channel.Close();
            std::cout << "关闭: " << channel.GetName() << std::endl;
            channel.Wait();
            std::cout << "回收：" << channel.GetName() << std::endl;
        }
    }

    ~ChannelManger() {}

private:
    std::vector<Channel> _channels;
    size_t _next;
};

class ProcessPool
{
public:
    ProcessPool(int num) : _process_num(num)
    {
        _tm.Register(PrintLog);
        _tm.Register(DownLoad);
        _tm.Register(UpLoad);
    }

    void Work(int rfd, pid_t subid)
    {
        while (1)
        {
            int code = 0;
            ssize_t n = read(rfd, &code, sizeof(code));
            if (n > 0)
            {
                if (n != sizeof(code))
                    continue;

                std::cout << "子进程[" << getpid() << "]收到一个任务码：" << code << std::endl;
                _tm.Execute(code);
            }
            else if (n == 0)
            {
                std::cout << "子进程退出" << std::endl;
                break;
            }
            else
            {
                std::cout << "读取错误" << std::endl;
                break;
            }
        }
    }

    void Debug()
    {
        _cn.PrintChannel();
    }

    bool BuildProcess()
    {
        for (int i = 0; i < _process_num; i++)
        {
            int pipefd[2] = {0};
            int n = pipe(pipefd);
            if (n < 0)
            {
                std::cerr << "pipe fail!" << std::endl;
                return false;
            }
            pid_t subid = fork();
            if (subid < 0)
            {
                std::cerr << "fork fail!" << std::endl;
                return false;
            }
            else if (subid == 0)
            {
                // child
                close(pipefd[1]);

                // 创建子进程时关闭其他兄弟的写端
                // 由于写时拷贝，不会关闭父进程对应当前子进程的写端，子进程关掉的是自己的写端
                _cn.CloseAll();


                Work(pipefd[0], subid);
                close(pipefd[0]);
                exit(1);
            }
            else
            {
                // father

                // ChannelManger在父进程的内存中存着，由父进程进行管理
                // 如果子进程进行insert，就会导致写时拷贝，不会影响父进程的 ChannelManer
                _cn.InsertChannel(pipefd[1], subid);

                close(pipefd[0]);
            }
        }
        return true;
    }

    void Dispatch()
    {
        // 1.选择任务
        int taskcode = _tm.RandTaskCode();

        // 2.选择一个信道，均衡负载的选择一个子进程
        auto &chl = _cn.Select();
        std::cout << "选择了一个子进程：" << chl.GetName() << std::endl;

        // 2.派发任务
        chl.Send(taskcode);
        std::cout << "发送了一个任务码：" << taskcode << std::endl;
    }

    void ShutDown()
    {
        // 1.关闭所有子进程
        // _cn.StopProcess();

        // 2.回收所有子进程
        // _cn.WaitProcess();

        _cn.StopAndWait();
    }

    ~ProcessPool() {}

private:
    ChannelManger _cn;
    int _process_num;
    TaskManger _tm;
};

#endif
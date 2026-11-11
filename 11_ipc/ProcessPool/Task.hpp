#pragma once

#include <iostream>
#include <vector>
#include <ctime>

typedef void (*task_t)();

void PrintLog()
{
    std::cout << "当前子进程用来打印日志" << std::endl;
}

void DownLoad()
{
    std::cout << "当前子进程用来下载" << std::endl;
}

void UpLoad()
{
    std::cout << "当前进程用来上传" << std::endl;
}

class TaskManger
{
public:
    TaskManger()
    {
        srand(time(nullptr));
    }

    void Register(task_t t)
    {
        _tasks.push_back(t);
    }

    int RandTaskCode()
    {
        return rand() % _tasks.size();
    }

    void Execute(int code)
    {
        if(code >= 0 && code < _tasks.size())
        {
            _tasks[code]();
        }
    }

    ~TaskManger(){}

private:
    std::vector<task_t> _tasks;
};
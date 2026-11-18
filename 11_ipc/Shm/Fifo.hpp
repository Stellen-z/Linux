#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "Comm.hpp"

#define PATH "."
#define NAME "fifo"

class NameFifo
{
public:
    NameFifo(const std::string &path, const std::string &name)
        : _path(path), _name(name), _wfd(-1), _rfd(-1)
    {
        _fifoname = _path + "/" + _name;
    }

    void Create()
    {
        // 1.创建管道文件
        umask(0);
        int n = mkfifo(_fifoname.c_str(), 0666);
        if (n < 0)
            ERR_EXIT("mkfifo");
        std::cout << "mkfifo success" << std::endl;
    }

    void OpenForRead()
    {
        // 2.以只读方式打开管道文件
        _rfd = open(_fifoname.c_str(), O_RDONLY);
        if (_rfd == -1)
            ERR_EXIT("open");
        std::cout << "open success" << std::endl;
    }

    void OpenForWrite()
    {
        // 1.以写入方式打开管道文件
        _wfd = open(_fifoname.c_str(), O_WRONLY);
        if (_wfd == -1)
            ERR_EXIT("open");
        std::cout << "open success" << std::endl;
    }

    bool Wait()
    {
        char c = 'a';
        int n = read(_rfd, &c, 1);
        if (n > 0)
        {
            printf("醒来: %d\n", n);
            return true;
        }
        return false;
    }

    void WakeUp()
    {
        char c = '1';
        int n = write(_wfd, &c, 1);
        if (n > 0)
            printf("尝试唤醒: %d\n", n);
    }

    void CloseRead()
    {
        if (_rfd != -1)
            close(_rfd);
    }

    void CloseWrite()
    {
        if (_wfd != -1)
            close(_wfd);
    }

    ~NameFifo()
    {
        // 引用计数判断是否真的unlink
        int n = unlink(_fifoname.c_str());
        if(n == 0)
            std::cout << "remove success!" << std::endl;
        else 
            std::cout << "remove failed!" << std::endl;
    }

private:
    std::string _path;
    std::string _name;
    std::string _fifoname;
    int _wfd, _rfd;
};
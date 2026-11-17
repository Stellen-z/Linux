// comm.hpp
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PATH "./"
#define NAME "fifo"

class NameFifo
{
public:
    NameFifo(const std::string &path, const std::string &name)
        : _path(path), _name(name),_wfd(-1),_rfd(-1)
    {
        _fifoname = _path + "/" + _name;
    }

    bool Create()
    {
        // 1.创建管道文件
        umask(0);
        int n = mkfifo(_fifoname.c_str(), 0666);
        if (n < 0)
        {
            std::cerr << "mkfifo error" << std::endl;
            return false;
        }
        std::cout << "mkfifo success" << std::endl;
        return true;
    }

    bool OpenForRead()
    {
        // 2.以只读方式管道文件
        _rfd = open(_fifoname.c_str(), O_RDONLY);
        if (_rfd == -1)
        {
            std::cerr << "open error" << std::endl;
            return false;
        }
        std::cout << "open success" << std::endl;
        return true;
    }

    bool OpenForWrite()
    {
        // 1.以写入方式打开管道文件
        _wfd = open(_fifoname.c_str(), O_WRONLY);
        if (_wfd == -1)
        {
            std::cerr << "open error" << std::endl;
            return false;
        }
        std::cout << "open success" << std::endl;
        return true;
    }

    void Read()
    {
        // 3.获取管道文件内容
        while (1)
        {
            char buffer[1024];
            int number = read(_rfd, buffer, sizeof(buffer) - 1);
            if (number > 0)
            {
                buffer[number] = 0;
                std::cout << "Child Say:" << buffer << std::endl;
            }
            else if (number == 0)
            {
                // 管道的w端退出
                std::cout << "Client Quit!,I will quit" << std::endl;
                break;
            }
            else
            {
                std::cerr << "read error" << std::endl;
                break;
            }
        }
    }

    void Write()
    {
        // 2.写入信息
        std::string msg;
        pid_t id = getpid();
        int cnt = 1;
        while (1)
        {
            std::cout << "Please Enter# ";
            if(!std::getline(std::cin, msg)) break;
            msg += (", message number: " + std::to_string(cnt++) + ", [" + std::to_string(id) + "]");
            write(_wfd, msg.c_str(), msg.size());
        }
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
        // 1.关闭文件
        CloseWrite();
        CloseRead();

        // 2.删除管道文件
        unlink(_fifoname.c_str());
    }

private:
    std::string _path;
    std::string _name;
    std::string _fifoname;
    int _wfd, _rfd;
};
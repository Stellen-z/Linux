#include "comm.hpp"

int main()
{
    // 创建管道文件
    NameFifo fifo(PATH,NAME);

    if(!fifo.Create()) return -1;
    if(!fifo.OpenForRead()) return 1;
    fifo.Read();

    return 0;
}
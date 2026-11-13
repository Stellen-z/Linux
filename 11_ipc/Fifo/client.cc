#include "comm.hpp"

int main()
{
    // 指向同一个命名管道
    NameFifo fifo(PATH,NAME);
    if(!fifo.OpenForWrite())  return 2;
    fifo.Write();

    return 0;
}
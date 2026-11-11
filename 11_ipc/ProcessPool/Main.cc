#include "ProcessPool.hpp"

int main()
{
    ProcessPool pp(gdefaultnum);
    pp.BuildProcess();

    // pp.Debug();

    int cnt = 10;
    while(cnt--)
    {
        pp.Dispatch();
        sleep(1);
    }

    pp.ShutDown();

    return 0;
}
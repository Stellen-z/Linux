#include "Shm.hpp"

int main()
{
    Shm shm(path, projectid, CREATER);

    char* mem = (char*)shm.GetShmAddr();
    while(1)
    {
        printf("%s\n",mem);
        sleep(1);
    }

    return 0;
}
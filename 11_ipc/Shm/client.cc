#include "Shm.hpp"

int main()
{
    Shm shm(path, projectid, USER);

    char* mem = (char*)shm.GetShmAddr();
    for(char ch = 'A';ch <= 'Z';ch++)
    {
        mem[ch - 'A'] = ch;
        sleep(1);
    }

    return 0;
}
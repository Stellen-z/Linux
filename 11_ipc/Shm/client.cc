#include "Shm.hpp"
#include "Fifo.hpp"

int main()
{
    NameFifo writer(PATH, NAME);
    Shm shm(path, projectid, USER);

    writer.OpenForWrite();

    char* mem = (char*)shm.GetShmAddr();

    int index = 0;
    for(char ch = 'A';ch <= 'Z';ch++,index += 2)
    {
        sleep(1);
        mem[index] = ch;
        mem[index+1] = ch;
        mem[index+2] = 0;
        
        writer.WakeUp(); 
    }

    writer.CloseWrite();

    return 0;
}
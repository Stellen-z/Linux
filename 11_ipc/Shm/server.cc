#include "Shm.hpp"
#include "Fifo.hpp"

int main()
{
    NameFifo reader(PATH, NAME);
    reader.Create();

    Shm shm(path, projectid, CREATER);

    reader.OpenForRead();

    char *mem = (char *)shm.GetShmAddr();
    while (1)
    {
        if (reader.Wait())
        {
            printf("%s\n", mem);
        }
        else
            break;
    }

    reader.CloseRead();

    return 0;
}
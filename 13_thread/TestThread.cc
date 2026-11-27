#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>

int flag = 10;

std::string Format(const pthread_t& tid)
{
    char id[64];
    snprintf(id, sizeof(id), "0x%lx", tid);

    return id;
}

void Showtid(const pthread_t& tid)
{
    printf("新线程的tid:0x%lx\n",tid);
}

void* routine(void* arg)
{
    std::string name = static_cast<char *>(arg);

    int cnt = 5;
    while (cnt)
    {
        sleep(1);
        std::cout << "我是一个新线程" << ", 线程id: " << Format(pthread_self()) << std::endl;
        --cnt;
        ++flag;
    }

    return (void*)nullptr;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, routine, (void*)"hello");

    Showtid(tid);

    int cnt = 5;
    while (cnt)
    {
        std::cout << "我是一个main线程" << ", 线程id: " << Format(pthread_self()) << " flag: "<< flag << std::endl;
        sleep(1);
        --cnt;
    }

    pthread_join(tid, nullptr);

    return 0;
}

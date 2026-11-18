#include <iostream>
#include <string>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include "Comm.hpp"

#define CREATER "creater"
#define USER "user"


const int gdefaultid = -1;
const std::string path = ".";
const int projectid = 0x66;
const int gsize = 4096;
const int perms = 0666;

class Shm
{
private:
    void CreateHelper(int flags)
    {
        // 申请共享内存
        _shmid = shmget(_key, _size, flags);
        if (_shmid == -1)
            ERR_EXIT("shmget");
        std::cout << "shmid success, shmid:" << _shmid << std::endl;
    }

    void Create()
    {
        CreateHelper(IPC_CREAT | IPC_EXCL | 0666);
    }

    void GetShm()
    {
        CreateHelper(IPC_CREAT);
    }

    void Attach()
    {
        // 检查_shmid
        if (_shmid == gdefaultid)
            ERR_EXIT("_shmid");

        _strat_addr = shmat(_shmid, nullptr, 0);
        // 64位指针8字节，用long long 强转
        if (_strat_addr == (void *)-1)
        {
            ERR_EXIT("shmat");
        }
        std::cout << "Attach success" << std::endl;
    }

    void Destroy()
    {
        if (_shmid == gdefaultid)
            return;
        int ctl = shmctl(_shmid, IPC_RMID, 0);
        if (ctl == -1)
        {
            ERR_EXIT("shmctl");
            return;
        }
        std::cout << "shm deleted success!" << std::endl;
    }

public:
    Shm(const std::string &pathname, int projectid, const std::string &usertype)
        : _shmid(gdefaultid),
          _size(gsize),
          _strat_addr(nullptr),
          _usertype(usertype)
    {
        // 创建key
        _key = ftok(path.c_str(), projectid);
        if (_key == -1)
            ERR_EXIT("ftok");
        std::cout << "ftok success, key:" << _key << std::endl;

        if (_usertype == CREATER)
            Create();
        else if (_usertype == USER)
            GetShm();
        else
        {
        }

        Attach();
    }

    void *GetShmAddr()
    {
        printf("Virtual Address: %p\n", _strat_addr);
        return _strat_addr;
    }

    int GetSize() { return _size; }

    void ShmInfo()
    {
        struct shmid_ds ds;
        shmctl(_shmid,IPC_STAT,&ds);
        printf("shm_segsz: %ld\n",ds.shm_segsz);
        printf("key: 0x%d\n",ds.shm_perm.__key);
        printf("shm_nattch: %ld\n",ds.shm_nattch);
    }

    void Detach()
    {
        if(_strat_addr == nullptr) return;
        int n = shmdt(_strat_addr);
        if (n == 0)
            printf("shmdt success!\n");
        else
            printf("shmdt error!\n");
    }

    ~Shm()
    {
        Detach();
        if (_usertype == CREATER)
            Destroy();
    }

private:
    key_t _key;
    int _shmid;
    int _size;
    void *_strat_addr;
    std::string _usertype;
};
#include "mystdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h> 
#include <unistd.h>

int fd = -1;
int flag = 0;

MyFile* BuyFile()
{
    MyFile* myfile = (MyFile*)malloc(sizeof(MyFile));
    if(myfile == NULL)
    {
        perror("malloc:");
        return NULL;
    }

    myfile->fileno = fd;
    myfile->flag = flag;
    myfile->bufferlen = 0;
    myfile->fflush_method = LINE_FFLUSH;
    memset(myfile->outbuffer,0,sizeof(myfile->outbuffer));

    return myfile;
}


MyFile* MyFopen(const char* path,const char* mode)
{
    if(strcmp(mode,"r")== 0)
    {
        flag = O_RDONLY;
        fd = open(path,flag,0666);
    }
    else if(strcmp(mode,"w") == 0)
    {
        flag = O_CREAT | O_WRONLY | O_TRUNC;
        fd = open(path,flag,0666);
    }
    else if(strcmp(mode,"a") == 0)
    {
        flag = O_CREAT | O_WRONLY | O_APPEND;
        fd = open(path,flag,0666);
    }
    else {}
    
    if(fd < 0) return NULL;
    return BuyFile();

}
void MyFclose(MyFile* file)
{
    if(file->fileno < 0) return;

    MyFFlush(file);
    close(file->fileno);
    free(file);

}

void MyFwrite(MyFile* file,void* str,size_t len)
{
    // 1.写入到缓冲区
    memcpy(file->outbuffer+file->bufferlen,str,len);
    file->bufferlen += len;

    // 2.判断是否满足刷新条件
   if(file->fflush_method == LINE_FFLUSH && file->outbuffer[file->bufferlen-1] == '\n')
   {
       MyFFlush(file);
   }
}

void MyFFlush(MyFile* file)
{
    if(file->bufferlen <= 0) return;

    write(file->fileno,file->outbuffer,file->bufferlen);
    fsync(file->fileno);
    file->bufferlen = 0;
}

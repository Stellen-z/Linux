#include <stdio.h>

#define MAX 1024
#define NONE_FFLUSH (1<<0)
#define LINE_FFLUSH (0<<1)
#define FULL_FFLUSH (1<<2)


typedef struct IO_FILE
{
    int fileno;
    int flag;
    int bufferlen;
    char outbuffer[MAX];
    int fflush_method;
}MyFile; 


MyFile* MyFopen(const char* path,const char* mode);
void MyFclose(MyFile*);
void MyFwrite(MyFile*,void* str,size_t len);
void MyFFlush(MyFile*);

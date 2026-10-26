#include <stdio.h>
#include <string.h>



//mycat log.txt
int main(int argc,char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s filename\n",argv[0]);
        return 1;
    }
    
    FILE* fp = fopen(argv[1],"r");
    if(fp == NULL)
    {
        perror("fopen");
        return 2;
    }
   
    char buff[128];
    memset(buff,0,sizeof(buff));
    while(1)
    {
        size_t n = fread(buff,1,sizeof(buff)-1,fp);
        if(n > 0)
        {
            fwrite(buff,1,n,stdout);
        }
        if(feof(fp)) break;
    }
    
    fclose(fp);
    return 0;
}


// int main()
// {
//     FILE* fp = fopen("log.txt","w");
//     if(fp == NULL)
//     {
//         perror("fopen failed!\n");
//         return 1;
//     }
// 
//     const char* msg = "hello world\n";
//     int cnt = 1;
//     while(cnt <= 10)
//     {
//         fwrite(msg,strlen(msg),1,fp);
//         cnt++;
//     }
//     return 0;
// }

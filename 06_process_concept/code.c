#include <stdio.h>

int main(int argc,char* argv[],char* env[])
{
   // (void)env;

   // for(int i = 0;i < argc;i++)
   // {
   //     printf("argv[%d]->%s\n",i,argv[i]);
   // }


    (void)argc;
    (void)argv;

    for(int i = 0;env[i];i++)
    {
        printf("env[%d]->%s\n",i,env[i]);
    }

    return 0;
}

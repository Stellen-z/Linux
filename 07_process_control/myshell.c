#include <stdio.h>
#include <stdlib.h>


#define MAX_SIZE 1024

const char* GetUsrName()
{
    const char* name = getenv("USER");
    return name == NULL ? "None" : name;
}

const char* GetHostName()
{
    const char* hostname = getenv("HOSTNAME");
    return hostname == NULL ? "None" : hostname;
}

const char* GetPwd()
{
    const char* pwd = getenv("PWD");
    return pwd == NULL ? "None" : pwd;
}


void PirntCommandPrompt()
{
    
}

int main()
{
    
    return 0;
}

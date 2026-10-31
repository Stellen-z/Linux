#include <iostream>
#include <cstdio>


const char* GetUserName()
{
    const char* username = getenv("USER");
    return username == NULL ? "None" : username;
}

const char* GetPwd()
{
    const char* pwd = getenv("PWD");
    return pwd == NULL ? "None" : pwd;
}

const char* GetHostName()
{
    const char* hostname = getenv("HOSTNAME");
    return hostname == NULL ? "None" : hostname;
}


void MakeCommandPrompt(char* commandline,size_t size)
{
    
}


void PrintCommandPrompt()
{

}


int main()
{
    // 1.输出提示符
    PrintCommandPrompt();

    return 0;
}

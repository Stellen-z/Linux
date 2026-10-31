#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <sys/stat.h>
#include <fcntl.h>


#define COMMAND_SIZE 1024
#define FORMAT "[%s@%s %s]# "
#define MAX_ARGC 64

// 1.命令行参数表
char* g_argv[MAX_ARGC];
int g_argc = 0;

// 2.环境变量表
#define MAX_ENV 100
char* g_env[MAX_ENV];
int g_envs = 0;


// 重定向
#define NO_PUT 0
#define IN_PUT 1
#define OUT_PUT 2
#define APPEND_PUT 3

int redir;
std::string filename;

void InitEnv()
{
    extern char** environ;
    memset(g_env,0,sizeof(g_env));
    g_envs = 0;

    for(int i = 0;environ[i];i++)
    {
        g_env[i] = strdup(environ[i]);
        g_envs++;
    }

    g_env[g_envs++] = strdup("HAHA=for_test");
    g_env[g_envs] = NULL;
    
    for(int i = 0;g_env[i];i++)
    {
       putenv(g_env[i]);
    }
    // environ = g_env;
}

// last_exit_code
int last_exit_code = 0;

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

const char* GetHome()
{
    const char* home = getenv("HOME");
    return home == NULL ? "" : home;
}

const char* GetOldPwd()
{
    const char* oldpwd = getenv("OLDPWD");
    return oldpwd == NULL ? "" : oldpwd;
}

std::string DirName(const char* pwd)
{
#define SLASH "/" 
    std::string dir = pwd;
    if(dir == SLASH) return SLASH;
    size_t pos  = dir.rfind(SLASH);
    if(pos == std::string::npos) return "BUG!";

    return dir.substr(pos + 1);
}


void MakeCommandPrompt(char prompt[],size_t size)
{
    snprintf(prompt,size,FORMAT,GetUsrName(),GetHostName(),DirName(GetPwd()).c_str());
    // snprintf(prompt,size,FORMAT,GetUsrName(),GetHostName(),GetPwd());
}

void PrintCommandPrompt()
{
    char prompt[COMMAND_SIZE];
    MakeCommandPrompt(prompt,sizeof(prompt));
    printf("%s",prompt);
    fflush(stdout);   
}

bool GetCommandLine(char* out,size_t size)
{
    char* res = fgets(out,size,stdin);
    if(res == NULL) return false;
   
    // "ls" -> "ls\n\0" [strlen(commandline) - 1] = \n 清理\n
    out[strlen(out) - 1] = 0;
    if(strlen(out) == 0) return false;
    return true;
}

void TrimSpace(char* cmd,int& end)
{
    while(isspace(cmd[end]))
    {
        ++end;
    }
}


void RedirCheck(char* cmd)
{
    // "ls -a -l < log.txt"
    redir = NO_PUT;
    filename.clear();

    int start = 0,end = strlen(cmd) - 1;
    while(end > start)
    {
        if(cmd[end] == '<')
        {
            redir = IN_PUT;
            cmd[end++] = 0;
            TrimSpace(cmd,end);
            filename = cmd + end;
            break;
        }
        else if(cmd[end] == '>')
        {
            if(cmd[end-1] == '>')
            {
                // <<
                redir = APPEND_PUT;
                cmd[end-1] = 0;
            }
            else 
            {
                // <
                redir = OUT_PUT;
            }
            cmd[end++] = 0;
            TrimSpace(cmd,end);
            filename = cmd + end;
            break;
        }
        else --end;
    }
}


bool ParseCommand(char cmd[])
{
#define DELIM " "

    g_argc = 0;
   //  char* token = strtok(cmd,DELIM);
    g_argv[g_argc++] = strtok(cmd,DELIM);
    while((g_argv[g_argc++] = strtok(NULL,DELIM)));
    g_argc--;
    return g_argc > 0 ? true : false;
   //  while(i < MAX_ARGC && token != NULL)
   //  {   
   //      g_argv[i++] = strtok(NULL," ");
   //  }
}

void PrintArgv()
{
    int i = 0;
    for(;g_argv[i];i++)
    {
        printf("g_argv[%d] -> %s\n",i,g_argv[i]);
    }
    printf("argc:%d\n",g_argc);
}

bool CD()
{   
    if(g_argc == 1)
    {
        // cd 
        std::string home = GetHome();
        if(home.empty()) return true;
        chdir(home.c_str());
    }
    else 
    {
        std::string where = g_argv[1];
        if(where == "~")
        {
            std::string home = GetHome();
            chdir(home.c_str());
        }
        else if(where == "-")
        {
            std::string oldpwd = GetOldPwd();
            chdir(oldpwd.c_str());
        }
        else chdir(where.c_str());
    }

    return true;
}

void ECHO()
{
    if(g_argc == 2)
    {
      std::string opt = g_argv[1];
      
      if(opt == "$?")
      {
          std::cout << last_exit_code << std::endl;
          last_exit_code = 0;
      }
      else if(opt[0] == '$') 
      {
          // $PATH
          std::string env_name = opt.substr(1);
          const char* env_val = getenv(env_name.c_str());
          if(env_val) std::cout << env_val << std::endl;
      }
      else std::cout << opt << std::endl;
    }
}

void ALISA()
{

}

bool CheckAndExecBuiltin()
{
    std::string name = g_argv[0];
    if(name == "cd")
    {
        CD();
        return true;
    }
    else if(name == "echo")
    {
        ECHO();
        return true;
    }
    else if(name == "alisa")
    {
        ALISA();
        return true;
    }

    return false;
}

void Excute()
{
     pid_t id = fork();
     if(id == 0)
     {
        // child
        int fd = -1;
        if(redir == IN_PUT)
        {
            fd = open(filename.c_str(),O_RDONLY);
            if(fd < 0) exit(1);
            dup2(fd,0);
            close(fd);
        }
        else if(redir == OUT_PUT)
        {
            fd = open(filename.c_str(),O_CREAT | O_WRONLY | O_TRUNC,0666);
            if(fd < 0) exit(2);
            dup2(fd,1);
            close(fd);

        }
        else if(redir == APPEND_PUT)
        {
            fd = open(filename.c_str(),O_CREAT | O_WRONLY | O_APPEND,0666);
            if(fd < 0) exit(3);
            dup2(fd,1);
            close(fd);
        }
        else {}

        // 程序替换,替换的是代码和数据,而虚拟地址空间，页表...是相同的
        execvp(g_argv[0],g_argv);
        perror("execvp");
        exit(1);
     }
     int status = 0; 

     // father
     pid_t ret = waitpid(id,NULL,0);
     (void)ret;
     if(ret > 0) last_exit_code = WEXITSTATUS(status);
}

void Destroy()
{

}

int main()
{
    InitEnv();
    while(true)
    {   
        // 1.输出命令提示符
        PrintCommandPrompt();
    
        // 2.获取用户指令
        char commandline[COMMAND_SIZE];
        if(!GetCommandLine(commandline,sizeof(commandline))) continue;
        // printf("echo %s\n",commandline);
        
        // 3. 解析重定向指令
        RedirCheck(commandline);        
        // printf("redir: %d, filename: %s\n", redir, filename.c_str());
            
        // 4.解析指令
        if(!ParseCommand(commandline)) continue;
         
        // PrintArgv();
         
        // 5.检测并处理内建命令
        if(CheckAndExecBuiltin()) continue;

        // 6.执行指令
        Excute();

        Destroy();
    }
    return 0;
}

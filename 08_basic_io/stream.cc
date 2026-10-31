#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdio.h>
#include <unistd.h>


int main()
{
  // std::cout << "hello world!" << std::endl;
  // fprintf(stdout,"hello cout!\n");

  // std::cerr << "hello world!" << std::endl;
  // fprintf(stderr,"hello cerr!\n");
    
    printf("hello printf\n");
    fprintf(stdout,"hello fprintf!\n");
    const char* msg = "hello fwrite!\n";
    fwrite(msg,strlen(msg),1,stdout);

    const char* s = "hello write!\n";
    write(1,s,strlen(s));

    fork();
    return 0;
}

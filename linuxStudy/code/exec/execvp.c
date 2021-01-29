#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char* argv[])
{
    if(argc<2)
    {
        printf("Usage1:./a.out cmd [-param1|param2|...] \n");
        printf("Usage2:./a.out cmd [-param1] [-param2] ...\n");
        exit(1);
    }

    argc+=1;
    argv[argc]='\0';

    int p;
    p=fork();
    if(p==0)
    {
        printf("创建子进程成功！！！\n");
        for(int i=0;i<argc;i++)
        printf("%s ",argv[i]);
        printf(" wc:%d\n",argc);
        char *_argv[argc-1];
        for(int i=1;i<argc;i++)
        {
            _argv[i-1]=argv[i];
        }
        int _argc=argc-1;
        for(int i=0;i<_argc;i++)
        printf("%s ",_argv[i]);

        
        
       int ret = execvp(_argv[0],_argv);
       if(ret==-1)
       {
           perror("execvp");
           exit(1);
       }
       

    }else if(p>0)
    {
        printf("我是父进程！！！\n");
        wait(NULL);

    }else
    {
        perror("创建子进程");
        exit(1);
    }
    exit(1);

}


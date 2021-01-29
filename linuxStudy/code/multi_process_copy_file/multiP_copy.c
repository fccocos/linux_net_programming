#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFSIZE 1024*64 //64KB
int main()
{
    const int p_count=5;//进程数
    pid_t p;
    pid_t tmp;//用于暂存子进程id
    pid_t child_p[p_count];
    long  sf_size;//源文件大小
    struct stat sb;//用于存放文件信息
    int i,j=5;
    int fd[5];
    int timer=0;

    //创建子进程
    for(i=0;i<p_count;i++)
    {
        p=fork();
        if(p == 0)
        {

            break;
        }
    }
    if(i<5)
    {
       char buf[BUFSIZE]={0};
       char s_buf[512];
       sprintf(s_buf,"file%d.txt",i);
       int s_fd=open("./index.html",O_RDONLY);
       fd[i]=open(s_buf,O_RDWR|O_CREAT,664);
       if(s_fd==-1)
       {
           perror("open error:");
           exit(1);
       }
       long fr_size=read(s_fd,buf,sizeof(buf));
       long s_count=fr_size;

        while(fr_size)
        {
            timer++;
            sleep(1);
            printf("time:%ds",timer++);
            printf("I am %dth child,I am reading and writing...\n",i+1);
            printf("pid=%u,ppid=%u,  ",getpid(),getppid());
            printf("I am writing %ld bytes,\n\n",s_count);
            write(fd[i],buf,fr_size);
            fr_size=read(s_fd,buf,sizeof(buf));
            s_count+=fr_size;
        }
        
    }else{
        sleep(1);
        printf("我是父进程\n");
        /*
        int timer=0;
        while(j)
        {
            timer++;
            sleep(1);
            printf("我有五个子进程，正在读写文件\n");
            printf("我是父进程，我的进程id=%d,正在监视我的子进程的读写。。。\n",getpid());
            if(timer<5)
            {
                printf("没有子进程来读写文件，等待了%d.......s\n",timer+1);
            }else{
                printf("读写超时，结束循环.....\n");
                break;
                
            }

            
            for(int i=0;i<p_count;i++)
            {
                if(tmp==(getpid()+i+1))
                {
                    printf("我的进程号为%u的子进程读写完毕，它将自动结束\n",tmp);
                    j--;
                }
            }
            
            
        }
        */
        printf("我是父进程，我将要结束运行。。。\n");
        sleep(2);
        printf("I am die！！！\n");
        

    }


return 0;
}


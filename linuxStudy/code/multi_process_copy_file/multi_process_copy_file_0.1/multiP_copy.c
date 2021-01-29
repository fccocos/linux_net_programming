#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc,char *argv[])
{
    int n=5,i;
    //至少有三个参数，第四个参数可以是进程个数
    //
    if(argc<3)
    {
        printf("./a.out src dst [n]\n");
        exit(1);
    }
    if(argc==4)
    {
        n=atoi(argv[3]);
    }
    
    //打开源文件
    //
    int src_fd=open(argv[1],O_RDWR);
    if(src_fd==-1)
    {
        perror("open src file error:");
        exit(1);
            
    }

    //打开目标文件
    //
    int dst_fd=open(argv[2],O_RDWR|O_CREAT|O_TRUNC,0664);
    if(dst_fd==-1)
    {
        perror("open dst file error:");
        exit(1);
    }

    //拓展目标文件
    //
    struct stat sb;
    stat(argv[1],&sb);
    int len=sb.st_size;

    truncate(argv[2],len);

    //将源文件映射到缓冲区
    //
    char *src_p=mmap(NULL,len,PROT_READ,MAP_SHARED,src_fd,0);
    if(src_p==MAP_FAILED)
    {
        perror("mmap src error");
        exit(1);
    }

    //将目标文件映射到缓冲区
    //
    char *dst_p=mmap(NULL,len,PROT_READ|PROT_WRITE,MAP_SHARED,dst_fd,0);
    if(dst_p==MAP_FAILED)
    {
        perror("mmap dst error");
        exit(1);
    }

    //循环创a建子进程
    //
    for(i=0;i<n;i++)
    {
        if(fork()==0)
        break;
    }

    //计算子进程需要拷贝的起点和大小
    //
    int cp_size=len/n;
    int mod=len%n;

    //数据拷贝用memcpy
    if(i<n)
    {
        sleep(i);
        if(i==n-1)
        {
            memcpy(dst_p+i*cp_size,src_p+i*cp_size,cp_size+mod);
            printf("子进程%d,pid=%u正在读写映射区。。。。\n",i+1,getpid());
        }
        else
        {
            memcpy(dst_p+i*cp_size,src_p+i*cp_size,cp_size);
            printf("子进程%d,pid=%u正在读写映射区。。。。\n",i+1,getpid());
        
        }
    }
    else
    {
        sleep(i+1);
        printf("如果父进程结束，子进程还在继续，注销子进程\n");
        for(i=0;i<n;i++)
        {
            wait(NULL);
        }
    }

    //释放映射区
    //
    if(munmap(src_p,len)<0)
    {
        perror("munmap src error");
        exit(1);
    }
    if(munmap(dst_p,len)<0)
    {
        perror("munmap dst error");
        exit(1);
    }

    //关闭描述符
    //
    close(src_fd);
    close(dst_fd);
return 0;
}

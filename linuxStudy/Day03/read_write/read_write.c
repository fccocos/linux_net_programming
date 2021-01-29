#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFSIZE 2048

int main()
{

	int fd=open("./index.html",O_RDONLY);
	int fd1=open("./file.html",O_CREAT|O_WRONLY,655);
	char buff[BUFFSIZE]={0};
	if(-1==fd)
	{
		perror("open index.html failure:");
		exit(1);
	}
	else printf("open index.html successfully\n");
	if(-1==fd1)
	{
		perror("open file.txt failure:");
		
	}
	//读文件
	int r_count=read(fd,buff,sizeof(buff));
	int w_count;
	if(r_count==-1)
	{
		perror("read:");
		exit(1);
	}
	printf("r_count value:%d\n",r_count);
	//循环读文件
	while(r_count)
	{
		
		//写文件
		w_count=write(fd1,buff,r_count);
		printf("当前读写文件%dbits\n",w_count);
		r_count=read(fd,buff,sizeof(buff));
		if(r_count==-1)
		{
			perror("write:");
			exit(1);
		}
		

	}
	if(r_count==0)
	{
		printf("文件读写完成");
	}
	close(fd);
	close(fd1);
}

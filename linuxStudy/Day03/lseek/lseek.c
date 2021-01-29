#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	int fd=open("aa",O_RDWR);
	if(fd==-1)
	{
		perror("open file");
		exit(1);
	}
	int ret = lseek(fd,0,SEEK_END);
	printf("file length=%d\n",ret);

	//文件扩展
	ret=lseek(fd,2000,SEEK_END);
	printf("file length=%d\n",ret);

	//做一次写操作
	write(fd,"aaad",4);
	close(fd);
}

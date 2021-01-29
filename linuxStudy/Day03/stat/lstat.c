#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sysmacros.h>

int main(int argc[],char *argv[])
{
	struct stat sb;
	if(argc!=2)
	{
		fprintf(stderr,"Usage:%s <pathname>\n",argv[0]);
		exit(EXIT_FAILURE);
	}
	if(lstat(argv[1],&sb)==-1)
	{
		perror("lstat");
		exit(EXIT_FAILURE);
	}
	//设备号，主设备号m，次设备号
	printf("文件设备ID: [%lx,%lx]\n",(long)major(sb.st_dev),(long)minor(sb.st_dev));
	//打印文件类型block,character,directory,pipe,link,regular file,socket
	printf("文件类型:");
	switch(sb.st_mode&S_IFMT)
	{
		case S_IFBLK:printf("块设备\n");break;
		case S_IFCHR:printf("字符设备\n");break;
		case S_IFDIR:printf("目录\n");break;
		case S_IFIFO:printf("管道\n");break;
		case S_IFLNK:printf("系统链接\n");break;
		case S_IFREG:printf("日常文件\n");break;
		case S_IFSOCK:printf("套接字\n");break;
		default:printf("未知文件\n");break;
	}

	//打印文件索引数量st_ino
	printf("文件索引号:                      %ld\n",(long)sb.st_ino);
	//打印文件权限st_mode
	printf("文件权限:                               %lo (octal)\n",(unsigned long)sb.st_mode);
	//打印文件的连接数st_nlink
	printf("硬链接数:                         %ld\n",(long)sb.st_nlink);
	//打印文件所属
	printf("所属关系:                          用户ID=%ld 组ID=%ld\n",(long)sb.st_uid,(long)sb.st_gid);
	//打印I/O块的大小st_blksize
	printf("首选的I/O块大小:           %ld bytes\n",(long)sb.st_blksize);
	//打印文件大小st_size
	printf("文件大小:                          %lld bytes\n",(long long)sb.st_size);
	//打印分配本地块的大小
	printf("分配的块数:                   %lld\n",(long long)sb.st_blocks);
	//打印文件修改的时间
	printf("文件状态改变的最后时间:                 %s",ctime(&sb.st_ctime));
	//打印进入文件的最后时间
	printf("进入文件的最后时间:                   %s",ctime(&sb.st_atime));
	//打印文件修改的最后时间
	printf("文件修改的最后时间:             %s",ctime(&sb.st_mtime));

	exit(EXIT_SUCCESS);
}

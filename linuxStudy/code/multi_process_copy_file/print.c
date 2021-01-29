#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
int main()
{
    int fd=open("./index.html",O_RDONLY);
    char buf[2048];
    int fr=read(fd,buf,sizeof(buf));
    while(fr)
    {
        write(0,buf,fr);
        fr=read(fd,buf,sizeof(buf));
    }
    return 0;
}

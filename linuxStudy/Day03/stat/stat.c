#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[])
{
    struct stat sb;
    if(argc<2)
    {
        printf("./a.out filename\n");
        exit(1);
    }
    int ret=stat(argv[1],&st);
    if(ret==-1)
    {
        perror("stat");
        exit(1);
    }

}

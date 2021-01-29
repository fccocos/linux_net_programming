#include<stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
int getFileNum(char* root)
{
    //open dir
    DIR* dir = NULL;
    dir = opendir(root);
    if(dir==NULL)
    {
        perror("opendir:");
        exit(1);
    }
    //遍历当前打开的目录
    struct dirent* ptr=NULL;
    char path[1024]={0};
    int total=0;
    while((ptr=readdir(dir))!=NULL)
    {
        //过滤掉.和..
        if(strcmp(ptr->d_name,".")==0||strcmp(ptr->d_name,"..")==0)
        {
            continue;
        }
        //如果是目录
        if(ptr->d_type==DT_DIR)
        {
            //递归 读目录
            sprintf(path,"%s/%s",root,ptr->d_name);
            total+=getFileNum(path);
        }
        //g如果是文件
        if(ptr->d_type==DT_REG)
        {
            total++;
        }
    }
    closedir(dir);
    return total;
}



int main(int argc,char* argv[])
{

    if(argc<2)
    {
        printf("Usage: ./command dirname\n");
        exit(1);       
    }
    int total=getFileNum(argv[1]);
    printf("current directory include: %d\n",total);
    return 0;


}

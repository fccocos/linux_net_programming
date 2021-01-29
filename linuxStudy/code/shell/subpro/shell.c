#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <pwd.h>
 

/*
 *
 * struct passwd{
 * char* name;//用户名
 * char* passwd;//用户登录密码
 * uid_t pw_uid;//用户ID
 * gid_t pw_gid;//用户组ID
 * char* gecos;//用户信息
 * char* pw_dir;//家目录
 * cahr* shell_program;//shell程序
 *
 *
 *
 * }
 *
 *
 * */
//获取登陆用户名
void GetLoginName(){
    struct passwd* pass;
    pass = getpwuid(getuid());//获取当前用户的passwd结构体
    printf("[%s@",pass->pw_name);
}
 
//获取主机名
void GetHostName(){
    char name[128];
    gethostname(name,sizeof(name)-1);
    printf("%s:",name);;
}
 
//获取当前文件路径
void GetDir(){
    char pwd[128];
    char *p;
    p=getcwd(pwd,sizeof(pwd)-1);
    printf("%s]# ",p);
}

int main()
{
    while(1)
    {
        GetLoginName();
        GetHostName();
        GetDir();
        char cmd[1024];
        fflush(stdout); //清空输出缓冲区
        //读取字符
        read(0,cmd,sizeof(cmd)-1);
        pid_t p=fork();    
        if(p==0)
        {
          //命令解析
          int i=0;
          char*argv[1024]={0};
          char *token_ptr=strtok(cmd," ");
          while(token_ptr)
          {
              argv[i]=token_ptr;
              printf("%s\n",argv[i]);
              token_ptr = strtok(NULL," ");
          }      
              execvp(argv[0],argv);
              exit(1);
        }else if(p>0)
        {
              wait(NULL);
        }else
        {
              perror("create process");
              exit(1);
        }

    }
}

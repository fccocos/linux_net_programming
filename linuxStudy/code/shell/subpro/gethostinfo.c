#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

void GetDir(){
    char pwd[128];
    getcwd(pwd,sizeof(pwd)-1);
    int len = strlen(pwd);
    char *p = pwd+len-1;
    while(*p != '/' && len--){
        p--;
    }
    p++;
    printf("%s] #",p);
}
int main()
{
    struct passwd *pass;
    pass=getpwuid(getuid());
    printf("用户名：%s\n",pass->pw_name);
    printf("用户密码：%s\n",pass->pw_passwd);
    printf("用户ID：%u\n",pass->pw_uid);
    printf("用户组ID：%u\n",pass->pw_gid);
    printf("用户信息：%s\n",pass->pw_gecos);
    printf("家目录：%s\n",pass->pw_dir);
    printf("shell：%s\n",pass->pw_shell);
    char buf[512];
    int ret=gethostname(buf,sizeof(buf)-1);
    if(ret==-1)
    {
        perror("hostname");
        exit(1);
    }
    printf("host_name:%s\n",buf);
    //得到当前工作目录
    char buffer[1024];
    char *p;
    p=getcwd(buffer,sizeof(buffer));
    if(p==NULL)
    {
        perror("getcwd");
        exit(1);
    }
    printf("%s\n",p);
    GetDir();


}

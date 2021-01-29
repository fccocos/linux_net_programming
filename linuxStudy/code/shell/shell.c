



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <fcntl.h>


char* strim(char* str)//清除字符串两端的空格
{
    char *sp,*end,*ep;
    sp=str;
    size_t len;

    end=ep=str+strlen(str)-1;
    while(sp<=end&&isspace(*sp)) sp++;
    while(ep>=sp&&isspace(*ep)) ep--;
    len=(ep<sp)?0:(ep-sp)+1;
    sp[len]='\0';
    return sp;

}

//获取字符串中分割字符的个数
//如果等于零，说明字符串中没有该分割符
int getDelimNum(char* line,char delimeter)
{
    int num=0;
    int len=strlen(line);
    for(int i=0;i<len;i++)
        if(line[i]==delimeter)
            num++;
    return num;
    
}

//分割字符
/*参数说明
 * char* line 为一个输入参数，需要分割的字符串。
 * char delimeter 为一个输入参数，分割符
 * int *count 为输出参数，被分割成的份数
 * int limit 为输入参数 分割符的数量
 *返回值
 * char** vector,一个二重字符指针。
 *
 * */
char** strsplit(char *line,char delimeter,int *count,int limit)
{
    char *ptr=NULL,*str=line;
    char **vector=NULL;
    int _count=0;
    while((ptr = strchr(str,delimeter)))
    {
        *ptr='\0';//将字符串中的分割符赋值为空白字符（为字符串结束标志）
        
        char** base=(char**)realloc(vector,(_count+1)*sizeof(char*));//为vector变量分配内存空间
        if(!base)
        {
            perror("realloc");
            exit(1);
        }
        vector=base;
        printf("%s\n",str);
        vector[_count]=strim(str);//将得到的字符串先清除两边的空格后在赋值给vector变量
        str = ptr+1;//将str指针指向分割符后一个指针
        _count++;//字符串个数加一
        if(--limit == 0) break;//每分割一次，limit减一，直到limit=0,退出循环
    }
    //因为程序从循环退出后str指针并没有指向字符的末尾。也就是说还有一个字符串没有处理
    if(*str!='\0')
    {
        char** base=(char**)realloc(vector,((_count)+1)*sizeof(char*));
        if(!base)
        {
            perror("realloc");
            exit(1);
        }
        vector=base;

        vector[_count] = strim(str);
        _count++;
    }
    //*count=_count;
    return vector;

}

char **strsplit2(char *cmd,char delimeter,int* count)//词法解析器
{
    char* pstr=strim(cmd);
    char* start=pstr;
    char** vector=NULL;
    int _count=0;
    int status=0;
    while(*pstr)
    {
       if(*pstr==delimeter)
       {
           status=1;
       }
       if(status==1)
       {
           *pstr='\0';
           if(start!=NULL)
           {
               
               char** new_base = (char**)realloc(vector,(_count+1)*sizeof(char*));
               if(!new_base)
               {
                   perror("realloc");
               }
               vector=new_base;
               vector[_count]=strim(start);
               _count++;
           }
           start=pstr+1;
           status=0;
       }
       pstr++;
    }
    if(*start != '\0')
    {
        vector=(char**)realloc(vector,((_count)+1)*sizeof(char*));
        vector[_count] = strim(start);
        _count++;
    }
    
    *count=_count;
    return vector;
}

char **lexicalpraser(char *cmd,int* count)//词法解析器
{
    /* 流程说明
     * 1. 先将命令字符串中的前后空格清除
     * 2. 以空格作为单词识别的依据，如果单词之间有多个空格，则将其视为一个空格
     * 3. 对于有多个空格的处理方法是，设置一个标志，当有空格（连续的l空格）的时候，这个标志将会加1，
     * 没有空格将会是0，从而可以通过这个标志来判断空格前后是否应该被处理成单词，而无需考虑空格的多少。
     * 4. 对于处理号的字符串的存储，可以用一个二重字符指针来存储
     *
     * */
    char* pstr=strim(cmd);
    char* start=pstr;
    char** vector=NULL;
    int _count=0;
    int status=0;
    while(*pstr)
    {
       if(isspace(*pstr))
       {
           status=1;
       }
       if(status==1)
       {
           *pstr='\0';
           if(start!=NULL)
           {
               
               char** new_base = (char**)realloc(vector,(_count+1)*sizeof(char*));
               if(!new_base)
               {
                   perror("realloc");
               }
               vector=new_base;
               vector[_count]=strim(start);
               _count++;
           }
           start=pstr+1;
           status=0;
       }
       pstr++;
    }
    if(*start != '\0')
    {
        vector=(char**)realloc(vector,((_count)+1)*sizeof(char*));
        vector[_count] = strim(start);
        _count++;
    }
    
    *count=_count;
    return vector;
}

void produce_tip()
{
    struct passwd* pass;
    char name[128];
    char pwd[128];
    char *p;
    gethostname(name,sizeof(name)-1);//获取主机名字
    pass=getpwuid(getuid());//获取用户登录名
    p = getcwd(pwd,sizeof(pwd)-1);//获取当前路径
    printf("[%s@%s:%s]# ",pass->pw_name,name,p);
}
/* 管道实现原理
 * 1.Shell进程先调用pipe创建管道
 * 2.fork()子进程，有多少管道就创建多少子进程+1
 * 3.一个子进程关闭读端，调用dup2将写端赋给标准
 * 4.另一个子进程关闭写端，调用dup2把读端赋给标准输入
 * 5.多个子进程分别调用exev执行程序，而shell进程把管道的两端都关闭，调用wait等待子进程结束
 * 对于有多个管道实现进程通信的方法
 * 用j对创建的子进程进行编号，(j+1)%2!=0的为写端进程同时关闭读端，(j+1)%2==0的为读端进程同时关闭写端
 * */
void do_pipe(char* cmd)
{
       int count=0,j=0;
       int fd[2];
       pid_t pid;
       char** tmp=strsplit2(strim(cmd),'|',&count);
       //创建管道pipe
        pipe(fd);
       for(j;j<count;j++)
       {
            pid=fork();
           if(pid==0)
               break;
       }
       if(j<count)
       {
           //printf("第%d个子进程\n",j+1); 
           int c=0;
           char**args=NULL;
           args=lexicalpraser(strim(tmp[j]),&c);
           if((j+1)%2!=0)//写，关闭读端
           {
               close(fd[0]);
               dup2(fd[1],STDOUT_FILENO);
           }else//读，关闭写端
           {
               close(fd[1]);
               dup2(fd[0],STDIN_FILENO);
            
           }

           execvp(args[0],args);
       }
       else
       {
           close(fd[0]);
           close(fd[1]);
           for(int j=0;j<count;j++)
               wait(NULL);
       }
}
//左重定向
void do_lredirect(char* cmd)
{
    int count=0;
	int is_lredirect=getDelimNum(cmd,'<');
    char** tmp=strsplit2(cmd,'<',&count);
    if(count<=is_lredirect)
    {
        perror("command < [option]");
    }
    for(int i=1;i<count;i++)
    {
        int fd=open(tmp[i],O_RDWR|O_CREAT|O_TRUNC,0664);
        if(fd==-1) 
        {
            perror("open");
            exit(1);
        }
        //重定向
        dup2(1,fd);
        close(fd);
    }
   int c=0;
   char** args=lexicalpraser(tmp[0],&c);
   execvp(args[0],args);
                
}

//右重定向
void do_rredirect(char* cmd)
{
    int count=0;
    int is_rredirect=getDelimNum(cmd,'>');
    char** tmp=strsplit2(cmd,'>',&count);
    if(count<=is_rredirect)
    {
        perror("command > [option]");
        exit(1);
    }
    for(int i=1;i<count;i++)
    {
        int fd=open(tmp[i],O_RDWR|O_CREAT|O_TRUNC,0664);
        if(fd==-1) 
        {
            perror("open");
            exit(1);
        }
        //重定向
        dup2(fd,1);
        close(fd);
    }
    int c=0;
    char** args=lexicalpraser(tmp[0],&c);
    execvp(args[0],args);

}
//系统调用
void do_systemcall(char* cmd)
{
    int count=0;
    char** args=lexicalpraser(cmd,&count);
    execvp(args[0],args);
            
}
int main()
{

    while(1)
    {
        int i=0;
        char cmd[128]={0};
        char** args;
        produce_tip();
        fgets(cmd,sizeof(cmd)-1,stdin);
        int len=strlen(cmd);
        cmd[len-1]='\0';
        //按exit退出程序
        if(strcmp(cmd,"exit")==0)
        {
                printf("exit process\n");
                exit(1);
        }

          
        /*
         * 程序流程思路
         * 如果有重定向符('<','>')、管道('|')、上级目录符号('.'、'..')等，
         * 先要判定命令行中是否有这些字符，
         * 如果有则按照指定的程序运行，
         * 否则按照系统调用命令来运行。
         *
         * */
      
       
        pid_t pid=fork();

         if(pid==0)
         {  
            int is_lredirect=getDelimNum(cmd,'<');
            int is_rredirect=getDelimNum(cmd,'>');
            int is_pipe = getDelimNum(cmd,'|');
            //int is_1dirc = getDelimNum(cmd,'.');
            if(is_lredirect)
            {
                //printf("这里是左重定向,有%d个左重定向符\n",is_lredirect);
                do_lredirect(strim(cmd));

            }
            else if(is_rredirect)
            {
                //printf("这里是右重定向,有%d个右重定向符\n",is_rredirect);
                do_rredirect(strim(cmd));
               
            }
            else if(is_pipe)
            {
                //printf("这里是管道,有%d个管道符\n",is_pipe);
                do_pipe(strim(cmd));
            }
            else{
                do_systemcall(strim(cmd));
               }
 
         }else if(pid>0)
         {
             //printf("parent process\n");
             wait(NULL);
         }else
         {
             perror("fork");
             exit(1);
         }

        //自定义程序调用


    }

}




#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>

void cmdparser(char *cmd,char** argv)
{
    int i=0;
    argv[i++]=strtok(cmd," ");
    while(1)
    {
      argv[i]=strtok(NULL," ");
      if(argv[i] == NULL) break;
      i++;
    }
 
}


int main()
{
 /*   while(1)
    {
        char* argv[1024]={0};
        int i=0;
        char sen[1024]={0};
        read(0,sen,sizeof(sen));
        char *p;
        p=strtok(sen," ");
        while(p)
        {
            argv[i]=p;
            printf("%s\n",argv[i]);
            p=strtok(NULL," ");
        }
    }*/

    while(1)
    {
        pid_t p_id;
        char cmd[1024];
        char *argv[1024];
        printf("请输入命令\n");
        int fsize=read(0,cmd,sizeof(cmd));
        if(fsize>0)
        {
            int i=0;
            char* tokenPtr=strtok(cmd," ");
            while(tokenPtr!=NULL)
            {
                argv[i] = tokenPtr;
                printf("%s ",argv[i]);
                tokenPtr = strtok(NULL," ");
                i++;

            }
            printf("\n");
            
            p_id=fork();
            if(p_id==0)
            {
                
               int ret = execvp(argv[0],argv);
               if(ret==-1)
               {
                   perror("execvp");
                   exit(1);
               }
                
            }
            else if(p_id>0)
            {
                wait(NULL);
            }else
            {
                perror("create process");
                exit(1);

            }
        


        }else
        {
            perror("read");
        }
       
    }


}

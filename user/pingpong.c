#include "kernel/types.h"
#include "user/user.h"

void main(int argc, char const *argv[])
{
//管道
    char trans='p';
    int ptc[2];
    int ctp[2];
    int ret1 = pipe(ptc);
    int ret2 = pipe(ctp);
    int exit_status =0;
    if (ret1==-1 || ret2==-1)
    {
        write(2,"pipe error",10);
    }

    int pid = fork();
    if(pid <0)
    {
        close(ptc[0]);
        close(ptc[1]);
        close(ctp[0]);
        close(ctp[1]);
    }else if (pid == 0)
    {
       close(ptc[1]);
       close(ctp[0]);
       if(read(ptc[0],&trans,sizeof(char))!=sizeof(char))
       {
            fprintf(2,"ptc,read error");
            exit_status = 1;
       }else
       {
            fprintf(1,"%d:received ping\n",getpid());

       }

       if(write(ctp[1],&trans,sizeof(char))!=sizeof(char))
       {
                fprintf(2,"ctp,write error");
            exit_status = 1;
       }

       close(ptc[0]);
       close(ctp[1]);
       exit(exit_status);
    }
    else{
        close(ptc[0]);
        close(ctp[1]);
        if(write(ptc[1],&trans,sizeof(char))!=sizeof(char))
        {
            fprintf(2,"ptc,write error");
            exit_status = 1;
        }

        if (read(ctp[0],&trans,sizeof(char))!=sizeof(char))
        {
           fprintf(2,"ctp,read error");
            exit_status = 1;
        }else
        {
            fprintf(1,"%d:received pong\n",getpid());

        }
        close(ptc[1]);
        close(ctp[0]);

        exit(exit_status);
    }
    return;
}
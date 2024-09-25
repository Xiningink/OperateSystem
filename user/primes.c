#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1

void l2r(int lpipe[2],int rpipe[2],int first)
{
    int data;
    while (read(lpipe[RD],&data,sizeof(int))==sizeof(int))
    {
        if(data%first)
        {
            write(rpipe[WR],&data,sizeof(int));
        }

    }

    close(lpipe[RD]);
    close(rpipe[WR]);

}

void primes(int lpipe[2])
{
    close(lpipe[WR]);



    int first;
    int flag = read(lpipe[RD],&first,sizeof(int));

    if(flag)
    {//读取成功证明无法整除数列中还存在数字，继续遍历
        fprintf(1,"prime %d\n",first);

        int p[2];
        int ret = pipe(p);
        if(ret == -1)
        {
            fprintf(2,"pipe error");
        }

        l2r(lpipe,p,first);

        if(fork()==0)
        {
            primes(p);
        }
        else
        {
            close(lpipe[RD]);
            wait(0);
         }
    }


    exit(0);
    return;
}
int main(int argc, char const *argv[]){
    int p[2];
    int ret = pipe(p);
    if(ret == -1)
    {
        fprintf(2,"pipe error");
    }
//写入数据流
    for(int i =2;i<=35;i++)
    {
        write(p[WR],&i,sizeof(int));
    }


    if (fork()==0)
    {
        primes(p);
    }
    else{
        close(p[WR]);
        close(p[RD]);
        wait(0);
    }
    
    exit(0);
    return 0;
}
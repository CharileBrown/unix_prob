#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int glob=6;
char buf[]="a write ro stdout/n";
int main()
{
        int var;
        pid_t pid;
        printf("a write to stdout\n");
        //fflush(NULL);
        if((pid=fork())<0)
        {
                printf("fork error");
        }
        else
        {
                if(pid==0)
                {
                        glob++;
                        var++;
               }
                else
                {
                        sleep(2);
                }
        }
        printf("pid=%d,glob=%d,var=%d\n",getpid(),glob,var);
        exit(0);
}


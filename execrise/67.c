#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main( int argc,char **argv  )
{
    int val;
    if(argc!=2)
    {
        fprintf(stderr,"num of arg is not right.\n");
        exit(1);
    }
    int fd = atoi(argv[1]);
    if( (val=fcntl(fd,F_GETFL,0)) <0  )
    {
        fprintf(stderr,"Fail to get file state.\n");
        exit(1);
    }
    switch(val & O_ACCMODE){
        case O_RDONLY:
            printf("read only");
            break;
        case O_WRONLY:
            printf("write only");
            break;
        case O_RDWR:
            printf("read write");
            break;
        default:
            printf("unknown case mode");}
    if(val & O_APPEND)
        printf(", append");
    if(val & O_NONBLOCK)
        printf(", nonblocking");
    if(val & O_SYNC)
        printf(", synchronous writes");
    putchar('\n');



    return 0;
}


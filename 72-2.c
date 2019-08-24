#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
int mydup2(int fd1,int fd2)
{
    int tmp_fd = 3;
    if(fd1<0||fd2<0)
        return -1;
    while( tmp_fd<fd2  ){
        tmp_fd = dup(fd);
        if (tmp_fd < 0)
            return -1;
    }
    if( tmp_fd == fd2  )
        return fd2;
    else
    {
        close(fd2);
        dup(fd);
        return fd;
    }
    

int main()
{
    int fd = open("./word.txt",O_RDWR)  
    mydup2()
    return 0;
}


#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
int main()
{
    struct stat statbuf;
    if(stat("foo",&statbuf)<0){
        fprintf(stderr,"stat foo error\n");
        exit(1);
    }
    if(chmod("foo",(statbuf.st_mode&~S_IXGRP)|S_ISGID)<0)
        fprintf(stderr,"chmod foo error\n");
    if(chmod("bar",S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)<0)
        fprintf(stderr,"chmod bar error\n");
    return 0;
}


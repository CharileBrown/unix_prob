#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

int main(int argc,char **argv)
{
    struct stat buf;
    int i;
    char *ptr;
    for(i=1;i<argc;i++){
        printf("%s: ",argv[i]);
        if(lstat(argv[i],&buf)<0){
            fprintf(stderr,"lstat error\n");
            continue;
        }
        if(S_ISREG(buf.st_mode))
            ptr = "regular";
        else if(S_ISDIR(buf.st_mode))
            ptr = "directory";
        else if(S_ISCHR(buf.st_mode))
            ptr = "character special";
        else
            ptr = "other mode";
        printf("%s\n",ptr);
    }
    return 0;
}


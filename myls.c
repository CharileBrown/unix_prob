#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
int main(int argc,char **argv)
{
    DIR *dp;
    struct dirent *dirp;
    if(argc != 2)
    {
        fprintf(stderr,"usage:ls directory_name");
        exit(0);
    }
    if( (dp=opendir(argv[1])) == NULL  )
    {
        fprintf(stderr,"can't open %s\n",argv[1]);
        exit(0);
    }
    while((dirp = readdir(dp)) != NULL)
        printf("%s\n",dirp->d_name);

    closedir(dp);
    return 0;
}


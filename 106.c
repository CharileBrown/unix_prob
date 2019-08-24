#include <stdio.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#define FTW_F 1
#define FTW_D 2
#define FTW_DNR 3
#define FTW_NS 4

static int deal(const char *fullpath, struct stat *statbuf,int situ)//deal with all situations
{
    switch(situ){
    case FTW_F: printf("file:%s\n",fullpath);break;
    case FTW_D: printf("dir:%s\n",fullpath);break;
    case FTW_DNR:fprintf(stderr,"error!can't open dir:%s\n",fullpath);break;
    case FTW_NS:fprintf(stderr,"error!can't stat file\n");break;
    }
    return 0;

}
static int myftw(char *fullpath)//visit all files
{
    struct stat statbuf;
    DIR *dp;
    struct dirent *dirp;
    int ret;
    if((dp = opendir(fullpath)) == NULL )
    {
        ret = deal(fullpath,NULL,FTW_DNR);
        return ret;
    }
    int len = strlen(fullpath);
    fullpath[len++] = '/';
    fullpath[len] = 0;
    while( (dirp = readdir(dp))!=NULL  )
    {
        if( strcmp(".",dirp->d_name)==0||strcmp("..",dirp->d_name)==0)
            continue;
        strcpy(&fullpath[len],dirp->d_name);
        if( lstat(fullpath,&statbuf) < 0  )
            deal(fullpath,&statbuf,FTW_NS);
        if( S_ISDIR(statbuf.st_mode)==0  )
            deal(fullpath,&statbuf,FTW_F);
        else
            ret = myftw(fullpath);
    }
    fullpath[len] = 0;
    if( closedir(dp) < 0   )
        fprintf(stderr,"can't close file:%s\n",fullpath);
    return ret;
}
int main(int argc,char **argv)
{
    if(argc != 2){
        fprintf(stderr,"Number of arg should be 2.\n");
        exit(1);
    }
    char pathname[512];
    realpath(argv[1],pathname);
   // printf("%s\n",pathname);
    //if( opendir(pathname)==NULL  )
        //printf("fuck");
    int ret = myftw(pathname);
    return 0;
}


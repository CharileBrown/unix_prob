#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void mysetbuf(FILE *restrict fp,char *restrict buf)
{
    if (buf == NULL)
    {
        setvbuf(fp,NULL,_IONBF,0);
        return;
    }
    int fd = fileno(fp);
    if ( isatty(fd)  ){
        setvbuf(fp,buf,_IOLBF,BUFSIZ);
        return;
    }
    setvbuf(fp,buf,_IOFBF,BUFSIZ);
}
int main()
{
    printf("Hello world\n");
    return 0;
}


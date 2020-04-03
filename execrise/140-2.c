#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define BUFFSIZE 10

int main()
{
    char buf[BUFFSIZE];
    FILE *file = fopen("./test.txt","r+");
    while(fgets(buf,BUFFSIZE,file)){
        fputs(buf,stdout);
        printf("|\n");
    }
    return 0;
}


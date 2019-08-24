#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define SIZE 10

int main()
{
    FILE *file = fopen("./test.txt","r");
    char buf[SIZE];
    fgets(buf,SIZE,file);
    printf("(%s)",buf);
    fgets(buf,SIZE,file);
    printf("(%s)",buf);
    return 0;
}


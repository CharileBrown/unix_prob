#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    char buf[100] = "adgf\ngergrgwef";
    fputs(buf,stdout);
    puts(buf);
    return 0;
}


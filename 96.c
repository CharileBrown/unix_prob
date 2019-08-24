#include <stdio.h>
#include <unistd.h>

int main()
{
    if( rename("96file","96dir") < -1  )
    {
        printf("error\n");
    }
    return 0;
}


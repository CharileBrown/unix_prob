#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    FILE *file = fopen("./test.txt","r");
    int flag = 1;
    while(1)
    {
        if( feof(file)&&flag  )
        {
            flag = 0;
            for(int i=0;i<7;i++)
                ungetc('z'-i,file);
        }
        else if( feof(file)&&!flag  )
            break;
        char c = getc(file);
        printf("(%c~%d)",c,c);
        //if( feof(file)  )
          //  break;
    }
    return 0;
}


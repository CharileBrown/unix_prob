#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct{
    int a;
    char b;
    long long c;
}ITEM;
int main()
{
    ITEM item,item2;
    item.a = 128;
    item.b = 'b';
    item.c = 256;
    fpos_t pos;
    FILE *file = fopen("./bin","r+b");
    fgetpos(file,&pos);
    fwrite(&item,sizeof(ITEM),1,file);
    fsetpos(file,&pos);
    fread(&item2,sizeof(ITEM),1,file);    
    printf("%d %c %lld\n",item2.a,item2.b,item2.c);
    return 0;
}


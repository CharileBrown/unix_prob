#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
int main()
{
	char s[10]="hello";
	for(int i=0;i<3;i++)
				printf("%s",&s[i]);
	char *str="abc";
	int a = 10;
	int *b = &a;
	*b = 20;
	int fd = open("test.txt",16450,0664);
	if (fd<0)
				printf("something error\n");
}

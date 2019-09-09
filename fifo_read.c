#include "common.h"

int main()
{
	char buf[1024];
	int status = mkfifo("fifo",0666);
	if( status<0&&errno!=EEXIST )
		perror("Create FIFO Failed");
	int fd = open("fifo",O_RDONLY);
	if( fd<0 )
	{
		perror("Open FIFO Failed");
		exit(1);
	}
	int len = 0;
	while( (len = read(fd,buf,1024))>0 )
		printf("Read message:%s",buf);
	close(fd);
	return 0;
}

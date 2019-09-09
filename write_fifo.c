#include "common.h"


int main()
{
	int fd;
	char buf[1024];
	time_t tp;
	printf("I am process %d\n",getpid());
	if( (fd = open("fifo",O_WRONLY|O_CREAT,0666)) < 0 )
	{
		perror("Open FIFO Failed.\n");
		exit(1);
	}	
	for( int i=0;i<10;i++ )
	{
		time(&tp);
		int n = sprintf(buf,"Process %d's time is %s",getpid(),ctime(&tp));
		printf("Send message:%s",buf);
		if( write(fd,buf,n+1) < 0 )
		{
			perror("Write FIFO Failed\n");
			close(fd);
			exit(1);
		}
		sleep(1);
	}
	close(fd);
	return 0;
}

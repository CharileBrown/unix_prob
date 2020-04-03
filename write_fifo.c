#include "unix.h"


int main()
{
	int fd;
	int n,i;
	char buf[1024];
	time_t tp;
	printf("I am %d process\n",getpid());
//	if( mknod("./fifo1",S_IFIFO|0666,0) < 0 )
//	{
//		perror("mknod error!");
//		exit(1);
//	}
	int res = mkfifo("fifo1",0777);
	if( res < 0  )
	{
		perror("mkfifo error!");
		exit(1);
	}
	if( (fd  = open("fifo1",O_WRONLY)) < 0 )
	{
		fprintf(stderr,"Open FIFO Failed");
		exit(1);
	}
	for(i=0;i<10;i++)
	{
		time(&tp);
		n=sprintf(buf,"Process %d's time is %s",getpid(),ctime(&tp));
		printf("Send message:%s",buf);
		if( write(fd,buf,n+1) < 0 )
		{
			fprintf(stderr,"Write FIFO Failed");
			close(fd);
			exit(1);
		}
		sleep(1);
	}
	close(fd);
	return 0;
}

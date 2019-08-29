#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>

pthread_t ntid;

void printids( const char *s )
{
	pid_t pid;
	pthread_t tid;
	pid = getpid();
	tid = pthread_self();
	printf("%s pid %lu tid %lu\n",s,(unsigned long)pid,(unsigned long)tid);
}

void *thr_fn( void *arg )
{
	printids("new therad: ");
	return( (void *)0 );
}

int main()
{
	int err;
	err = pthread_create(&ntid,NULL,thr_fn,NULL);
	if( err!=0 )
	       fprintf(stderr,"pthread_create error!\n");
	printids("main thread:");
	sleep(1);
	return 0;	
}

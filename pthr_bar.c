#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <pthread.h>

pthread_barrier_t b;

void *thr_fn( void *arg )
{
	pthread_barrier_wait(&b);
	printf("this is not main thread\n");
}

int main()
{
	pthread_t tid;
	pthread_barrier_init(&b,NULL,7);
	for( int i=0;i<6;i++ ){
		int err = pthread_create(&tid,NULL,thr_fn,NULL);
		if( err!=0 )
			fprintf(stderr,"pthread create error!\n");
	}
	pthread_barrier_wait(&b);
	printf("this is main thread!\n");
	sleep(1);
	pthread_barrier_destroy(&b);
}

#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<pthread.h>
#include<unistd.h>


pthread_mutex_t mutex;
pthread_mutexattr_t attr;

void *thr_fn(void *arg)
{
	pthread_mutex_lock(&mutex);
	printf("this is %ld thread\n",(long)arg);
	pthread_mutex_unlock(&mutex);
}

int main()
{
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex,&attr);
	for( long i=0;i<10;i++ ){
		pthread_t tid;
		pthread_create(&tid,NULL,thr_fn,(void *)i);
	}
	sleep(1);
}

#include "common.h" 


pthread_mutex_t mutex;
pthread_mutexattr_t attr;

void sig_deal(int signo)
{
	pthread_mutex_lock(&mutex);
	printf("this is the signal deal function\n");
	pthread_mutex_unlock(&mutex);	
}


void fun()
{
	pthread_mutex_lock(&mutex);
	printf("this is the fun\n");
	sleep(10);
	pthread_mutex_unlock(&mutex);
}

int main()
{
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex,&attr);
	pthread_mutexattr_destroy(&attr);
	signal(SIGINT,sig_deal);
	fun();
	return 0;
}

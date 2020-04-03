#include "common.h"

int quitflag;
sigset_t mask;
pthread_cond_t waitloc = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void *thr_fn(void *arg)
{
	int err,signo;
	while(1)
	{
		err = sigwait(&mask,&signo);
		if(err!=0)
			fprintf(stderr,"sigwait error\n");
		switch(signo)
		{
			case SIGINT:
				printf("\ninterrupt\n");
				break;
			case SIGQUIT:
				pthread_mutex_lock(&mutex);
				quitflag = 1;
				pthread_mutex_unlock(&mutex);
				pthread_cond_signal(&waitloc);
				return 0;
			default:
				printf("unexpected signal %d\n",signo);
				exit(1);
		}
	}
}
int main()
{
	int err;
	sigset_t oldmask;
	pthread_t tid;
	quitflag = 0;
	sigemptyset(&mask);
	sigaddset(&mask,SIGINT);
	sigaddset(&mask,SIGQUIT);
	if( (err = pthread_sigmask(SIG_BLOCK,&mask,&oldmask))!=0 )
		fprintf(stderr,"pthread_sigmask error\n");
	err = pthread_create(&tid,NULL,thr_fn,0);
	if(err!=0)
		fprintf(stderr,"can not create pthread");
	pthread_mutex_lock(&mutex);
	while(quitflag == 0)
		pthread_cond_wait(&waitloc,&mutex);
	pthread_mutex_unlock(&mutex);
	if( sigprocmask(SIG_SETMASK,&oldmask,NULL) <0 )
		fprintf(stderr,"sigpromask error");
	return 0;
}

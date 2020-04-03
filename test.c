<<<<<<< HEAD
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

void sig_usr1( int signo ){
	printf("has received the signal\n");
}


int main()
{
    sigset_t oldset,newset;
    struct sigaction action;
    action.sa_handler = sig_usr1;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigemptyset(&newset);
    sigemptyset(&oldset);
    sigaddset(&newset,SIGUSR1);
    sigprocmask(SIG_BLOCK,&newset,&oldset);
    sigaction(SIGUSR1,&action,NULL);
    kill(getpid(),SIGUSR1);
    sigsuspend(&oldset);

    return 0;
=======
#include "unix.h"

typedef struct tt{
	int a;
}tt;

int main()
{
	tt test;
	test.a = 10;
>>>>>>> the first commit
}


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){
	pid_t pid;
	if( (pid = fork()) < 0 ){
		fprintf(stderr,"fork error");
		exit(1);
	} 
	else if( pid==0 ){
		if(execl("./job1","job1","myarg1","MY ARG2",(char *)0)<0)
			fprintf(stderr,"execl error");
			exit(2);
	}
	if( waitpid(pid,NULL,0)<0 ){
		fprintf(stderr,"waitpid error");
		exit(3);	
	}
	       
	return 0;	
}

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
int globvar = 6;
int main(){
	int var;
	pid_t pid;
	var = 88;
	printf("before vfork\n");
	if(( pid = vfork())<0)
		fprintf(stderr,"vfork error");
	else if(pid == 0){
		globvar++;
		var++;
		exit(0);
	}
	printf("pid=%ld, glob = %d, var = %d\n",(long)getpid(),globvar,var);
	exit(0);
}

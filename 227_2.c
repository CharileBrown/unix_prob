#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

void fun(){
	pid_t pid;
	if( (pid=vfork()) < 0  ){
		fprintf(stderr,"vfork error");
		exit(1);
	}
	else if( pid == 0 ){
		return;
	} 
	printf("this is a parent process\n");
}

int main(){
	fun();
}

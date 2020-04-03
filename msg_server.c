#include "unix.h"

#define MSG_FILE "./char.c"

struct msg_form{
	long mtype;
	char mtext[256];
};

int main()
{
	int msqid;
	key_t key;
	struct msg_form msg;
	if( (key = ftok(MSG_FILE,'z')) <0 )
	{
		perror("ftok error");
		exit(1);
	}
	printf("Message Queue-Server key is %d\n",key);
	if((msqid=msgget(key,IPC_CREAT|0666)) <0  )
	{
			perror("msgget error");
			exit(1);
	}
	printf("My msqid is: %d\n",msqid);
	printf("My pid is:%d\n",getpid());
	for(;;)
	{
		int len = msgrcv(msqid,&msg,256,888,0);
		//if(len<0)
		//	perror("can not receive");
		printf("Server:receive msg.mtext is: %s\n",msg.mtext);
		printf("Server:receive msg.mtype is:%lld\n",msg.mtype);
		msg.mtype = 999;
		sprintf(msg.mtext,"hello,I'm server %d",getpid());
		msgsnd(msqid,&msg,sizeof(msg.mtext),IPC_NOWAIT);
	}
}

<<<<<<< HEAD
#include "common.h"

struct msg
{
	long mtype;
	char content[256];
=======
#include "unix.h"

#define MSG_FILE "./char.c"

struct msg_form{
	long mtype;
	char mtext[256];
>>>>>>> the first commit
};

int main()
{
<<<<<<< HEAD
	struct msg msg_item;
	key_t key = ftok("./test.txt",256);
	if(key < 0)
	{
		perror("ftok error\n");
		exit(1);
	}
	printf("Message Queue-Server key is %d\n",key);
	int msgqid;
	if((msgqid = msgget(key,IPC_CREAT|0777))==-1)
	{
		perror("message error");
		exit(1);
	}
	printf("My msqid is %d\n",msgqid);
	printf("My pid is %d\n",getpid());
	while(1)
	{
		msgrcv(msgqid,&msg_item,256,888,0);
		printf("Server:receive msg.content is %s\n",msg_item.content);
		printf("Server:receive msg.content is %ld\n",msg_item.mtype);
		msg_item.mtype = 999;
		sprintf(msg_item.content,"Hello,I am server %d",getpid());
		msgsnd(msgqid,&msg_item,sizeof(msg_item.content),0);
	}

=======
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
>>>>>>> the first commit
}

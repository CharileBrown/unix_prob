#include "common.h"

struct msg
{
	long mtype;
	char content[256];
};

int main()
{
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

}

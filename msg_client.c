#include "common.h"

struct msg{
	long mtype;
	char content[256];
};

int main()
{
	key_t key = ftok("./test.txt",256);
	if( key<0 )
	{
		perror("ftok error");
		exit(1);
	}
	printf("Message Queue-Client key is %d\n",key);
	int msgqid = msgget(key,IPC_CREAT|0777);
	if( msgqid == -1 )
	{
		perror("msgget error\n");
		exit(1);
	}
	printf("My msgqid is %d\n",msgqid);
	printf("My pid is %d\n",getpid());
	struct msg msg_item;
	msg_item.mtype = 888;
	sprintf(msg_item.content,"Hello,I'm client %d",getpid());
	msgsnd(msgqid,&msg_item,sizeof(msg_item.content),0);

	msgrcv(msgqid,&msg_item,256,999,0);
	printf("Client:receive content is %s\n",msg_item.content);
	printf("Client:receive type is %ld\n",msg_item.mtype);
	return 0;
}

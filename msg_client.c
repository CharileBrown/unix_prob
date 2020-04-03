<<<<<<< HEAD
#include "common.h"

struct msg{
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
	key_t key = ftok("./test.txt",256);
	if( key<0 )
=======
	int msqid;
	key_t key;
	struct msg_form msg;
	if( (key = ftok(MSG_FILE,'z')) <0 )
>>>>>>> the first commit
	{
		perror("ftok error");
		exit(1);
	}
<<<<<<< HEAD
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
=======
	printf("Message Queue - Client Key is %d\n",key);
	if ((msqid = msgget(key, IPC_CREAT|0666)) == -1)
	{
			perror("msgget error");
			exit(1);
	}
	 printf("My msqid is: %d.\n", msqid);
	 printf("My pid is: %d.\n", getpid());
	 while(1){
	 msg.mtype = 888;
	 sprintf(msg.mtext,"hello,I'm client %d",getpid());
	 int res = msgsnd(msqid,&msg,sizeof(msg.mtext),0);
	 if(res < 0)
					 perror("error");
	 printf("sended\n");
	 //msgrcv(msqid,&msg,256,999,0);
	 //printf("Client:receive msg.mtext is:%s\n",msg.mtext);
	 //printf("Client:receive msg.mtype is:%lld\n",msg.mtype);
	 sleep(2);
	 }
	 return 0;
>>>>>>> the first commit
}

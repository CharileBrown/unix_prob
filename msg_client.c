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
}

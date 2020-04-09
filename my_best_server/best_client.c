#include "unix.h"

#define SERVER_PORT 6666

int main()
{
	int serverSocket,clientSocket;
	struct sockaddr_in client_addr;
	struct sockaddr_in server_addr;
	char sendbuf[1024],recvbuf[1024];
	while(1){
	if( (clientSocket = socket(AF_INET,SOCK_STREAM,0)) < 0 )
	{
			perror("client: create socket failed");
			exit(1);
	}
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if( connect(clientSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("client: connect to server failed");
		exit(1);
	}
	printf("连接到主机...\n");
	sleep(1);
	}
	while(1)
	{
		printf("发送消息:");
		scanf("%s",sendbuf);
		write(clientSocket,sendbuf,strlen(sendbuf));
		if( strcmp(sendbuf,"quit") ==0 )
			break;
		printf("读取消息:\n");
		int count = read(clientSocket,recvbuf,1024);
		if( count<0 )
		{
			printf("read error\n");
			exit(1);
		}
		printf("%s\n",recvbuf);
	}
}

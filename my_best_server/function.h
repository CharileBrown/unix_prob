#ifndef UNIX_H
#include "unix.h"
#endif


void *deal_with_connect(void *client_pool);

void *deal_with_connect(void *client_pool )
{
	
	clientpool_t * clientpool = (clientpool_t*) client_pool;
	int client_fd;
	struct sockaddr_in client_addr;
	pthread_mutex_lock(&(clientpool->lock));
		int queue_size = clientpool->queue_size;
		if( queue_size == 0 )
		{
					pthread_mutex_unlock(&(clientpool->lock));
					return NULL;
		}
		client_fd =  clientpool->fd_queue[clientpool->queue_front];
		client_addr = clientpool->addr_queue[clientpool->queue_front];
		clientpool->queue_size--;
		clientpool->queue_front = ( clientpool->queue_front + 1 )%clientpool->max_num;
	pthread_mutex_unlock(&(clientpool->lock));
	char buf[2048];
	printf("读取消息:");
	buf[0] = '\0';
	int dataNum = read(client_fd,buf,2048);
	if(dataNum < 0)
	{
		perror("read error");
		close(client_fd);
		return NULL;
	}
	if( strcmp(buf,"quit") == 0 )
	{
			printf("The connect from %s%d is close\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
			close(client_fd);
			return NULL;
	}
	
}

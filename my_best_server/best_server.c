#include "thread_pool.h"
#include "function.h"
#define SERVER_PORT 6666

/*typedef struct clientpool_t
{
		pthread_mutex_t lock; //用于锁住整个结构体
		int *fd_queue;
		struct sockaddr_in *addr_queue;
		int max_num;
		int queue_size;
		int queue_front;
		int queue_rear;
}clientpool_t;*/

extern void *deal_with_connect(void *);

clientpool_t * clientpool_create(int max_num)
{
	clientpool_t *clientpool = NULL;
	if( (clientpool = (clientpool_t *)malloc(sizeof(clientpool_t))) == NULL )
	{
		printf("malloc clientpool error\n");
		return NULL;
	}
	clientpool->queue_size = clientpool->queue_front = clientpool->queue_rear = 0;
	clientpool->max_num = max_num;
	clientpool->fd_queue = (int *)malloc(sizeof(int)*max_num);
	if( clientpool->fd_queue == NULL )
	{
		printf("malloc fd_queue error\n");
		return NULL;
	}
	memset(clientpool->fd_queue,0,sizeof(clientpool->fd_queue));
	clientpool->addr_queue = (struct sockaddr_in *)malloc(sizeof(sockaddr_in)*max_num);
	if( clientpool->addr_queue == NULL)
	{
		printf("malloc addr_queue error\n");
		return NULL;
	}
	memset(clientpool->addr_queue,0,sizeof(clientpool->addr_queue));
	if( pthread_mutex_init( &(clientpool->lock),NULL) !=0 )
	{
		printf("init clientpool lock false\n");
		return NULL;
	}
	return clientpool;
}

int main()
{
	int serverSocket;

	//两个sockaddr_in,分别表示客户端和服务器的地址信息
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	
	int addr_len = sizeof(client_addr);

	int client;

	char buffer[200];

	int iDataNum;

	if( (serverSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket create error");
		exit(1);
	}

	memset(&server_addr,0,sizeof(server_addr));

	server_addr.sin_family = AF_INET;

	server_addr.sin_port = htons(SERVER_PORT);

	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if( bind(serverSocket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 )
	{
		perror("bind error");
		return 1;
	}

	if( listen(serverSocket,100) < 0 )
	{
		perror("listen error");
		exit(1);
	}

threadpool_t * pool = threadpool_create(20,100,150);
clientpool_t *clientpool = clientpool_create(150);

	while(1)
	{
		printf("监听端口: %d\n",SERVER_PORT);
		client = accept(serverSocket,(struct sockaddr *)&client_addr,(socklen_t *)&addr_len);
		
		if(client < 0)
		{
			perror("accept error");
			continue;
		}
		
		printf("build a connect from %s:%d\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		pthread_mutex_lock(&(clientpool->lock));
			int queue_size = clientpool->queue_size, max_num = clientpool->max_num;
			if( queue_size == max_num )
			{
				char *str = (char *)"sorry,the connect is full\n";
				write(client,str,sizeof(str));
				close(client);
				pthread_mutex_unlock(&(clientpool->lock));
				continue;
			}
			else
			{
				clientpool->fd_queue[clientpool->queue_rear] = client;
				clientpool->addr_queue[clientpool->queue_rear] = client_addr;
				clientpool->queue_rear = (clientpool->queue_rear+1)%(clientpool->max_num);
				clientpool->queue_size = (clientpool->queue_size+1)%(clientpool->max_num);
			}
		pthread_mutex_unlock(&(clientpool->lock));
		threadpool_add_task(pool, deal_with_connect, (void *)clientpool);
	}

}

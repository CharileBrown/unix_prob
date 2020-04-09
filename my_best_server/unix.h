#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <algorithm>
#include <iostream>
#include <map>
#include <regex>
using namespace std;

#define UNIX_H

typedef struct clientpool_t
{
		pthread_mutex_t lock; //用于锁住整个结构体
		int *fd_queue;
		struct sockaddr_in *addr_queue;
		int max_num;
		int queue_size;
		int queue_front;
		int queue_rear;
}clientpool_t;

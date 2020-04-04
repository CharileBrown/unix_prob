#ifndef UNIX_H
#include "unix.h"
#endif

#define DEFAULT_TIME 5
#define MIN_WAIT_TASK_NUM 10
#define DEFAULT_THREAD_NUM 10

/*任务*/
typedef struct threadpool_task_t{
	void *(*function)(void *);
	void *arg;
}threadpool_task_t;

/*线程池管理*/
typedef struct threadpool_t{
	pthread_mutex_t lock; //锁住整个结构体
	pthread_mutex_t thread_counter; //用于使用忙线程数时的锁
	pthread_cond_t queue_not_full; //条件变量,任务队列不为满
	pthread_cond_t queue_not_empty; //条件变量,任务队列不为空

	pthread_t *threads; //存放线程的tid的数组
	pthread_t admin_tid; //管理者线程tid
	threadpool_task_t *task_queue; //任务队列

	int min_thr_num; //线程池中最小线程数
	int max_thr_num; //线程池中最大线程数
	int live_thr_num; //线程池中存活线程数
	int busy_thr_num; //忙线程
	int wait_exit_thr_num; //需要销毁的线程数

	/*任务队列信息*/
	int queue_front; //队头
	int queue_rear; //队尾
	int queue_size;

	/*存在的任务数*/
	int queue_max_size; //队列能容纳的最大任务数
	/*线程池状态*/
	int shutdown; //true为关闭
}threadpool_t;

void *admin_thread(void *threadpool);
void *threadpool_thread(void *threadpool);
int is_thread_alive(pthread_t pid);
int threadpool_free(threadpool_t *pool);

/*创建线程池 参数：最小线程数，最大线程数，任务队列最大值*/
threadpool_t * threadpool_create(int min_thr_num,int max_thr_num,int queue_max_size)
{
	int i;
	threadpool_t *pool = NULL;
	do
	{
		if( (pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL )
		{
			printf("malloc threadpool false;\n");
			break;
		}
		pool->min_thr_num = min_thr_num;
		pool->max_thr_num = max_thr_num;
		pool->busy_thr_num = 0;
		pool->live_thr_num = min_thr_num;
		pool->wait_exit_thr_num = 0;
		pool->queue_front = 0;
		pool->queue_rear = 0;
		pool->queue_size = 0;
		pool->queue_max_size = queue_max_size;
		pool->shutdown = 0;

		/*为线程数组开辟空间*/
		pool->threads = (pthread_t *)malloc(sizeof(pthread_t)*max_thr_num);
		if( pool->threads == NULL )
		{
			printf("malloc threads false\n");
			break;
		}
		memset( pool->threads,0,sizeof(pthread_t)*max_thr_num );

		/*为任务队列开辟空间*/
		pool->task_queue = (threadpool_task_t *)malloc(sizeof(threadpool_task_t)*queue_max_size);
		if(pool->task_queue == NULL)
		{
			printf("malloc task queue false");
			break;
		}

		/*初始化互斥锁和条件变量*/
		if( pthread_mutex_init(&(pool->lock),NULL) !=0 || pthread_mutex_init(&(pool->thread_counter), NULL)!=0 ||
		pthread_cond_init(&(pool->queue_not_empty),NULL)!=0 || pthread_cond_init(&(pool->queue_not_full),NULL)!=0 )
		{
			printf("init lock or cond false\n");
			break;
		}

		/*启动min_thr_num个工作线程*/
		for( int i = 0; i < min_thr_num; i++ )
		{
			pthread_create(&(pool->threads[i]),NULL,threadpool_thread,(void *)pool);
			printf("start thread 0x%x...\n",(unsigned int)pool->threads[i]);
		}

		/*启动管理者线程*/
		pthread_create(&(pool->admin_tid),NULL,admin_thread,(void *)pool);
		return pool;
	}while(0);
	threadpool_free(pool);
	return NULL;
}

/*工作线程*/
void *threadpool_thread(void *threadpool)
{
	threadpool_t *pool = (threadpool_t *) threadpool;
	threadpool_task_t task;

	while( 1 )
	{
		pthread_mutex_lock(&(pool->lock));
		
		/*无任务则阻塞在 “任务队列不为空” 上，有任务则跳出*/
		while( (pool->queue_size == 0) && (!pool->shutdown) )
		{
			printf("thread 0x%x is waiting \n", (unsigned int)pthread_self());
			pthread_cond_wait(&(pool->queue_not_empty), &(pool->lock));

			/*判断是否需要清除线程，自杀功能*/
			if( pool->wait_exit_thr_num > 0 )
			{
				pool->wait_exit_thr_num--;
				/*判断线程池中的线程数是否大于最小线程数，是则结束当前线程*/
				if( pool->live_thr_num >pool->min_thr_num )
				{
					printf("thread 0x%x is exiting\n", (unsigned int)pthread_self());
					pool->live_thr_num--;
					pthread_mutex_unlock(&(pool->lock));
					pthread_exit(NULL);
				}
			}
		}

		/*线程池开关状态*/
		if( pool->shutdown )//关闭线程池
		{
			pthread_mutex_unlock( &(pool->lock) );
			printf("thread 0x%x is exiting \n",(unsigned int)pthread_self());
			pthread_exit(NULL);
		}

		/*否则线程可以拿出任务*/
		task.function = pool->task_queue[pool->queue_front].function; //出队操作
		task.arg = pool->task_queue[pool->queue_front].arg;

		pool->queue_front = (pool->queue_front + 1) % pool->queue_max_size; //环形队列
		pool->queue_size--;

		/*通知可以添加新任务*/
		pthread_cond_broadcast(&(pool->queue_not_full));

		pthread_mutex_unlock(&(pool->lock));

		/*执行刚刚取出的任务*/
		printf("thread 0x%x start working\n",(unsigned int)pthread_self());
		pthread_mutex_lock(&(pool->thread_counter));
		pool->busy_thr_num++;
		pthread_mutex_unlock(&(pool->thread_counter));

		(*(task.function))(task.arg);

		/*任务处理结束*/
		printf("thread 0x%x end working \n",(unsigned int)pthread_self());
		pthread_mutex_lock(&(pool->thread_counter));
		pool->busy_thr_num--;
		pthread_mutex_unlock(&(pool->thread_counter));
	}
	pthread_exit(NULL);
}

/*向线程池的任务队列中添加一个任务*/
int threadpool_add_task(threadpool_t *pool, void *(*function)(void *arg), void *arg)
{
	pthread_mutex_lock( &(pool->lock) );

	/*如果队列满了，调用wait阻塞*/
	while( (pool->queue_size == pool->queue_max_size) && (!pool->shutdown) )
			pthread_cond_wait(&(pool->queue_not_full),&(pool->lock));

	/*如果线程池处于关闭状态*/
	if( pool->shutdown )
	{
			pthread_mutex_unlock(&(pool->lock));
			return -1;
	}

	/*清空工作线程的回调函数的参数arg*/
	if( pool->task_queue[pool->queue_rear].arg != NULL )
	{
		free( pool->task_queue[pool->queue_rear].arg );
		pool->task_queue[pool->queue_rear].arg = NULL;
	}

	/*添加任务到队列*/
	pool->task_queue[pool->queue_rear].function = function;
	pool->task_queue[pool->queue_rear].arg = arg;
	pool->queue_rear = (pool->queue_rear+1)%pool->queue_max_size;
	pool->queue_size++;

	/*添加完任务后，队列就不为空了，唤醒一个等待的线程*/
	pthread_cond_signal(&(pool->queue_not_empty));
	pthread_mutex_unlock(&(pool->lock));

	return 0;
}



/*管理者线程*/
void *admin_thread( void *threadpool )
{
	int i;
	threadpool_t *pool = (threadpool_t *)threadpool;
	while( !pool->shutdown )
	{
		printf("admin-------------------------\n");
		sleep(DEFAULT_TIME); //隔一段时间再管理
		pthread_mutex_lock(&(pool->lock));
		int queue_size = pool->queue_size;
		int live_thr_num = pool->live_thr_num;
		pthread_mutex_unlock(&(pool->lock));

		pthread_mutex_lock(&(pool->thread_counter));
		int busy_thr_num = pool->busy_thr_num;
		pthread_mutex_unlock(&(pool->thread_counter));

		printf("admin busy live -%d--%d-\n",busy_thr_num, live_thr_num);
		/*创建新线程 实际任务数量大于 最小正在等待的任务数量， 存活线程小于最大线程数*/
		if( queue_size >= MIN_WAIT_TASK_NUM && live_thr_num <= pool->max_thr_num )
		{
			printf("admin add--------------\n");
			pthread_mutex_lock(&(pool->lock));
			int add = 0;

			pool->wait_exit_thr_num = 0;
			/*一次增加DEFAULT_THREAD_NUM个线程*/
			for( i=0; i< pool->max_thr_num && add < DEFAULT_THREAD_NUM && pool->live_thr_num < pool->max_thr_num;i++ )
			{
				if( pool->threads[i]==0 || is_thread_alive(pool->threads[i]) )
				{
					pthread_create(&(pool->threads[i]),NULL,threadpool_thread,(void *)pool);
					add++;
					pool->live_thr_num++;
					printf("new thread----------------------\n");
				}
			}

			pthread_mutex_unlock(&(pool->lock));
		}



		if( (busy_thr_num*2) < live_thr_num && live_thr_num > pool->min_thr_num )
		{
			/*一次销毁DEFAULT_THREAD_NUM个线程*/
			pthread_mutex_lock(&(pool->lock));
			pool->wait_exit_thr_num = DEFAULT_THREAD_NUM;
			pthread_mutex_unlock(&(pool->lock));

			for( int i=0;i<DEFAULT_THREAD_NUM;i++ )
			{
				pthread_cond_signal(&(pool->queue_not_empty));
				printf("admin clear--\n");
			}
		}

	}
	return NULL;
}



/*判断线程是否存活*/
int is_thread_alive(pthread_t tid)
{
	int kill_rc = pthread_kill(tid,0);
	if( kill_rc == ESRCH )
		return 0;
	return 1;
}



/*释放线程池*/
int threadpool_free( threadpool_t *pool )
{
	if( pool == NULL )
			return -1;

	if(pool->task_queue)
			free(pool->task_queue);
	if(pool->threads)
	{
		free(pool->threads);
		pthread_mutex_lock(&(pool->lock)); //先锁住再销毁
		pthread_mutex_destroy(&(pool->lock));
		pthread_mutex_lock(&(pool->thread_counter));
		pthread_mutex_destroy(&(pool->thread_counter));
		pthread_cond_destroy(&(pool->queue_not_empty));
		pthread_cond_destroy(&(pool->queue_not_full));
	}
	free(pool);
	pool = NULL;
	return 0;
}



int threadpool_destroy(threadpool_t *pool)
{
	int i;
	if( pool == NULL )
	{
		return -1;
	}
	pool->shutdown = 1;

	/*销毁管理者线程*/
	pthread_join(pool->admin_tid, NULL);

	for( i=0;i<pool->live_thr_num;i++ )
	{
		pthread_cond_broadcast(&(pool->queue_not_empty));
	}

	for( i=0;i<pool->max_thr_num;i++ )
	{
		pthread_join(pool->threads[i],NULL);
	}
	threadpool_free(pool);
	return 0;
}

/*int main()
{
	threadpool_create(20,100,150);
	printf("threadpool init ... ...\n");
}*/

#include "unix.h"

/*任务*/
typedef struct{
	void *(*function)(void *);
	void *arg;
}threadpool_task_t;

/*线程池管理*/
typdef struct{
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
}threadpool_t;


/*创建线程池 参数：最小线程数，最大线程数，任务队列最大值*/
threadpool_t * threadpool_create(int min_thr_num,int max_thr_num,int queue_max_size)
{
	int i;
	threadpool_t *pool = NULL;
	do
	{
		if( (pool = (threadpool_t *)malloc(sizeof(threadpool_t))) == NULL )
		{
			pirntf("malloc threadpool false;\n");
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
		pool->shutdown = false;

		/*为线程数组开辟空间*/
		pool->threads = (pthread_t *)malloc(sizeof(pthread_t)*max_thr_num);
		if( pool->threads == NULL )
		{
			printf("malloc threads false\n");
			break;
		}
		memset( pool->threads,0,sizeof(phread_t)*max_thr_num );

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


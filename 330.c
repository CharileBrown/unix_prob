#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct job{
	struct job *j_next;
	struct job *j_prev;
	pthread_t j_id;
};

struct queue{
	struct job *j_head;
	struct job *j_tail;
	pthread_rwlock_t q_lock;
};

int queue_init(struct queue *qp){
	int err;
	qp->j_head = NULL;
	qp->j_tail = NULL;
	err = pthread_rwlock_init(&qp->lock,NULL);
	if( err!=0 )
		return err;
	return 0;
}
void job_insert(struct queue *qp,struct queue *jp)
{
	pthread_rwlock_wrlock(&qp->q_lock);
	jp->j_next = qp->head;
	jp->prev = NULL;
	if(qp->head!=NULL)
		qp->head->prev = jp;
	else
		qp->tail = jp;
	qp->head = jp;
	pthread_rwlock_unlock(&qp->q_lock);
}


#include "common.h"

struct msg_form
{
	long mtype;
	char mtext;
};

union semun
{
	int val;
	struct semid_ds *buf;
	unsigned short *array;
};

int init_sem(int sem_id,int value)
{
	union semun tmp;
	tmp.val = value;
	if( semctl(sem_id,0,SETVAL,tmp)==-1 )
	{
		perror("Init Semaphore Error");
		return -1;
	}
	return 0;
}

int sem_p(int sem_id)
{
	struct sembuf sbuf;
	sbuf.sem_num = 0;
	sbuf.sem_op = -1;
	sbuf.sem_flg = SEM_UNDO;
	if( semop(sem_id,&sbuf,1) == -1 )
	{
		perror("Semaphore Action P Error\n");
		return -1;
	}
	return 0;
}

int sem_v(int sem_id)
{
	struct sembuf sbuf;
	sbuf.sem_num = 0;
	sbuf.sem_op = 1;
	sbuf.sem_flg = SEM_UNDO;
	if( semop(sem_id,&sbuf,1) == -1 )
	{
		perror("Semaphore Action V Error\n");
		return -1;
	}	
	return 0;
}

int del_sem(int sem_id)
{
	union semun tmp;
	if(semctl(sem_id,0,IPC_RMID,tmp)==-1)
	{
		perror("Delete Semaphore Error");
		return -1;
	}
	return 0;
}

int creat_sem(key_t key)
{
	int sem_id;
	if( (sem_id = semget(key,1,IPC_CREAT|0666)) == -1 )
	{
		perror("semget error");
		exit(-1);
	}
	init_sem(sem_id,1);
	return sem_id;
}

int main()
{
	key_t key;
	int shmid,semid,msqid;
	char *shm;
	char data[] = "this is client";
	struct msg_form msg;
	if( (key=ftok("./test.txt",256)) == -1 )
	{
		perror("Ftok Error");
		exit(1);
	}
	if((shmid = shmget(key,0,IPC_CREAT|0666))==-1)
	{
		perror("Shmget Error");
		exit(1);
	}
	shm = shmat(shmid,0,0);
	if((long)shm == -1)
	{
		perror("Shmat Error");
		exit(1);
	}	
	if((semid = semget(key,1,IPC_CREAT|0666))==-1)
	{
		perror("Semget Error");
		exit(1);
	}
	if((msqid = msgget(key,IPC_CREAT|0777)) == -1)
	{
		perror("Msgget error");
		exit(1);
	}
	sem_p(semid);
	scanf("%[^\n]",shm);
	sem_v(semid);
	msg.mtype = 888;
	msg.mtext = 'r';
	msgsnd(msqid,&msg,1,0);
	shmdt(shm);
}

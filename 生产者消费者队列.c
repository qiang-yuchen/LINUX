#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

#define BUFF 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

typedef struct
{
	int data[BUFF];
	int front;
	int rear;
	int count;
}Queue;

Queue queue;

int piao_id = 0;

void init_queue(Queue* q)
{
	q->front = 0;
	q->rear = 0;
	q->count = 0;
}

int is_full(Queue* q)
{
	return q->count == BUFF;
}

int is_empty(Queue* q)
{
	return q->count == 0;
}

int enqueue(Queue* q,int val)
{
	if(is_full(q))
	{
		return 0;
	}
	q->data[q->rear]=val;
	q->rear=(q->rear+1)%BUFF;
	q->count++;
	return 1;
}

int dequeue(Queue* q,int* val)
{
	if(is_empty(q))
	{
		return 0;
	}
	*val=q->data[q->front];
	q->front=(q->front+1)%BUFF;
	q->count--;
	return 1;
}
void* produce(void* arg)
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(is_full(&queue))
		{
			pthread_cond_wait(&not_full,&mutex);
		}
		piao_id++;
		if(enqueue(&queue,piao_id))
		{
			printf("生产票ID=%d, 队列中有%d张票\n",piao_id,queue.count);
		}
		pthread_cond_signal(&not_empty);
		pthread_mutex_unlock(&mutex);
		usleep(100000);
	}
	return NULL;
}

void* consume(void* arg)
{
	char* name=(char*)arg;
	while(1)
	{
		pthread_mutex_lock(&mutex);
		while(is_empty(&queue))
		{
			pthread_cond_wait(&not_empty,&mutex);
		}
		int get_piao;
		if(dequeue(&queue,&get_piao))
		{
			printf("%s消费票ID=%d, 队列中剩余%d张票\n", name, get_piao, queue.count);
		}
		pthread_cond_signal(&not_full);
		pthread_mutex_unlock(&mutex);
		usleep(500000);
	}
	return NULL;
}

int main()
{
	init_queue(&queue);
	pthread_t p;
	pthread_create(&p,NULL,produce,NULL);
	pthread_t c1,c2;
	pthread_create(&c1,NULL,consume,"JATA");
	pthread_create(&c2,NULL,consume,"JICK");
	pthread_join(p, NULL);
       	pthread_join(c1, NULL);
	pthread_join(c2, NULL);
	return 0;
}

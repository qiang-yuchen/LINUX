#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<stdlib.h>

#define BUFF 10

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

int queue_data[BUFF];
int* front_ptr;
int* rear_ptr;
int queue_count;

int piao_id = 0;

void init_queue()
{
    front_ptr = &queue_data[0];
    rear_ptr = &queue_data[0];
    queue_count = 0;
}

int is_full()
{
    return queue_count == BUFF;
}

int is_empty()
{
    return queue_count == 0;
}

int enqueue(int val)
{
    if(is_full())
    {
        return 0;
    }
    
    *rear_ptr = val;
    rear_ptr++;
    if(rear_ptr >= &queue_data[BUFF])
    {
        rear_ptr = &queue_data[0];
    }
    
    queue_count++;
    return 1;
}

int dequeue(int* val)
{
    if(is_empty())
    {
        return 0;
    }
    *val = *front_ptr;
    front_ptr++;
    if(front_ptr >= &queue_data[BUFF])
    {
        front_ptr = &queue_data[0];
    }
    
    queue_count--;
    return 1;
}

int get_queue_count()
{
    return queue_count;
}

void* produce(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(is_full())
        {
            pthread_cond_wait(&not_full, &mutex);
        }
        piao_id++;
        if(enqueue(piao_id))
        {
            printf("生产票ID=%d, 队列中有%d张票\n", piao_id, get_queue_count());
        }
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
    return NULL;
}

void* consume(void* arg)
{
    char* name = (char*)arg;
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(is_empty())
        {
            pthread_cond_wait(&not_empty, &mutex);
        }
        int get_piao;
        if(dequeue(&get_piao))
        {
            printf("%s消费票ID=%d, 队列中剩余%d张票\n", name, get_piao, get_queue_count());
        }
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
        usleep(500000);
    }
    return NULL;
}

int main()
{
    init_queue();
    pthread_t p;
    pthread_create(&p, NULL, produce, NULL);
    pthread_t c1, c2;
    pthread_create(&c1, NULL, consume, "JATA");
    pthread_create(&c2, NULL, consume, "JICK");
    pthread_join(p, NULL);
    pthread_join(c1, NULL);
    pthread_join(c2, NULL);   
    return 0;
}

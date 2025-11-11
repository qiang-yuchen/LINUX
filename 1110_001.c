#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>

void* thread_func(void* args)
{
	printf("hello\n");
	exit(0);
}
int main()
{
	pthread_t thread_id;
	pthread_create(&thread_id,NULL,thread_func,NULL);
	sleep(1);
	return 0;
}

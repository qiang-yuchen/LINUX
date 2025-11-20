#include<stdio.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define COUNT 3
#define BUFF 8192
typedef struct
{
	char* src;
	char* det;
	off_t start;
	off_t end;
	int id;
}thread_arg;

void* segment_copy(void* arg)
{
	thread_arg* t_arg=(thread_arg*)arg;
	int src_fd=open(t_arg->src,O_RDONLY);
	int det_fd=open(t_arg->det,O_WRONLY);
	lseek(src_fd,t_arg->start,SEEK_SET);
	lseek(det_fd,t_arg->start,SEEK_SET);
	off_t cur_start=t_arg->start;
	char buff[BUFF];
	while(cur_start=t_arg->end)
	{
		off_t real=BUFF;
		if(cur_start+BUFF>t_arg->end)
		{
			real=t_arg->end-cur_start;
		}
		ssize_t read_count;
		read_count=read(src_fd,buff,real);
		if(read_count==0)
		{
			break;
		}
		write(det_fd,buff,read_count);
		cur_start+=real;
	}
}
int main()
{
	char* src="file1";
	char* det="file2";
	int src_fd=open(src,O_RDONLY);
	struct stat st;
	fstat(src_fd,&st);
	off_t file_size=st.st_size;
	thread_arg args[COUNT];
	pthread_t thread_ids[COUNT];
	off_t segment_size=file_size/COUNT;
	for(int i=0;i<COUNT;i++)
	{
		args[i].src=src;
		args[i].det=det;
		args[i].start=i*segment_size;
		args[i].end=(i+1)==COUNT?file_size:(i+1)*segment_size;
		args[i].id=i;
	}
	for(int i=0;i<COUNT;i++)
	{
		pthread_create(&thread_ids[i],NULL,segment_copy,&args[i]);
	}
	for(int i=0;i<COUNT;i++)
	{
		pthread_join(thread_ids[i],NULL);
	}
	printf("copy YES\n");
}

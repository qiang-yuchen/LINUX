#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main()
{
	int child = 3;
	for(int i = 0;i < 3;i++)
	{
		if(fork()==0)
		{
			printf("子进程%dPID:%d工作中\n",i,getpid());
			sleep(2+i);
			printf("子进程%dPID:%d结束\n",i,getpid());
			exit(i);
		}
	}

	printf("父进程PID:%d,等待子进程\n",getpid());

	while(child>0)
	{
		int s;
		pid_t pid = waitpid(-1,&s,WNOHANG);

		if(pid>0)
		{
			printf("PID为%d的子进程被父进程回收;退出码为:%d\n",pid, WEXITSTATUS(s));
			child--;
		}
		else if(pid==0)
		{
			printf("还有%d个子进程未结束工作\n",child);
			sleep(1);
		}
		else if(pid<0)
		{
			perror("ERROR\n");
		        break;
		}
	}
	
	printf("所有子进程都已经结束工作\n");
	return 0;
}

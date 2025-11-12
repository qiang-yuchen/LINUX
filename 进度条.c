#include<stdio.h>
#include<unistd.h>
#include<string.h>

int main()
{
	int i=0;
	char arr[102];
	const char* label="|/-\\";
	memset(arr,0,sizeof(arr));
	for(i=0;i<=100;i++)
	{
		printf("[%-100][%d%%][%c]\r",arr,i,label[i%4]);
		fflush(stdout);
		arr[i]='=';
		if(i<100)
		{
			arr[i+1]='>';
		}
		usleep(100000);
	}
	printf("\n");
	return 0;
}

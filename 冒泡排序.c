#include<stdio.h>

void maopao(int arr[],int n)
{
	int i,j,temp;
	for(i=0;i<n-1;i++)
	{
		for(j=0;j<n-i-1;j++)
		{
			if(arr[j]>arr[j+1])
			{
				temp=arr[j];
				arr[j]=arr[j+1];
				arr[j+1]=temp;
			}
		}
	}
}

void printfA(int arr[],int size)
{
	int i;
	for(i=0;i<size;i++)
	{
		printf("%d->",arr[i]);
	}
	printf("\n");
}

int main()
{
	int arr[]={10,7,87,23,1,9,36};
	int n=sizeof(arr)/sizeof(arr[0]);
	printf("排序前的数组:");
	printfA(arr,n);
	maopao(arr,n);
	printf("排序后的数组:");
	printfA(arr,n);
	return 0;
}

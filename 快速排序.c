#include<stdio.h>

void swap(int* a,int*b)
{
	int temp=*a;
	*a=*b;
	*b=temp;
}

int partition(int arr[],int low,int high)
{
	int piv=arr[high];
	int i=(low-1);
	for(int j=low;j<=high-1;j++)
	{
		if(arr[j]<=piv)
		{
			i++;
			swap(&arr[i],&arr[j]);
		}
	}
	swap(&arr[i+1],&arr[high]);
	return (i+1);
}

void quickA(int arr[],int low,int high)
{
	if(low<high)
	{
		int pi=partition(arr,low,high);
		quickA(arr,low,pi-1);
		quickA(arr,pi+1,high);
	}
}

void printfA(int arr[],int size)
{
	for(int i=0;i<size;i++)
	{
		printf("%d ",arr[i]);
	}
	printf("\n");
}

int main()
{
	int arr[]={6,1,23,78,54,2,4,9,3};
	int n=sizeof(arr)/sizeof(arr[0]);

	printf("原始数组:");
	printfA(arr,n);

	quickA(arr,0,n-1);

	printf("排序后数组:");
	printfA(arr,n);

	return 0;
}

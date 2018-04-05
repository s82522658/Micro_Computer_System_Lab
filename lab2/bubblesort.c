#include<stdio.h>

void swap(int data[],int j){
	int temp=data[j];
	data[j]=data[j+1];
	data[j+1]=temp;
}

void bubblesort(int data[]){
	for(int i=0;i<7;i++){
		for(int j=0;j<7-i;j++){
			if(data[j]>data[j+1]) swap(data,j);
		}
	}

	
}

int main(){
	int data[]={19,34,14,32,52,23,61,29};
	bubblesort(data);
	printf("after sorting:");
	for(int i=0;i<8;i++) printf("%d ",data[i]);
	return 0;
}
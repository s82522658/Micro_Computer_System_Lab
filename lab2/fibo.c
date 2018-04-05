#include<stdio.h>

//#define N 50

int main(){
	int n;
	scanf("%d",&n);
	int rlt[3]={1,1,0};
	if(n<1||n>100){
		printf("out of range!!\n");
		return -1;
	}
	else {
		int chk=0;
		
		for(int i=1;i<=n-2;i++){
			rlt[2]=rlt[0]+rlt[1];
			if(rlt[2]<rlt[1]) {
				printf("result overflow\n");
				return -2;
			}
			else{
				rlt[0]=rlt[1];
				rlt[1]=rlt[2];
			}		
		}
		printf("result is :%d\n",rlt[2] );
	}

	
	return 0;
}
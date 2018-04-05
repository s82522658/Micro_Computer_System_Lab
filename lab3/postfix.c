#include<stdio.h>

#define len 25

void atoi(char input[],int data[]){
	//int data[len];
	int cnt=0;
	for(int i=0;i<len;i++){
		if(input[i]!=' '){
			data[cnt]=input[i]-'0';
			cnt++;
		} 	
	}
}

int main(){
	char input[len]="-100 10 20 + - 10 +";
	int data[len]={};
	int stack[len]={};
	int cnt=0;
	int ans=0;
	
	atoi(input,data);

	for(int i=0;i<len;i++){
		if(1<=data[i] && data[i]<=9){
			printf("data is:%d,cnt is:%d,i is:%d\n",data[i],cnt,i);
			stack[cnt]=data[i];
			cnt++;

		}
		else if(data[i] == 0) break;
		else{
			if((data[i]+'0')=='+') ans=stack[cnt-2]+stack[cnt-1];
			else if((data[i]+'0')=='-') ans=stack[cnt-2]-stack[cnt-1];
			else if((data[i]+'0')=='*') ans=stack[cnt-2]*stack[cnt-1];
			else if((data[i]+'0')=='/') ans=stack[cnt-2]/stack[cnt-1];
			stack[cnt-2]=ans;
			cnt--;
			printf("ans is:%d,cnt is:%d\n",ans,cnt);
		}
	}

	printf("final ans is:%d\n",stack[0]);

}
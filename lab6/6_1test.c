#include<stdio.h>
#include<stdlib.h>

//These functions inside the asm file
extern void GPIO_init();
extern void max7219_init();
extern void MAX7219Send(unsigned char address, unsigned char data);

/**
* TODO: Show data on 7-seg via max7219_send
* Input:
* data: decimal value
* num_digs: number of digits will show on 7-seg * Return:
* 0: success
* -1: illegal data range(out of 8 digits range) */
int display_id(int* data, int num_digs)
{
   if(num_digs > 7) return -1;
   for(int i=0;i<num_digs;i++){
      MAX7219Send(num_digs-i,data[i]);
   }
   return 0;   
}
int main()
{
   int student_id[7] = {0,4,1,6,3,1,4};
   GPIO_init();
   max7219_init();
   int check=display_id(student_id, 7);
  // if(check == -1) printf("error!\n");
   return 0;
}
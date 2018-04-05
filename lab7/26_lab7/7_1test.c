#include<stdio.h>
#include<stdlib.h>
#include "stm32l476xx.h"

extern void GPIO_init();
extern void MHz_delay_1s();
extern int User_button();
void SystemClock_Config(){
    //TODO: Change the SYSCLK source and set the correspondingPrescaler value.
	RCC->CR = (RCC->CR & ~RCC_CR_MSIRANGE) | RCC_CR_MSIRANGE_7 | RCC_CR_MSIRGSEL;// MSI選8MHz時鐘
	while((RCC->CR & RCC_CR_MSIRDY) == 0);//check MSI ready
	//MSI=8,PLLN=8,PLLM=8,PLLN=8  clock rate=MSI*PLLN/(PLLM*PLLR)=1MHz
	RCC->PLLCFGR = RCC_PLLCFGR_PLLREN | RCC_PLLCFGR_PLLR_0| RCC_PLLCFGR_PLLR_1| RCC_PLLCFGR_PLLN_3 | RCC_PLLCFGR_PLLM_0|
			RCC_PLLCFGR_PLLM_1| RCC_PLLCFGR_PLLM_2 | RCC_PLLCFGR_PLLSRC_MSI;
	RCC->CR |= RCC_CR_PLLON;
	while  ((RCC->CR & RCC_CR_PLLRDY) == 0);  //等待PLL穩定

	//指定PLL為sysclk
	RCC->CFGR |= RCC_CFGR_SW;
	while  ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS);

	return;
}
int main(){
  SystemClock_Config();
  GPIO_init();
  int cnt=0;
  while(1){
	if (User_button())
	{
		cnt=(cnt+1)%5;
		//TODO: Update system clock rate
		//RCC->CR =(0<<24);
		RCC->CFGR &= ~RCC_CFGR_SW;
		while  ((RCC->CFGR & RCC_CFGR_SWS) == RCC_CFGR_SWS);

		RCC->CR &= ~RCC_CR_PLLON;
		while  ((RCC->CR & RCC_CR_PLLRDY) != 0);  //等待PLL穩定
		switch(cnt){
		case 0://1
			RCC->PLLCFGR=0x07000871;
			break;
		case 1://6
			RCC->PLLCFGR=0x07003071;
			break;
		case 2://10
			RCC->PLLCFGR=0x07005071;
			break;
		case 3://16
			RCC->PLLCFGR=0x07005041;
			break;
		case 4://40
			RCC->PLLCFGR=0x07005011;
			break;
		}
		RCC->CR |= RCC_CR_PLLON;
		while  ((RCC->CR & RCC_CR_PLLRDY) == 0);  //等待PLL穩定

		//指定PLL為sysclk
		RCC->CFGR |= RCC_CFGR_SW;
		while  ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS);
	}
	GPIOA->BSRR = (1<<5);
	MHz_delay_1s ();
	GPIOA->BRR = (1<<5);
	MHz_delay_1s ();
	}
}

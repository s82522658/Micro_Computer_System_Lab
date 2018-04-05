#include "stm32l476xx.h"
#include<stdio.h>

#define do 40000/261
#define re 40000/293
#define mi 40000/329
#define fa 40000/349
#define so 40000/392
#define la 40000/440
#define si 40000/493
#define hdo 40000/523


static inline void GPIO_init(){
	asm("	push {r0-r11,lr}\r\n"
			"movs 	r0, #0x7\r\n"
	"ldr 	r1, =RCC_AHB2ENR\r\n"
	"str 	r0,	[r1]\r\n"

	//set PA5,PA6,PA7 as output mode PA1
	"ldr	r0,	=#0x5404\r\n"
	"ldr		r1, =GPIOA_MODER\r\n"
	"ldr		r2,	[r1]\r\n"
	"ldr		r5,=#0xFFFF57F7\r\n"
	"and		r2, r5\r\n"
	"orrs	r2,	r2,	r0\r\n"
	"str		r2,	[r1]\r\n"
	//set PA5,PA6,PA7 as high speed mode
	"LDR	r0,	=#0xA808\r\n"
	"ldr		r1,	=GPIOA_OSPEEDR\r\n"
	"strh	r0, [r1]\r\n"
	//1011 1111//pb 3 Alternate function mode

	"pop {r0-r11,pc}\r\n"
	"BX LR\r\n");
}
// extern int User_button();
// extern void Delay_sec();

static inline void Delay_sec(){
	asm("push {r0}\r\n"
			"mov r0, %0\r\n"
			"LOOP:\r\n"
			"subs r0, #1\r\n"
			"BGT LOOP\r\n"
			"POP {r0}\r\n"
			:: "r" (1*1333*1000));
}

void GPIO_init_AF(){
//TODO: Initial GPIO pin as alternate function for buzzer. You can choose to use C or assembly to finish this function.
   GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL15;//AFR[0] LOW
   GPIOA->AFR[1] |= 0001<<28;//PB3 Alternate function mode
  //GPIO_SetBits(GPIOB,3);
  //GPIO_ResetBits(GPIOB,3);
}
void Timer_init(uint32_t presc){
   //TODO: Initialize timer
   RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
   TIM2->CR1 &= ~(TIM_CR1_DIR | TIM_CR1_CMS);//clear CMS and DIR to 0(edge-align mode,up counter)
   //TIM2->CR1 &= ~(TIM_CR1_DIR);
   //TIM2->CR1 &= ~(TIM_CR1_CMS);
   //SET_REG(TIM2->CR1, TIM_CR1_DIR | TIM_CR1_CMS, TIM_COUNTERMODE_DOWN);// Edge-aligned mode, down counter
   TIM2->ARR = (uint32_t)100;//Reload value
   TIM2->PSC = (uint32_t)presc;//Prescaler
   TIM2->EGR = TIM_EGR_UG;//Reinitialize the counter
   TIM2->CR1 |= TIM_CR1_CEN;
}
void PWM_channel_init(){
   //TODO: Initialize timer PWM channel
   /*TIM2->CCMR1 &= ~TIM_CCMR1_OC2M;
   TIM2->CCMR1 |= (110 << 12);//set mode1

   TIM2->CCMR1 &= ~TIM_CCMR1_OC2PE;//OCxPE
   TIM2->CCMR1 |= (0b1 << 11);//enable preload value

   TIM2->CCR2 = 50;//set duty cycle
   TIM2->CCER |= TIM_CCER_CC2E;//ebable tim2's channel 2*/
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
   TIM2->CCMR1 |= (110 << 4);//set mode1

   TIM2->CCMR1 &= ~TIM_CCMR1_OC1PE;//OCxPE
   TIM2->CCMR1 |= (0b1 << 3);//enable preload value

   TIM2->CCR1 = 50;//set duty cycle
   TIM2->CCER |= TIM_CCER_CC1E;//ebable tim2's channel 2

}

void skycastle(){
  Timer_init(la);
  Delay_sec();
  Timer_init(si);
  Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Delay_sec();
  Delay_sec();
  Timer_init(si);
  Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Delay_sec();
  Timer_init(mi/2);
  Delay_sec();
  Delay_sec();
  Timer_init(si);
  Delay_sec();
  Delay_sec();
  Delay_sec();
  Delay_sec();
  Timer_init(30000);
  Delay_sec();
  Delay_sec();
  Timer_init(mi);
  Delay_sec();
  Delay_sec();
  Timer_init(la);
  Delay_sec();
  Delay_sec();
  Delay_sec();
  Timer_init(so);
  Delay_sec();
  Timer_init(la);
  Delay_sec();
  Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Delay_sec();
  Timer_init(so);
  Delay_sec();
  Delay_sec();
  Delay_sec();
  Delay_sec();
  Timer_init(30000);
}
void bubu(){
  Timer_init(mi);
  Delay_sec();
  //Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Delay_sec();
  Timer_init(la);
  Delay_sec();
  Delay_sec();
  Timer_init(mi/2);
  Delay_sec();
  Delay_sec();
  Timer_init(re/2);
  Delay_sec();
  //Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Delay_sec();
  Timer_init(re/2);
  Delay_sec();
  Delay_sec();
  /*2*/
  Timer_init(mi);
  Delay_sec();
  //Delay_sec();
  Timer_init(so);
  Delay_sec();
  Delay_sec();
  Timer_init(so);
  Delay_sec();
  Timer_init(so);
  Delay_sec();
  Delay_sec();
  Timer_init(mi);
  Delay_sec();
  Delay_sec();
  Timer_init(re/2);
   Delay_sec();
   Delay_sec();
   Timer_init(mi/2);
   Delay_sec();
   //Delay_sec();
   Timer_init(re/2);
   Delay_sec();
   Delay_sec();
   Timer_init(do/2);
   Delay_sec();
   Delay_sec();
  /*3*/
  Timer_init(mi/2);
  Delay_sec();
  Timer_init(fa/2);
  Delay_sec();
  Delay_sec();
  Timer_init(mi/2);
  Delay_sec();
  Timer_init(mi/2);
  Delay_sec();
  Delay_sec();
  Timer_init(re/2);
  Delay_sec();
  Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Delay_sec();
  Timer_init(si);
  Delay_sec();
  //Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Delay_sec();
  Timer_init(re/2);
  Delay_sec();
  Delay_sec();
  Delay_sec();
  /*4*/
  Timer_init(mi/2);
  Delay_sec();
  Delay_sec();
  Timer_init(re/2);
  Delay_sec();
  Timer_init(re/2);
  Delay_sec();
  Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Delay_sec();
  Timer_init(do/2);
  Delay_sec();
  Delay_sec();
  Delay_sec();
  Timer_init(30000);
}

int main(){
  GPIO_init();
  GPIO_init_AF();
  Timer_init(30000);//(40000/261);
  PWM_channel_init();
  //while(!User_button());
  //skycastle();
  bubu();


}

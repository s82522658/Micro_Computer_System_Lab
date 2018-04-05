
#include "buzzer.h"

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
   TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
   TIM2->CCMR1 |= (110 << 4);//set mode1

   TIM2->CCMR1 &= ~TIM_CCMR1_OC1PE;//OCxPE
   TIM2->CCMR1 |= (0b1 << 3);//enable preload value

   TIM2->CCR1 = 50;//set duty cycle
   TIM2->CCER |= TIM_CCER_CC1E;//ebable tim2's channel 1
}

void buzz_init_AF() {
  // GPIO_init_AF
  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL15;//AFR[0] LOW
  GPIOA->AFR[1] |= 0001<<28;
}

static inline void Delay_sec(){
	asm("push {r0}\r\n"
			"mov r0, %0\r\n"
			"LOOP:\r\n"
			"subs r0, #1\r\n"
			"BGT LOOP\r\n"
			"POP {r0}\r\n"
			:: "r" (1*1333*250));
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

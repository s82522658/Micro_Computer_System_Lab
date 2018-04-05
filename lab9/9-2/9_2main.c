#include "stm32l476xx.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#define space  " "
#define line  "\n"
extern void delay_us(int a);

int adc_value = 0;

void init_GPIO(){
	RCC->AHB2ENR |= 7;//Enable Port A B C
	GPIOA->MODER = 0xABEBFFFF;
	GPIOA->AFR[1] = 0x770; // AF7 for pin

	//Set PC13 INPUT mode & PC0 analog mode
	GPIOC->MODER &= 0xF3FFFFFF;
	GPIOC->MODER |= 0x3;
	GPIOC->ASCR = 0x1;
}

int UART_Transmit(uint8_t *arr, uint32_t size) {
	//TODO: Send str to UART and return how many bytes are successfully transmitted.
	for (int i = 0; i < size; i++) {
			while (!(USART1->ISR & (1<<7)) );//READY
			USART1->TDR = arr[i];
		}
		while (!(USART1->ISR & (1<<6)));
		return 1;
}

void USART1_Init(void) {
	/* Enable clock for USART??? */
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// CR1
	USART1->CR1 &= 0x0;
	USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	//MODIFY_REG(USART1->CR1, USART_CR1_M | USART_CR1_PS | USART_CR1_PCE | USART_CR1_TE | USART_CR1_RE |USART_CR1_OVER8, USART_CR1_TE | USART_CR1_RE);
	// CR2
	USART1->CR2 &= 0x0;
	USART1->CR2 |= (0x00 << 12);
	//MODIFY_REG(USART1->CR2, USART_CR2_STOP, 0x0); // 1-bit stop
	// CR3
	USART1->CR3 &= 0x0;
	USART1->CR3 |= (0x0<<11);
	//MODIFY_REG(USART1->CR3, (USART_CR3_RTSE | USART_CR3_CTSE | USART_CR3_ONEBIT), 0x0); // none hwflowctl
	USART1->BRR &= 0x0;
	USART1->BRR |= 417L;//9600L;//4000000L/417L;
	//MODIFY_REG(USART1->BRR, 0xFF, 4000000L/???? L);
	/* In asynchronous mode, the following bits must be kept cleared:
	- LINEN and CLKEN bits in the USART_CR2 register,
	- SCEN, HDSEL and IREN bits in the USART_CR3 register.*/
	USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
	USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);
	// Enable UART
	USART1->CR1 |= (USART_CR1_UE);
}


void configureADC(){
	// TODO
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
		ADC1->CR &= ~ADC_CR_DEEPPWD;// Clear Deep Sleep
		//CLEAR_BIT(ADC1->CR, ADC_CR_DEEPPWD);

		ADC1->CR |= ADC_CR_ADVREGEN;//SET_BIT(ADC1->CR, ADC_CR_ADVREGEN);
		// Turn on Voltage Regulator
		delay_us(200);
		// Prescaler
		ADC123_COMMON->CCR &= ~(ADC_CCR_PRESC|ADC_CCR_CKMODE);
		ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
		//MODIFY_REG(ADC123_COMMON->CCR, ADC_CCR_PRESC|ADC_CCR_CKMODE, ADC_CCR_CKMODE_0);

		//ADC1->CFGR &= ~(ADC_CFGR_FIELDS_1);
		ADC1->CFGR |= ADC_CFGR_CONT;
		//MODIFY_REG(ADC1->CFGR, ADC_CFGR_FIELDS_1, ADC_CFGR_CONT);

		ADC1->CFGR2 &= ~ ADC_CFGR2_ROVSE;
		//CLEAR_BIT(ADC1->CFGR2, ADC_CFGR2_ROVSE);
		ADC1->SQR1 &= ~ADC_SQR1_L;
		//CLEAR_BIT(ADC1->SQR1, ADC_SQR1_L);

		ADC1->SQR1 &= ~((0xF)|(0x1F<<24)|(0x1F<<18)|(0x1F<<12)|(0x1F<<6));
		ADC1->SQR1 |= (0x1<<6);// Channel 1, Rank 1
		//MODIFY_REG(ADC1->SQR1, (0xF)|(0x1F<<24)|(0x1F<<18)|(0x1F<<12)|(0x1F<<6), (0x1<<6)); // Channel 1, Rank 1

		ADC1->SMPR1 &= ~ (0x3FFFFFFFFF);
		ADC1->SMPR1 |=  (0x6<<3);// Channel 1, Sampling Time: 247.5 ADC cycles
		//MODIFY_REG(ADC1->SMPR1, (0x3FFFFFFFFF), (0x6<<3)); // Channel 1, Sampling Time: 247.5 ADC cyc

		ADC1->IER = (1<<ADC_IER_ADRDYIE);// ADC ready interrupt enable
		NVIC_SetPriority(ADC1_2_IRQn, 1);
		NVIC_EnableIRQ(ADC1_2_IRQn);

}

void startADC() {
	while (!(ADC1->ISR & ADC_ISR_ADRDY)) ADC1->CR |= ADC_CR_ADEN; // TURN ON
		delay_us(5000);
		ADC1->ISR = ADC_ISR_EOC | ADC_ISR_EOS | ADC_ISR_OVR; // Clear flags
		//interrupt and status register

		ADC1->CR |=  ADC_CR_ADSTART;// START CONV
		//ADC control register |= ADC start of regular conversion
		//SET_BIT(ADC1->CR, ADC_CR_ADSTART);

}

void  ADC1_2_IRQHandler(void) {
	adc_value = ADC1->DR;//ADC regular Data Register
	startADC();
}

void EXTI_Setup(){
//TODO: Setup EXTI interrupt
////PC13
	RCC->APB2ENR |= 0x1;
	SYSCFG->EXTICR[3] = 0x20;

	EXTI->RTSR1 |= (1<<13);
	EXTI->IMR1 |= (1<<13);

	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI15_10_IRQHandler(void){
	char temp[100];
	if(EXTI->PR1 & (1<<13)){
		EXTI->PR1 |= (1<<13);

		UART_Transmit("\r", 1);//aline
		itoa(adc_value, temp, 10);
		UART_Transmit(temp, strlen(temp));
		UART_Transmit(line, strlen(line));
		//UART_Transmit("Hello World", 11);
	}
}

int main(){
	init_GPIO();
	USART1_Init();
	configureADC();
	EXTI_Setup();
	startADC();
	while(1){
	}
}

#include "gpio.h"
#include "lcd.h"

#include "stm32l476xx.h"
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#define space  " "
#define line  "\n"


char str[] = "26 test";
LCDdef set_lcd;

void init_GPIO(){
	RCC->AHB2ENR |= 7;//Enable Port A B C
	GPIOA->MODER = 0xABEBFFFF;
	GPIOA->AFR[1] = 0x770; // AF7 for pin
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

void lcd_init()
{
	set_lcd.CMD_GPIO_port = C;
	set_lcd.Data_GPIO_port = B;
  // set_lcd.PIN_Lightness = GPIO_Pin_0;
	set_lcd.PIN_RS = GPIO_Pin_0;
	set_lcd.PIN_RW = GPIO_Pin_1;
	set_lcd.PIN_EN = GPIO_Pin_2;
	set_lcd.PIN_DB_0 = GPIO_Pin_0;
  set_lcd.PIN_DB_1 = GPIO_Pin_1;
  set_lcd.PIN_DB_2 = GPIO_Pin_2;
  set_lcd.PIN_DB_3 = GPIO_Pin_3;
  set_lcd.PIN_DB_4 = GPIO_Pin_4;
  set_lcd.PIN_DB_5 = GPIO_Pin_5;
  set_lcd.PIN_DB_6 = GPIO_Pin_6;
  set_lcd.PIN_DB_7 = GPIO_Pin_7;
	LCDinit(&set_lcd);
}

void display_init()
{
	int ready, addr;
	// readLCM(&set_lcd, &ready, &addr);

	setFunction(&set_lcd, Bit_8, DOUBLE_ROW, Font_5_7);
	enterMode(&set_lcd, CURSOR, RIGHT);
	resetCursor(&set_lcd);
	turnOnScreen(&set_lcd, ON, ON, BLINK_ON);
	clearScreen(&set_lcd);

}

int main()
{
	init_GPIO();
	USART1_Init();
	lcd_init();
	display_init();

	displayStr(&set_lcd, str);

	UART_Transmit("\r", 1);//aline
	UART_Transmit("Hello!", 6);//aline

   return 0;
}

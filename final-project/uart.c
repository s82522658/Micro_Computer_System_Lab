/**
  ******************************************************************************
  * @file    lcd.h
  * @author  cysun
  * @date    2017-11-21
  ******************************************************************************
***/

#include "uart.h"

void uart_gpio_init(UARTdef* uart_def)
{
  GPIO_InitTypeDef gpio_set;
  gpio_set.port = uart_def->GPIO_port;
  gpio_set.GPIO_Pin = uart_def->rx_pin + uart_def->tx_pin;
  gpio_set.GPIO_Speed = Low_Speed;
  gpio_set.GPIO_Mode = Alternate_Mode;
  gpio_set.GPIO_PULL = No_Pull;
  GPIO_initialize(gpio_set);
  GPIOA->AFR[1] = 0x770; // AF7 for pin
}

void uart1_init() {
  // enable uart1 clk
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  /* last enable uart, since many cmd can write when disable */

  // CR1 (Control register 1)
	USART1->CR1 &= 0x0;
  // Transmitter enable, Receiver enable
	USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);

  // CR2 (Control register 2)
  USART1->CR2 &= 0x0;
	USART1->CR2 |= (0x00 << 12); // STOP bits (0 = 1 bit)

  // CR3 (Control register 3)
  USART1->CR3 &= 0x0;
	USART1->CR3 |= (0x0<<11); // One sample bit method enable (0 = 3 bit)

  // BRR (Baud rate register)
  USART1->BRR &= 0x0;
	USART1->BRR |= 417L; //9600L;//4000000L/417L;

  // The LIN mode enables the capability to send LIN Sync Breaks (13 low bits)
  // using the SBKRQ bit in the USART_RQR register, and to detect LIN Sync breaks
  USART1->CR2 &= ~(USART_CR2_LINEN | USART_CR2_CLKEN);
  // Smartcard mode enable, Half-duplex selection, IrDA mode enable
	USART1->CR3 &= ~(USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN);

  // Enable UART
	USART1->CR1 |= (USART_CR1_UE);
}

int UART_Transmit(uint8_t *arr, uint32_t size) {
	//TODO: Send str to UART and return how many bytes are successfully transmitted.
	for (int i = 0; i < size; i++) {
			while (!(USART1->ISR & (1<<7)) );//READY
			USART1->TDR = arr[i];
		}
//	while (!(USART1->ISR & (1<<7)) ); //END of str
//	USART1->TDR = 0;
		while (!(USART1->ISR & (1<<6)));
		return 1;
}

char UART_Receive() {
  // RXNE (Read data register not empty)
  // char tmp = USART1->RDR;
  USART1->RQR |= 0b1000;
  int t = 0;
  while (!(USART1->ISR & 0b100000)) {
    if(t>50000)
      return -1;
    t++;
  }
  return USART1->RDR;
}

void UART_Receive_Str(char* str, int len) {
  for (int i = 0; i < len; i++) {
    str[i] = 0;
  }

  int d = 0;
  while (1) {
    char r = UART_Receive();
    if(r == '\r' || d>len)
    	break;
    str[d] = r;
    d++;
  }
}

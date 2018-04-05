/**
  ******************************************************************************
  * @file    lcd.h
  * @author  cysun
  * @date    2017-11-21
  ******************************************************************************
***/

#include "gpio.h"
#include "stm32l476xx.h"

#ifndef __UART_H
#define __UART_H



#define DIGIT_ROM_BASE 0b00110000
#define CAP_ROM_BASE 0b01000001
#define LOWER_ROM_BASE 0b01100001

/* uart structure */
typedef struct UARTdef{
  int GPIO_port;
  int rx_pin;
  int tx_pin;
} UARTdef;

void uart_gpio_init(UARTdef* uart_def);
void uart1_init();

int UART_Transmit(uint8_t *arr, uint32_t size);
char UART_Receive();
void UART_Receive_Str(char* str, int len);




#endif

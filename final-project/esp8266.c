/**
  ******************************************************************************
  * @file    esp8266.h
  * @author  cysun
  * @date    2018-1-16
  ******************************************************************************
***/

#include "esp8266.h"

void esp8266_init(char* resp, int n){
  UART_Transmit("AT", strlen("AT"));
  UART_Receive_Str(resp, n);
}

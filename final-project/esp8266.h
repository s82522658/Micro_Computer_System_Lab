/**
  ******************************************************************************
  * @file    esp8266.h
  * @author  cysun
  * @date    2018-1-16
  ******************************************************************************
***/

#include "gpio.h"
#include "stm32l476xx.h"
#include "uart.h"

#ifndef __ESP8266_H
#define __ESP8266_H

/* uart structure */
typedef struct esp8266_def {
  char ip[20];
  char ssid[20];
} esp8266_def;

void esp8266_init(char* resp, int n);


#endif

#include "stm32l476xx.h"
#include "gpio.h"
#include <stdio.h>

#ifndef __MAX6675_H
#define __MAX6675_H

// PB11=CSK PB10=CS PB12=SO
#define   SO_PIN 12
#define   CS_PIN 10
#define   CSK_PIN 11

#define   MAX6675_CSH()   Reg(GPIOB+GPIO_ODR) |= 1<<10;
#define   MAX6675_CSL()   Reg(GPIOB+GPIO_ODR) &= 0b1111101111111111;
#define   MAX6675_CLKH()  Reg(GPIOB+GPIO_ODR) |= 1<<11;
#define   MAX6675_CLKL()  Reg(GPIOB+GPIO_ODR) &= 0b1111011111111111;

unsigned int Temperature_Spi_Read(void);
void SPI_MAX6675_Init(void);


#endif

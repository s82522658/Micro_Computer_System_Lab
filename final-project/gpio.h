/**
  ******************************************************************************
  * @file    gpio.h
  * @author  cysun
  * @date    2017-11-21
  ******************************************************************************
***/

#ifndef __GPIO_H
#define __GPIO_H

#ifndef _REG
#define REG
#define Reg(a) (*(unsigned long *)(a))
#endif

// RCC_AHB2ENR
#define RCC_AHB2ENR 0x4002104C
// enable gpio offset
enum { A = 1, B = 2, C = 4, D = 0x10, E = 0x20, F = 0x40, G = 0x80, H = 0x100 };

// GPIO address
#define PERIPH_BASE           0x40000000U
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000U)
#define GPIOA_BASE            (AHB2PERIPH_BASE + 0x0000U)
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x0400U)
#define GPIOC_BASE            (AHB2PERIPH_BASE + 0x0800U)
#define GPIOD_BASE            (AHB2PERIPH_BASE + 0x0C00U)
#define GPIOE_BASE            (AHB2PERIPH_BASE + 0x1000U)
#define GPIOF_BASE            (AHB2PERIPH_BASE + 0x1400U)
#define GPIOG_BASE            (AHB2PERIPH_BASE + 0x1800U)
#define GPIOH_BASE            (AHB2PERIPH_BASE + 0x1C00U)

#define GPIOA                 GPIOA_BASE
#define GPIOB                 GPIOB_BASE
#define GPIOC                 GPIOC_BASE
#define GPIOD                 GPIOD_BASE
#define GPIOE                 GPIOE_BASE
#define GPIOF                 GPIOF_BASE
#define GPIOG                 GPIOG_BASE
#define GPIOH                 GPIOH_BASE

// GPIO register offset
#define GPIO_MODER 		0x0000
#define GPIO_OTYPER		0x0004
#define GPIO_OSPEEDR 	0x0008
#define GPIO_PUPDR	 	0x000C
#define GPIO_IDR	 		0x0010
#define GPIO_ODR	 		0x0014
#define GPIO_BSRR			0x0018
#define GPIO_BRR			0x0028

// GPIO Pin
#define PIN_NUM 16
#define GPIO_Pin_0                 0x0001  // Pin 0 selected
#define GPIO_Pin_1                 0x0002  // Pin 1 selected
#define GPIO_Pin_2                 0x0004  // Pin 2 selected
#define GPIO_Pin_3                 0x0008  // Pin 3 selected
#define GPIO_Pin_4                 0x0010  // Pin 4 selected
#define GPIO_Pin_5                 0x0020  // Pin 5 selected
#define GPIO_Pin_6                 0x0040  // Pin 6 selected
#define GPIO_Pin_7                 0x0080  // Pin 7 selected
#define GPIO_Pin_8                 0x0100  // Pin 8 selected
#define GPIO_Pin_9                 0x0200  // Pin 9 selected
#define GPIO_Pin_10                0x0400  // Pin 10 selected
#define GPIO_Pin_11                0x0800  // Pin 11 selected
#define GPIO_Pin_12                0x1000  // Pin 12 selected
#define GPIO_Pin_13                0x2000  // Pin 13 selected
#define GPIO_Pin_14                0x4000  // Pin 14 selected
#define GPIO_Pin_15                0x8000  // Pin 15 selected
#define GPIO_Pin_All               0xFFFF  // All pins selected

// speed mode
enum { Low_Speed = 0, Medium_Speed = 1, High_Speed = 2, Very_High_Speed = 3};
// pin mode
enum { Input_Mode = 0, Output_Mode = 1, Alternate_Mode = 2, Analog_Mode = 3};
// PUPDR mode
enum { No_Pull = 0, Pull_UP = 1, Pull_Down = 2 };
// ODR
enum { ON = 1, OFF = 0};

/* GPIO Init structure */
typedef struct
{
	int port;
	int GPIO_Pin;
	int GPIO_Speed;
	int GPIO_Mode;
	int GPIO_PULL;
} GPIO_InitTypeDef;

void GPIO_initialize(GPIO_InitTypeDef init);
void enable_GPIO(int GPIOx);
int get_GPIO_port(int GPIOx);

/* I/O */
void GPIO_Write_Output(int GPIOx, int pin, int on_off);
int GPIO_Read_Input(int GPIOx, int pin);

#endif // __GPIO_H

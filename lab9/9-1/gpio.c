/**
  ******************************************************************************
  * @file    gpio.c
  * @author  cysun
  * @date    2017-11-21
  ******************************************************************************
***/

// Includes
#include "gpio.h"

// Initialize
int get_GPIO_port(int GPIOx)
{
	int GPIO_port;
	switch (GPIOx)
	{
		case A: GPIO_port = GPIOA; break;
		case B: GPIO_port = GPIOB; break;
		case C: GPIO_port = GPIOC; break;
		case D: GPIO_port = GPIOD; break;
		case E: GPIO_port = GPIOE; break;
		case F: GPIO_port = GPIOF; break;
		case G: GPIO_port = GPIOG; break;
		case H: GPIO_port = GPIOH; break;
	}
	return GPIO_port;
}

void enable_GPIO(int GPIOx)
{
	int en = Reg(RCC_AHB2ENR) | GPIOx;
	Reg(RCC_AHB2ENR) = en;
}

void GPIO_initialize(GPIO_InitTypeDef init)
{
	int GPIO_port = get_GPIO_port(init.port);

	// enable
	enable_GPIO(init.port);

	// mode
	unsigned int mode = 0;
	unsigned int mask = 0;
	unsigned int tmp;
	int set_pin = init.GPIO_Pin;

	for (int i = 0; i < PIN_NUM; i++)
	{
		if (set_pin % 2) {
			mode += init.GPIO_Mode << i*2;
			mask += 0b11 << i*2 ;
		}
		set_pin = set_pin >> 1;
	}

	mask = mask ^ -1;
	tmp = Reg(GPIO_port + GPIO_MODER) & mask;
	Reg(GPIO_port + GPIO_MODER) = tmp + mode;

	// speed
	unsigned int speed = 0;
	mask = 0;
	set_pin = init.GPIO_Pin;
	for (int i = 0; i < PIN_NUM; i++)
	{
		if (set_pin % 2) {
			speed += init.GPIO_Speed << i*2;
			mask += 0b11 << i*2 ;
		}
		set_pin = set_pin >> 1;
	}

	mask = mask ^ -1;
	tmp = Reg(GPIO_port + GPIO_OSPEEDR) & mask;
	Reg(GPIO_port + GPIO_OSPEEDR) = tmp + speed;

	// Pull-up/down
	unsigned int pull = 0;
	mask = 0;
	set_pin = init.GPIO_Pin;
	if (init.GPIO_PULL)
	{
		for (int i = 0; i < PIN_NUM; i++)
		{
			if (set_pin % 2) {
				pull += init.GPIO_PULL << i*2;
				mask += 0b11 << i*2 ;
			}
			set_pin = set_pin >> 1;
		}

		mask = mask ^ -1;
		tmp = Reg(GPIO_port + GPIO_PUPDR) & mask;
		Reg(GPIO_port + GPIO_PUPDR) = tmp + pull;
	}
}



// I/O
void GPIO_Write_Output(int GPIOx, int pin, int on_off)
{
	int GPIO_port = get_GPIO_port(GPIOx);
	int old_odr = Reg(GPIO_port + GPIO_ODR) & pin;

	if(old_odr && on_off) return; // already on
	if(!old_odr && !on_off) return;

	if(!old_odr && on_off) Reg(GPIO_port + GPIO_ODR) += pin;
	if(old_odr && !on_off) Reg(GPIO_port + GPIO_ODR) -= pin;

	// if (on_off)
	// 	outputMsk =
  //
	//  = on_off << pin;
	// Reg(GPIO_port + GPIO_ODR) = output;
  //
  //
	// int output = 0, mask = 0, set_pin = pin, tmp;
	// for (int i = 0; i < PIN_NUM; i++)
	// {
	// 	if (set_pin % 2) {
	// 		if(on_off)
	// 			output += 1 << i;
	// 		else
	// 			output = 0;
	// 		// mask += 1 << i;
	// 	}
	// 	set_pin = set_pin >> 1;
	// }
  //
	// // mask = mask ^ -1;
	// // tmp = Reg(GPIO_port + GPIO_ODR) & mask;
	// // Reg(GPIO_port + GPIO_ODR) = tmp + output;
	// Reg(GPIO_port + GPIO_ODR) = output;
}

int GPIO_Read_Input(int GPIOx, int pin)
{
	int GPIO_port = get_GPIO_port(GPIOx);
	int input = Reg(GPIO_port + GPIO_IDR);
	input = input & pin;

	return (input != 0)? 1 : 0;
}

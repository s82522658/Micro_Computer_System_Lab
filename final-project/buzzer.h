#include "stm32l476xx.h"
#include<stdio.h>

#ifndef __BUZZER_H
#define __BUZZER_H

#define do 40000/261
#define re 40000/293
#define mi 40000/329
#define fa 40000/349
#define so 40000/392
#define la 40000/440
#define si 40000/493
#define hdo 40000/523


void Timer_init(uint32_t presc);
void PWM_channel_init();
void buzz_init_AF();

void skycastle();
void bubu();

#endif

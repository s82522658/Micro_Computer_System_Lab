#include "stm32l476xx.h"
#include "gpio.h"
#include <stdio.h>

// PB11=CSK PB10=CS PB12=SO
#define   SO_PIN 12
#define   MAX6675_CSH()   Reg(GPIOB+GPIO_ODR) |= 1<<10;
#define   MAX6675_CSL()   Reg(GPIOB+GPIO_ODR) &= 0b1111101111111111;
#define   MAX6675_CLKH()  Reg(GPIOB+GPIO_ODR) |= 1<<11;
#define   MAX6675_CLKL()  Reg(GPIOB+GPIO_ODR) &= 0b1111011111111111;

static inline void delay_ms(int n){
  asm("push {r0}\r\n"
      "mov r0, %0\r\n"
      "LOOP:\r\n"
      "subs r0, #1\r\n"
      "BGT LOOP\r\n"
      "POP {r0}\r\n"
      :: "r" (n*1333));
}
unsigned int Temperature_Spi_Read(void)
{
    unsigned int j;
    //unsigned char i;
    int error=0;
    j = 0;
    MAX6675_CSL();
    MAX6675_CSH();
    MAX6675_CSL();
    MAX6675_CLKH();
    MAX6675_CLKL();
    for (int i =11; i >= 0; i--){
      MAX6675_CLKH();
      //j = (j <<1);
      // PB12=SO
      if ( Reg(GPIOB+GPIO_IDR) & (1<<SO_PIN) ) j += 1<<i;
      // if ( ((GPIOC->IDR) & (1<<2)) ==0x4) j += 1<<i;
      MAX6675_CLKL();
      //__nop();
    }
    MAX6675_CLKH();
    // if ( ((GPIOB->IDR) & (1<<SO_PIN)) ==0x4) error=1;
    MAX6675_CLKL();

    for(int i=1;i>=0;i--){
      MAX6675_CLKH();
      int a=3;
      MAX6675_CLKL();
    }

    MAX6675_CSH();
    return j;
}

void SPI_MAX6675_Init(void)
{
    GPIO_InitTypeDef max6675_set;
    max6675_set.port = B;
    max6675_set.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    max6675_set.GPIO_Speed = High_Speed;
    max6675_set.GPIO_Mode = Output_Mode;
    max6675_set.GPIO_PULL = Pull_UP;
    GPIO_initialize(max6675_set);

    max6675_set.GPIO_Pin = GPIO_Pin_12;
    max6675_set.GPIO_Mode = Input_Mode;
    GPIO_initialize(max6675_set);

    // RCC->AHB2ENR |= 4;
    // GPIOC->MODER = 0xFFFFFFC5;//PC0 PC1=OUTmode PC2=INmode
    // GPIOC->OSPEEDR=0X000000AA;//High speed mode
    //
    // GPIOC->PUPDR=0x00000010;//PA6=pull-up

    MAX6675_CSH();//���S
    MAX6675_CLKL();  //蝵桐�CK
}

int main ()
{
   unsigned int i;
   float temprature;
   /*蝟餌絞����72M */
   //SystemInit();

   /* MAX6675�����*/
   SPI_MAX6675_Init();
   //USART1_Config();
  while(1)
   {
      i=Temperature_Spi_Read();
      temprature = i*0.25;
      delay_ms(100);
  }
}

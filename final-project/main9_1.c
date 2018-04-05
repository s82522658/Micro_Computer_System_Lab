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
	turnOnScreen(&set_lcd, ON, CURSOR, BLINK_ON);
	clearScreen(&set_lcd);
	resetCursor(&set_lcd);

}

int main()
{
	lcd_init();
	display_init();

	displayStr(&set_lcd, str);

   return 0;
}

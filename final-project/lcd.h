/**
  ******************************************************************************
  * @file    lcd.h
  * @author  cysun
  * @date    2017-11-21
  ******************************************************************************
***/

#include "gpio.h"

#ifndef __LCD_H
#define __LCD_H

enum { RIGHT = 1, LEFT = 0};
enum { SCREEN = 1, CURSOR = 0};
enum { false = 0, true = 1};
enum { BLINK_OFF = 0, BLINK_ON = 1};
enum { Bit_4 = 0, Bit_8 = 1, DOUBLE_ROW = 1, SINGLE_ROW = 0, Font_5_10 = 1, Font_5_7 = 0};

#define DIGIT_ROM_BASE 0b00110000
#define CAP_ROM_BASE 0b01000001
#define LOWER_ROM_BASE 0b01100001

/* LCD structure */
typedef struct LCDdef
{
  int CMD_GPIO_port;
  int Data_GPIO_port;
  int PIN_Lightness;
	int PIN_RS;
	int PIN_RW;
	int PIN_EN;
	int PIN_DB_0;
  int PIN_DB_1;
  int PIN_DB_2;
  int PIN_DB_3;
  int PIN_DB_4;
  int PIN_DB_5;
  int PIN_DB_6;
  int PIN_DB_7;
} LCDdef;

void LCDinit(LCDdef* lcd_def);

void writeByte(LCDdef* lcd_def, unsigned short cmd);
unsigned short readByte(LCDdef* lcd_def);

void writeCommand(LCDdef* lcd_def, unsigned short cmd);
unsigned short readStatus(LCDdef* lcd_def);

void writeDR(LCDdef* lcd_def, unsigned short data);
unsigned short readDR(LCDdef* lcd_def);

void clearScreen(LCDdef* lcd_def);
void resetCursor(LCDdef* lcd_def);
void enterMode(LCDdef* lcd_def, int csr, int dir);
void turnOnScreen(LCDdef* lcd_def, int on_off, int csr, int blink);
void moveCursor(LCDdef* lcd_def, int csr, int dir);
void setFunction(LCDdef* lcd_def, int bit, int row, int font);
void setCGRAMaddr(LCDdef* lcd_def, int addr);
void setDDRAMaddr(LCDdef* lcd_def, int addr);
void readLCM(LCDdef* lcd_def, int* ready, int* addr);

void displayChar(LCDdef* lcd_def, char* c);
void row1_display(LCDdef* lcd_def, char* str);
void row2_display(LCDdef* lcd_def, char* str);

static inline void delay_ms(int n){
	asm("push {r0}\r\n"
			"mov r0, %0\r\n"
			"LOOP:\r\n"
			"subs r0, #1\r\n"
			"BGT LOOP\r\n"
			"POP {r0}\r\n"
			:: "r" (n*1333));
}

static inline void delay_us(int n){
	asm("push {r0}\r\n"
			"mov r0, r0\r\n"
			"LOOP_US:\r\n"
			"nop\r\n"
			"subs r0, #1\r\n"
			"BGT LOOP_US\r\n"
			"POP {r0}\r\n"
			:: "r" (n));
}

#endif

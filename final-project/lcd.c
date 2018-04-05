/**
  ******************************************************************************
  * @file    lcd.c
  * @author  cysun
  * @date    2017-11-21
  ******************************************************************************
***/
#include "lcd.h"

GPIO_InitTypeDef gpio_set;

void LCDinit(LCDdef* lcd_def)
{
  gpio_set.port = lcd_def->CMD_GPIO_port;
  gpio_set.GPIO_Pin = lcd_def->PIN_Lightness + lcd_def->PIN_RS +
                      lcd_def->PIN_RW +	lcd_def->PIN_EN;
  gpio_set.GPIO_Speed = Medium_Speed;
  gpio_set.GPIO_Mode = Output_Mode;
  gpio_set.GPIO_PULL = Pull_UP;
  GPIO_initialize(gpio_set);

  gpio_set.port = lcd_def->Data_GPIO_port;
  gpio_set.GPIO_Pin = lcd_def->PIN_DB_0 + lcd_def->PIN_DB_1 +
                      lcd_def->PIN_DB_2 + lcd_def->PIN_DB_3 +
                      lcd_def->PIN_DB_4 + lcd_def->PIN_DB_5 +
                      lcd_def->PIN_DB_6 + lcd_def->PIN_DB_7;
	gpio_set.GPIO_Mode = Output_Mode;
  GPIO_initialize(gpio_set);

  Reg(GPIOB + GPIO_OTYPER) = 0;
  Reg(GPIOC + GPIO_OTYPER) = 0;
}

void writeByte(LCDdef* lcd_def, unsigned short cmd)
{
  int tmp = Reg(get_GPIO_port(lcd_def->Data_GPIO_port) + GPIO_ODR) & 1111111100000000;
  Reg(get_GPIO_port(lcd_def->Data_GPIO_port) + GPIO_ODR) = tmp + cmd;

  // for (int i = 0; i < 8; i++) {
  //   int c = cmd % 2;
  //   cmd = cmd >> 1;
  //   switch (i) {
  //     case 0: GPIO_Write_Output(lcd_def->GPIO_port, lcd_def->PIN_DB_0, c); break;
  //     case 1: GPIO_Write_Output(lcd_def->GPIO_port, lcd_def->PIN_DB_1, c); break;
  //     case 2: GPIO_Write_Output(lcd_def->GPIO_port, lcd_def->PIN_DB_2, c); break;
  //     case 3: GPIO_Write_Output(lcd_def->GPIO_port, lcd_def->PIN_DB_3, c); break;
  //     case 4: GPIO_Write_Output(lcd_def->GPIO_port, lcd_def->PIN_DB_4, c); break;
  //     case 5: GPIO_Write_Output(lcd_def->GPIO_port, lcd_def->PIN_DB_5, c); break;
  //     case 6: GPIO_Write_Output(lcd_def->GPIO_port, lcd_def->PIN_DB_6, c); break;
  //     case 7: GPIO_Write_Output(lcd_def->GPIO_port, lcd_def->PIN_DB_7, c); break;
  //     default: break;
  //   }
  // }
}

unsigned short readByte(LCDdef* lcd_def)
{
  gpio_set.GPIO_Mode = Input_Mode;
  GPIO_initialize(gpio_set);

  int gp = lcd_def->CMD_GPIO_port;
  unsigned short ir = 0;
  for (int i = 0; i < 8; i++) {
    ir = ir << 1;
    ir += GPIO_Read_Input(gp, lcd_def->PIN_DB_0<<i);
  }
  return ir;
}

void writeCommand(LCDdef* lcd_def, unsigned short cmd)
{
  gpio_set.GPIO_Mode = Output_Mode;
  GPIO_initialize(gpio_set);

  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_RS, OFF);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_RW, OFF);
  writeByte(lcd_def, cmd);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_EN, OFF);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_EN, ON);
  delay_ms(10);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_EN, OFF);
}

unsigned short readStatus(LCDdef* lcd_def)
{
  unsigned short ir;
  Reg(get_GPIO_port(lcd_def->Data_GPIO_port)+GPIO_ODR) = 0;
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_RS, OFF);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_RW, ON);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_EN, ON);
  ir = readByte(lcd_def);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_EN, OFF);
  return ir;
}

void writeDR(LCDdef* lcd_def, unsigned short data)
{
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_RS, ON);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_RW, OFF);
  writeByte(lcd_def, data);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_EN, OFF);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_EN, ON);
  delay_us(100);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_EN, OFF);
}

unsigned short readDR(LCDdef* lcd_def)
{
  unsigned short dr;
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_RS, ON);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_RW, ON);

  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_EN, ON);
  dr = readByte(lcd_def);
  GPIO_Write_Output(lcd_def->CMD_GPIO_port, lcd_def->PIN_EN, OFF);
  return dr;
}

void clearScreen(LCDdef* lcd_def)
{
  writeCommand(lcd_def, 1);
}

void resetCursor(LCDdef* lcd_def)
{
  writeCommand(lcd_def, 0b10);
}

void enterMode(LCDdef* lcd_def, int movScn, int dir)
{
  unsigned short cmd = 0b100;
  cmd += movScn; // D0
  cmd += (movScn)? (dir-1)*2 : dir*2;
  writeCommand(lcd_def, cmd);
}

void turnOnScreen(LCDdef* lcd_def, int on_off, int csr, int blink)
{
  unsigned short cmd = 0b1000;
  cmd += blink;
  cmd += csr<<1;
  cmd += on_off<<2;
  writeCommand(lcd_def, cmd);
  // writeDR(lcd_def, cmd);
}

void moveCursor(LCDdef* lcd_def, int csr, int dir)
{
  unsigned short cmd = 0b10000;
  cmd += csr<<3; // D3, S/C
  cmd += dir<<2; // D2, R/L
  writeCommand(lcd_def, cmd);
}

void setFunction(LCDdef* lcd_def, int bit, int row, int font)
{
  unsigned short cmd = 0b100000;
  cmd += bit << 4; // D4, bit number
  cmd += row << 3;
  cmd += font << 2;
  writeCommand(lcd_def, cmd);
}

void setCGRAMaddr(LCDdef* lcd_def, int addr)
{
  unsigned short cmd = 0b1000000;
  cmd += addr;
  writeCommand(lcd_def, cmd);
}

void setDDRAMaddr(LCDdef* lcd_def, int addr)
{
  unsigned short cmd = 0b10000000;
  cmd += addr;
  writeCommand(lcd_def, cmd);
}

void readLCM(LCDdef* lcd_def, int* ready, int* addr)
{
  unsigned short st = readStatus(lcd_def);
  ready = st >> 7;
  addr = st & 0b01111111;
}

void displayChar(LCDdef* lcd_def, char* c)
{
  // CGROM addr
  unsigned short cg;
  cg = (c >= 'A' && c <= 'Z')? (c-'A')+CAP_ROM_BASE : (c-'a')+LOWER_ROM_BASE;
  // display
  writeDR(lcd_def, cg);
}

void row1_display(LCDdef* lcd_def, char* str)
{
  // clear
  setDDRAMaddr(lcd_def, 0);
  for (int i = 0; i < 16; i++) {
    displayChar(lcd_def, ' ');
  }

  // display each char
  setDDRAMaddr(lcd_def, 0);
  for (int i = 0; str[i] != 0; i++) {
    displayChar(lcd_def, str[i]);
  }
}

void row2_display(LCDdef* lcd_def, char* str)
{
  // clear
  setDDRAMaddr(lcd_def, 0x40);
  for (int i = 0; i < 16; i++) {
    displayChar(lcd_def, ' ');
  }

  // display each char
  setDDRAMaddr(lcd_def, 0x40);
  for (int i = 0; str[i] != 0; i++) {
    displayChar(lcd_def, str[i]);
  }
}

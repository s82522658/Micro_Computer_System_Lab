/**
  ******************************************************************************
  * @file    keypad.h
  * @author  cysun
  * @date    2017-11-21

  * Keypad[0:3] = X[0:3] is output
  * Keypad[4:7] = Y[0:3] is input
  ******************************************************************************
***/

// include
#include "gpio.h"

// enum & struct

#define DELAY 1000

enum { KEY_1 = 0, KEY_2 = 1, KEY_3 = 2, KEY_A = 3, KEY_4 = 4, KEY_5 = 5,
	   KEY_6 = 6, KEY_B = 7, KEY_7 = 8, KEY_8 = 9, KEY_9 = 10, KEY_C = 11,
       KEY_STAR = 12, KEY_0 = 13, KEY_HASH = 14, KEY_D = 15 };

typedef struct KEYPAD_InitTypeDef
{
	int X_GPIO;
	int Y_GPIO;
	int x_pin[4];
	int y_pin[4];
} KEYPAD_InitTypeDef;

// initialize funcitons
void keypad_init(KEYPAD_InitTypeDef kp_set);

// keypad scan funcitons
int if_keypad_pushed(KEYPAD_InitTypeDef kp_set);
void keypad_scan(int* keypad_stat, KEYPAD_InitTypeDef kp_set);
int get_push_btn(int* keypad_stat);

// keypad state functions
void keypad_stat_init(int* stat);
void get_stat(int* to, int* from);
int stat_update(int* old, int* new);

// btn to num
int get_btn_num(int btn);

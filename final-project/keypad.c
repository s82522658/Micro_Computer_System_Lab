/**
  ******************************************************************************
  * @file    keypad.c
  * @author  cysun
  * @date    2017-11-24
  ******************************************************************************
***/

// include
#include "gpio.h"
#include "keypad.h"

// initialize funcitons
void keypad_init(KEYPAD_InitTypeDef kp_set)
{
	GPIO_InitTypeDef GPIO_set;

	// column
	GPIO_set.port = kp_set.X_GPIO;
	GPIO_set.GPIO_Pin = kp_set.x_pin[0] | kp_set.x_pin[1] | kp_set.x_pin[2] | kp_set.x_pin[3];
	GPIO_set.GPIO_Speed = Medium_Speed;
	GPIO_set.GPIO_Mode = Output_Mode;
	GPIO_set.GPIO_PULL = Pull_UP;
	GPIO_initialize(GPIO_set);
	GPIO_Write_Output(kp_set.X_GPIO, GPIO_set.GPIO_Pin, ON);

	// row
	GPIO_set.port = kp_set.Y_GPIO;
	GPIO_set.GPIO_Pin =	kp_set.y_pin[0] | kp_set.y_pin[1] | kp_set.y_pin[2] | kp_set.y_pin[3];
	GPIO_set.GPIO_Speed = Medium_Speed;
	GPIO_set.GPIO_Mode = Input_Mode;
	GPIO_set.GPIO_PULL = Pull_Down;
	GPIO_initialize(GPIO_set);
}

// keypad scan
int if_keypad_pushed(KEYPAD_InitTypeDef kp_set)
{
	int push = 0;
	int row4 = GPIO_Read_Input(kp_set.Y_GPIO, kp_set.y_pin[3]);
	int row3 = GPIO_Read_Input(kp_set.Y_GPIO, kp_set.y_pin[2]);
	int row2 = GPIO_Read_Input(kp_set.Y_GPIO, kp_set.y_pin[1]);
	int row1 = GPIO_Read_Input(kp_set.Y_GPIO, kp_set.y_pin[0]);
	push = row1 + row2 + row3 + row4;

	return push;
}

void keypad_scan(int* keypad_stat, KEYPAD_InitTypeDef kp_set)
{
	int flag_debounce = 0, d = 0;

	GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[3], ON);
	GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[2], ON);
	GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[1], ON);
	GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[0], ON);

	int flag_keypad = if_keypad_pushed(kp_set);

    // debounce handling
    if (flag_keypad) // push
		d = DELAY;
	while (d != 0) {
		flag_debounce = if_keypad_pushed(kp_set);
		d--;
	}

    // btn push
	if (flag_debounce != 0)
	{
		// scan keypad
		for(int x = 0; x < 4; x++)
		{
			GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[3], OFF);
			GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[2], OFF);
			GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[1], OFF);
			GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[0], OFF);

			GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[x], ON);
			for (int y = 0; y < 4; y++)	{
				keypad_stat[x+y*4] = GPIO_Read_Input(kp_set.Y_GPIO, kp_set.y_pin[y]);
			}
		}
	}
	else  // set all the key unpushed
	{
		keypad_stat_init((int*)keypad_stat);
		GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[3], OFF);
		GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[2], OFF);
		GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[1], OFF);
		GPIO_Write_Output(kp_set.X_GPIO, kp_set.x_pin[0], OFF);
	}
}

int get_push_btn(int* keypad_stat)
{
	int btn;

    // check keypad state
    for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (keypad_stat[i*4 + j]) {
				btn = i*4 + j;
                break;
			}
		}
	}

    return btn;
}

// keypad state function
void keypad_stat_init(int* stat)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			stat[i*4+j] = 0;
		}
	}
}

void get_stat(int* to, int* from)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			to[i*4+j] = from[i*4+j];
		}
	}
}

int stat_update(int* old, int* new)
{
	int update = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if ( old[i*4+j] == 0 && new[i*4+j] == 1 ) // new btn push
				update++;
		}
	}
	return update;
}

// get push num
int get_btn_num(int btn)
{
	switch (btn)
	{
			case KEY_0: return 0; break;
			case KEY_1: return 1; break;
			case KEY_2: return 2; break;
			case KEY_3: return 3; break;
			case KEY_4: return 4; break;
			case KEY_5: return 5; break;
			case KEY_6: return 6; break;
			case KEY_7: return 7; break;
			case KEY_8: return 8; break;
			case KEY_9: return 9; break;
			case KEY_A: return 10; break;
			case KEY_B: return 11; break;
			case KEY_C: return 12; break;
			case KEY_D: return 13; break;
	}
}

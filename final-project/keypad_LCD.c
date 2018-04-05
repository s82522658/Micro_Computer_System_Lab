#include "gpio.h"
#include "keypad.h"
#include "lcd.h"

KEYPAD_InitTypeDef kp_set;
KEYPAD_InitTypeDef keypadInit()

{
    kp_set.X_GPIO = A;
    kp_set.x_pin[0] = GPIO_Pin_4;
    kp_set.x_pin[1] = GPIO_Pin_6;
    kp_set.x_pin[2] = GPIO_Pin_5;
    kp_set.x_pin[3] = GPIO_Pin_1;

    kp_set.Y_GPIO = C;
    kp_set.y_pin[0] = GPIO_Pin_8;
    kp_set.y_pin[1] = GPIO_Pin_7;
    kp_set.y_pin[2] = GPIO_Pin_6;
    kp_set.y_pin[3] = GPIO_Pin_5;

    keypad_init(kp_set);
}


LCDdef set_lcd;
void lcd_init()
{
	set_lcd.CMD_GPIO_port = C;
	set_lcd.Data_GPIO_port = B;
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

void lcd_display_init()
{
  setFunction(&set_lcd, Bit_8, DOUBLE_ROW, Font_5_7);
	enterMode(&set_lcd, CURSOR, RIGHT);
	turnOnScreen(&set_lcd, ON, CURSOR, BLINK_ON);
	clearScreen(&set_lcd);
	resetCursor(&set_lcd);
}

int main()
{
    keypadInit();
    lcd_init();
    lcd_display_init();

    int keypad_stat[4][4];
    int cur_stat[4][4];
    row1_display(&set_lcd, "test keypad");

    while (1)
    {
        get_stat((int*)cur_stat, (int*)keypad_stat);
        keypad_scan((int*)keypad_stat, kp_set);

        if (stat_update((int*)cur_stat, (int*)keypad_stat))
        {
        	int btn = get_push_btn((int*)keypad_stat);
        	switch (btn)
            {
                case KEY_0: row2_display(&set_lcd, "0"); break;
                case KEY_1: row2_display(&set_lcd, "1"); break;
                case KEY_2: row2_display(&set_lcd, "2"); break;
                case KEY_3: row2_display(&set_lcd, "3"); break;
                case KEY_4: row2_display(&set_lcd, "4"); break;
                case KEY_5: row2_display(&set_lcd, "5"); break;
                case KEY_6: row2_display(&set_lcd, "6"); break;
                case KEY_7: row2_display(&set_lcd, "7"); break;
                case KEY_8: row2_display(&set_lcd, "8"); break;
                case KEY_9: row2_display(&set_lcd, "9"); break;
                case KEY_A: row2_display(&set_lcd, "A"); break;
                case KEY_B: row2_display(&set_lcd, "B"); break;
                case KEY_C: row2_display(&set_lcd, "C"); break;
                case KEY_D: row2_display(&set_lcd, "D"); break;
                case KEY_STAR: row2_display(&set_lcd, "*"); break;
                case KEY_HASH: row2_display(&set_lcd, "#"); break;
            }
        }
    }

    return 0;
}

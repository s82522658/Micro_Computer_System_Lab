#include "gpio.h"

// user button
int on_off = 0;
void btn_init()
{
  GPIO_InitTypeDef set_btn;
  set_btn.port = C;
  set_btn.GPIO_Pin = GPIO_Pin_13;
  set_btn.GPIO_Mode = Input_Mode;
  GPIO_initialize(set_btn);
}

int user_press_button()
{
  int push = 0;
  push = !GPIO_Read_Input(C, GPIO_Pin_13);
  int d = 5000;

  if(push) {
    while (d) {
      d--;
      push = !GPIO_Read_Input(C, GPIO_Pin_13);
    }
  }

  if(push)
    return 1;
  else
    return 0;
}

/* main */

while(1){
    if(user_press_button()){
      on_off = 1 - on_off;
      if(on_off) {
        // relay_open();
        // row1_display(&set_lcd, "Relay open!");
      }
      else {
        // relay_close();
        // row1_display(&set_lcd, "Relay close!");
      }
    }
}

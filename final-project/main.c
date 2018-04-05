#include "stm32l476xx.h"
#include "gpio.h"
#include "lcd.h"
#include "keypad.h"
#include "uart.h"
//#include "esp8266.h"
#include "max6675.h"
#include "buzzer.h"
#include <stdio.h>
#include <string.h>
#include <math.h>


#define TIME_SEC 1
#define TIM_ARR_VAL 3999
#define MSI_DEFAULT_FREQ 4000000//4MHz
#define true 1
#define flase 0

/* devices */
// buzzer
void buzzer_init() {
  GPIO_InitTypeDef set_buzz;
  set_buzz.port = A;
  set_buzz.GPIO_Pin = GPIO_Pin_15;
  set_buzz.GPIO_Mode = Alternate_Mode;
  GPIO_initialize(set_buzz);

  buzz_init_AF();
  Timer_init(30000);
  PWM_channel_init();
}


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

// relay
GPIO_InitTypeDef relay;
void relay_init()
{
  relay.port = B;
  relay.GPIO_Pin = GPIO_Pin_9;
  relay.GPIO_Mode = Output_Mode;
  GPIO_initialize(relay);
  GPIO_Write_Output(relay.port, relay.GPIO_Pin, OFF);
}

void relay_open() {
  GPIO_Write_Output(relay.port, relay.GPIO_Pin, ON);
}

void relay_close() {
  GPIO_Write_Output(relay.port, relay.GPIO_Pin, OFF);
}

// lcd
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

// keypad
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

// uart
UARTdef uart_set;
void uart_init() {
  uart_set.GPIO_port = A;
  uart_set.rx_pin = GPIO_Pin_10;
  uart_set.tx_pin = GPIO_Pin_9;
  uart_gpio_init(&uart_set);
  uart1_init();
}


char start_str[] = "start";

void delay_s(int s){
  for (int i = 0; i < s; i++) {
    delay_ms(1000);
  }
}

void delay_m(int m){
  for (int i = 0; i < m; i++) {
    delay_s(60);
  }
}

int get_num_digs(int n)
{
	if(n == 0) return 1;
	int digit = 0;
	while (n) {
		digit++;
		n = n / 10;
	}
	return digit;
}

void UlToStr(char *s, unsigned long bin, unsigned char n)
{
    s += n;
    *s = '\0';

    while (n--) {
        *--s = (bin % 10) + '0';
        bin /= 10;
    }
}

void counting_s(int s) {
  char time_str[16];
  for (int i = s; i >= 0; i--) {
    delay_ms(980);
    strcpy(time_str, "remain: ");

    // remain time
    int rm = i/60;
    int md = get_num_digs(rm);
    char m_str[4];
    UlToStr(m_str, rm, md);
    strcat(time_str, m_str);
    strcat(time_str, "m ");

    int rs = i%60;
    char s_str[4];
    int sd = get_num_digs(rs);
    UlToStr(s_str, rs, sd);
    strcat(time_str, s_str);
    strcat(time_str, "s");

    // current temprature
    char temp_str[16];
    char t_str[5];
    strcpy(temp_str, "temp: ");
    unsigned int t; float temp;
    t = Temperature_Spi_Read();
    temp = t*0.25;
    int t_int;
    t_int = t*25;
    t_int /= 100;
    UlToStr(t_str, t_int, get_num_digs(t_int));
    strcat(temp_str, t_str);
    strcat(temp_str, ".");
    int tf = (t*25) % 100;
    UlToStr(t_str, tf, get_num_digs(tf));
    strcat(temp_str, t_str);
    row1_display(&set_lcd, temp_str);
    row2_display(&set_lcd, time_str);
  }
}

int buf_to_num(int* buf, int dig) {
	int num = 0, dec = 1;
  for (int i = 0; i < dig; i++) {
		num += buf[i]*dec;
		dec = dec*10;
	}
	return num;
}

void buf_push(int* buf, int input) {
	buf[3] = buf[2];
	buf[2] = buf[1];
	buf[1] = buf[0];
	buf[0] = input;
}

int get_user_time() {
  int min_buf[4];
  int sec_buf[4];
  int keypad_stat[4][4];
  int cur_stat[4][4];
  char m_str[6];
  char s_str[6];
  row1_display(&set_lcd, "input min(#end):");
  setDDRAMaddr(&set_lcd, 0x40);
  int btn = -1;
  int min = 0, sec = 0;
  int md = 0, sd = 0;
  while (btn != KEY_HASH) {
      get_stat((int*)cur_stat, (int*)keypad_stat);
      keypad_scan((int*)keypad_stat, kp_set);
      if (stat_update((int*)cur_stat, (int*)keypad_stat)){
          btn = get_push_btn((int*)keypad_stat);
          if(btn == KEY_HASH) break;
          md++;
          int n_num = get_btn_num(btn);
          buf_push(min_buf, n_num);
          min = buf_to_num(min_buf, md);

          UlToStr(m_str, min, md);
          strcat(m_str, "m");
          row2_display(&set_lcd, m_str);
      }
  }

  row1_display(&set_lcd, "input sec(#end):");
  row2_display(&set_lcd, " ");
  setDDRAMaddr(&set_lcd, 0x40);

  btn = -1;
  while (btn != KEY_HASH) {
        get_stat((int*)cur_stat, (int*)keypad_stat);
        keypad_scan((int*)keypad_stat, kp_set);
        if (stat_update((int*)cur_stat, (int*)keypad_stat)){
            btn = get_push_btn((int*)keypad_stat);
            if(btn == KEY_HASH) break;
            sd++;
            int n_num = get_btn_num(btn);
            buf_push(sec_buf, n_num);
            sec = buf_to_num(sec_buf, sd);
            UlToStr(s_str, sec, sd);
            strcat(s_str, "s");
            row2_display(&set_lcd, s_str);
        }
    }

  char confirm_str[16];
  strcpy(confirm_str, "time: ");
  strcat(confirm_str, m_str);
  strcat(confirm_str, " ");
  strcat(confirm_str, s_str);
  row1_display(&set_lcd, confirm_str);
  row2_display(&set_lcd, "press # to start");

  btn = -1;
  while(btn != KEY_HASH) {
	 keypad_scan((int*)keypad_stat, kp_set);
	 btn = get_push_btn((int*)keypad_stat);
  }

  return min*60 + sec;
}

void clear_receive_buf(char* buf, int len) {
	for(int i=0; i< len; i++) {
		buf[i] = 0;
	}
}

/* main */
int main(){
  lcd_init();
  lcd_display_init();
  relay_init();
  btn_init();
  keypadInit();
  uart_init();
  SPI_MAX6675_Init();

  while(1)
  {
    row1_display(&set_lcd, "A: input time");
    row2_display(&set_lcd, "B: wifi mode");

    int keypad_stat[4][4];
    int cur_stat[4][4];
    int btn = -1;
    while(btn != KEY_A && btn != KEY_B) {
    	 keypad_scan((int*)keypad_stat, kp_set);
    	 btn = get_push_btn((int*)keypad_stat);
      }

    if(btn == KEY_A) {
    	clearScreen(&set_lcd);
    	int t = get_user_time();
    	relay_open();
    	row1_display(&set_lcd, "cooking...");
    	counting_s(t);
    	relay_close();
    	row1_display(&set_lcd, "cook finish!");
    	row2_display(&set_lcd, "singing...");

    	buzzer_init();
    	bubu();

    	row2_display(&set_lcd, "# restart");
    	int btn = -1;
    	    while(btn != KEY_HASH) {
    	    	 keypad_scan((int*)keypad_stat, kp_set);
    	    	 btn = get_push_btn((int*)keypad_stat);
    	      }

    }

    if(btn == KEY_B) {
      clearScreen(&set_lcd);
      char resp[16];
      char receive_data[200]; int d= 0;
        clear_receive_buf(receive_data, 200); d=0;

        // AT
        UART_Transmit("AT\r\n", strlen("AT\r\n"));
        // UART_Transmit("AT+CWJAP=\"824\",\"94879487\"", strlen("AT+CWJAP=\"824\",\"94879487\""));
        while (d<12) {
           char r = UART_Receive();
           receive_data[d] = r;
           d++;
        }
        row1_display(&set_lcd, receive_data);
        delay_s(1);
        clear_receive_buf(receive_data, 200); d=0;

        // wifi
        row1_display(&set_lcd, "CONNECT wifi..");
        UART_Transmit("AT+CWJAP=\"cy der iphone\",\"password\"\r\n", strlen("AT+CWJAP=\"cy der iphone\",\"password\"\r\n"));
        while (d<125) {
             char r = UART_Receive();
             receive_data[d] = r;
             d++;
          }

        row1_display(&set_lcd, receive_data);
        row2_display(&set_lcd, "wifi OK");
        delay_s(1);

        // TCP
        clear_receive_buf(receive_data, 200); d=0;
        row1_display(&set_lcd, "CONNECT TCP..");
        UART_Transmit("AT+CIPSTART=\"TCP\",\"10.64.186.83\",5001\r\n", strlen("AT+CIPSTART=\"TCP\",\"10.64.186.83\",5001\r\n"));

        while (d<60) {
               char r = UART_Receive();
               receive_data[d] = r;
               d++;
            }
        row1_display(&set_lcd, receive_data);
        row2_display(&set_lcd, "TCP OK");

        // send hello message
        clear_receive_buf(receive_data, 200); d=0;
        row1_display(&set_lcd, "hi server...");
        UART_Transmit("AT+CIPSEND=10\r\n", strlen("AT+CIPSEND=10\r\n"));
        while (d<25) {
                 char r = UART_Receive();
                 receive_data[d] = r;
                 d++;
              }

        clear_receive_buf(receive_data, 200); d=0;
        UART_Transmit("hi iPhone\r\n", strlen("hi iPhone\r\n"));
        while (d<50) {
                   char r = UART_Receive();
                   receive_data[d] = r;
                   d++;
                }

        row2_display(&set_lcd, "send finish");

        // wait for start cmd
        row1_display(&set_lcd, "wait cmd...");
        row2_display(&set_lcd, "send to start");
          char tmp = USART1->RDR;
          clear_receive_buf(receive_data, 200); d=0;
          while (!(USART1->ISR & 0b100000));
          while (d<10) {
                           char r = UART_Receive();
                           receive_data[d] = r;
                           d++;
                        }
          relay_open();
          row1_display(&set_lcd, "cooking...");
          int cook_time = (receive_data[8]-'0')*60;

          clear_receive_buf(receive_data, 200); d=0;
                	        UART_Transmit("AT+CIPSEND=10\r\n", strlen("AT+CIPSEND=10\r\n"));
                	        while (d<25) {
                	                 char r = UART_Receive();
                	                 receive_data[d] = r;
                	                 d++;
                	              }

                	        clear_receive_buf(receive_data, 200); d=0;
                	        UART_Transmit("cook start\r\n", strlen("cook finish\r\n"));
                	        while (d<50) {
                	                   char r = UART_Receive();
                	                   receive_data[d] = r;
                	                   d++;
                	                }


          counting_s(cook_time);






        // row1_display(&set_lcd, receive_data);

        // send to close
        // row1_display(&set_lcd, "wait cmd...");
//        row2_display(&set_lcd, "send to stop");
//        tmp = USART1->RDR;
//        while (!(USART1->ISR & 0b100000));
//                  clear_receive_buf(receive_data, 200); d=0;
//                    while (d<10) {
//
//                         char r = UART_Receive();
//                         receive_data[d] = r;
//                         d++;
//                      }
        relay_close();
//      	int t = get_user_time();
//
//      	row1_display(&set_lcd, "cooking...");
//      	counting_s(t);

      	row1_display(&set_lcd, "cook finish!");
      	// send finish message
      	        clear_receive_buf(receive_data, 200); d=0;
      	        UART_Transmit("AT+CIPSEND=11\r\n", strlen("AT+CIPSEND=11\r\n"));
      	        while (d<25) {
      	                 char r = UART_Receive();
      	                 receive_data[d] = r;
      	                 d++;
      	              }

      	        clear_receive_buf(receive_data, 200); d=0;
      	        UART_Transmit("cook finish\r\n", strlen("cook finish\r\n"));
      	        while (d<50) {
      	                   char r = UART_Receive();
      	                   receive_data[d] = r;
      	                   d++;
      	                }

      	      row2_display(&set_lcd, "singing...");
      	    buzzer_init();

      	          	bubu();

      	row2_display(&set_lcd, "# restart");
      	    	int btn = -1;
      	    	    while(btn != KEY_HASH) {
      	    	    	 keypad_scan((int*)keypad_stat, kp_set);
      	    	    	 btn = get_push_btn((int*)keypad_stat);
      	    	      }
    }
  }

  // while(1){
  //     if(user_press_button()){
  //       on_off = 1 - on_off;
  //       if(on_off) {
  //         relay_open();
  //         row1_display(&set_lcd, "Relay open!");
  //       }
  //       else {
  //         relay_close();
  //         row1_display(&set_lcd, "Relay close!");
  //       }
  //     }
  // }

  return 0;
}

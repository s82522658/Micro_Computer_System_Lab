.syntax unified
   .cpu cortex-m4
   .thumb
   .text
.global GPIO_init

.equ million, 250000
.equ sec, 500000
.equ thousand, 1000
.equ RCC_AHB2ENR, 0x4002104C
  .equ GPIOA_MODER, 0x48000000
  .equ GPIOA_OTYPER,0x48000004
  .equ GPIOA_OSPEEDR,0x48000008
  .equ GPIOA_PUPDR, 0x4800000C
  .equ GPIOA_ODR, 	0x48000014
  .equ	DATA,		0x20//PA5
  .equ	LOAD,		0x40//PA6
  .equ	CLOCK,		0x80//PA7
  .equ	DECODE_MODE,0x09
  .equ	DISPLAY_TEST,0x0F
  .equ	SCAN_LIMIT,	0x0B
  .equ	INTENSITY,	0x0A
  .equ	SHUTDOWN,	0x0C
  .equ	GPIO_BSRR_OFFSET, 0x18
  .equ	GPIO_BRR_OFFSET, 0x28
  .equ GPIOB_MODER,    0x48000400
 .equ GPIOC_MODER,    0x48000800
  .equ GPIOC_OTYPER,   0x48000804
  .equ GPIOC_OSPEEDR,  0x48000808
  .equ GPIOC_PUPDR,    0x4800080C
  .equ GPIOC_IDR,      0x48000810
  .equ GPIOC_ODR,      0x48000814


GPIO_init:
//TODO: Initialize three GPIO pins as output for max7219 DIN, CS and CLK

	push {r0-r11,lr}
	movs 	r0, #0x7
	ldr 	r1, =RCC_AHB2ENR
	str 	r0,	[r1]

	//set PA5,PA6,PA7 as output mode PA1
	ldr	r0,	=#0x5404
	ldr		r1, =GPIOA_MODER
	ldr		r2,	[r1]
	ldr		r5,=#0xFFFF57F7
	and		r2, r5
	orrs	r2,	r2,	r0
	str		r2,	[r1]
	//set PA5,PA6,PA7 as high speed mode
	LDR	r0,	=#0xA808
	ldr		r1,	=GPIOA_OSPEEDR
	strh	r0, [r1]
	//1011 1111//pb 3 Alternate function mode


	ldr  r1,=GPIOB_MODER
	ldr r2, =0xfffffffbf
	str  r2, [r1]


//pc 0 Alternate function mode
	//Set PC 13 as input mode
	ldr  r1,=GPIOC_MODER
	ldr r2, =0xf3ffffffe
	str  r2, [r1]

	//Default PB is Pull-up output

	//Set PC13 as high speed mode
	movs r0, #0xf000000
	ldr  r1, =GPIOC_OSPEEDR
	strh r0, [r1]


	pop {r0-r11,pc}
	BX LR


Delay_sec:
	//TODO: Write a delay 1sec function
	push {r4-r11,lr}
	ldr r5,=sec//2
dsec_loop:
	subs r5,r5,#1//1
//	cmp r5,#0//1
	bgt dsec_loop//1 or 3
	pop {r4-r11,lr}
	BX LR

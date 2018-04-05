	.syntax unified
	.cpu cortex-m4
	.thumb

.text
	.global GPIO_init
	.global MHz_delay_1s
	.global User_button
	.equ RCC_AHB2ENR,0x4002104C
	.equ GPIOA_MODER,0x48000000
	.equ GPIOA_OTYPER,0x48000004
	.equ GPIOA_OSPEEDR,0x48000008
	.equ GPIOA_PUPDR,0x4800000C
	.equ GPIOA_ODR,0x48000014
	.equ A,0xF3FFFFD5
	.equ GPIOC, 0x48000800
	.equ GPIOC_MODER, 0x48000800
	.equ GPIOC_OTYPER, 0x48000804
	.equ GPIOC_OSPEEDR, 0x48000808
	.equ GPIOC_PUPDR, 	0x4800080C
	.equ GPIOC_IDR, 	0x48000810
	.equ GPIOC_ODR, 	0x48000814
	.equ GPIO_BSRR,	0x18
	.equ GPIO_BRR,	0x28
	.equ million,1000000
	.equ thousand,1000

GPIO_init:
	push {r4-r11,lr}
	//enable clk on port C and A
	movs r0,#0x5
	ldr r1,=RCC_AHB2ENR
	str r0,[r1]
	
	movs r0,#0x400
	ldr r1,=GPIOA_MODER
	ldr r2,[r1]
	and r2,#0xFFFFF3FF
	orrs r2,r2,r0
	str r2,[r1]

	movs r0,#0x800
	ldr r1,=GPIOA_OSPEEDR
	strh r0,[r1]

	//set mode of port c
	ldr r0,=GPIOC_MODER
	ldr r2,=A
	str r2,[r0]
//set PC_0~PC_2 as high speed mode (why?
	ldr r0,=GPIOC_OSPEEDR
	movs r1,#0x2A
	str r1,[r0]


	pop {r4-r11,pc}
MHz_delay_1s:
	push {r4-r11,lr}
	//TODO: Write a delay 1sec function
	ldr r5,=million//2
d_loop:
	subs r5,r5,#1//1
	cmp r5,#0//1
	bgt d_loop//1 or 3

	pop {r4-r11,pc}

User_button:
	push {r4-r11,lr}
	ldr r0,=GPIOC_IDR
	ldr r3,[r0]
	movs r4,#1
	lsl r4,#13
	ands r3,r4
	beq debounce//0=push,1=unpush
	movs r0,#0
	pop {r4-r11,pc}
debounce://for unpush->push
	bl Delay
	ldr r0,=GPIOC_IDR
	ldr r3,[r0]
	movs r4,#1
	lsl r4,#13
	ands r3,r4
	beq do_pushed //pushed
	movs r0,#0//unpush
	pop {r4-r11,pc}
debounce_2://for short push->unpush
	bl Delay
	ldr r0,=GPIOC_IDR
	ldr r3,[r0]
	movs r4,#1
	lsl r4,#13
	ands r3,r4
	bne short_pushed //unpush
	beq do_pushed //push
do_pushed:
	ldr r0,=GPIOC_IDR
	ldr r3,[r0]
	movs r4,#1
	lsl r4,#13
	ands r3,r4
	bne debounce_2 //push->unpush

	b do_pushed//push->push
short_pushed:
	movs r0,#1
	pop {r4-r11,pc}
Delay:
	//TODO: Write a delay 20msec function
	ldr r5,=thousand//2
de_loop:
	subs r5,r5,#1//1
	bgt de_loop//1 or 3

	BX LR

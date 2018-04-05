.syntax unified
	.cpu cortex-m4
	.thumb
.data
	arr: .byte 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x77, 0x1f, 0x4e, 0x3d, 0x4f, 0x47 //TODO: put 0 to F 7-Seg LED pattern here

.text
	.global main
	.equ million, 1500000
	.equ RCC_AHB2ENR, 0x4002104C
	.equ GPIOC, 0x48000800
	.equ GPIOC_MODER, 0x48000800
	.equ GPIOC_OTYPER, 0x48000804
	.equ GPIOC_OSPEEDR, 0x48000808
	.equ GPIOC_PUPDR, 	0x4800080C
	.equ GPIOC_IDR, 	0x48000810
	.equ GPIOC_ODR, 	0x48000814
	.equ GPIO_BSRR,	0x18
	.equ GPIO_BRR,	0x28
	//DIN=PC_0,CS=PC_1,CLK=PC_2
	.equ CS,0x2
	.equ DIN,0x1
	.equ CLK,0x4
	.equ DECODE_MODE, 0x9
	.equ DISPLAY_TEST,0xF
	.equ INTENSITY, 0xA
	.equ SCAN_LIMIT, 0xB
	.equ SHUTDOWN, 0xC

main:
    BL   GPIO_init
    BL   max7219_init
loop:
    BL   Display0toF
    B loop

GPIO_init:
	//TODO: Initialize three GPIO pins as output for max7219 DIN, CS and CLK
//enable clk on port c
	ldr r0,=RCC_AHB2ENR
	movs r1,#0x4
	str r1,[r0]
//set mode of port c
	ldr r0,=GPIOC_MODER
	movs r1,#0xFFFFFFD5
	str r1,[r0]
//set PC_0~PC_2 as high speed mode (why?
	ldr r0,=GPIOC_OSPEEDR
	movs r1,#0x2A
	str r1,[r0]
//set PC_0~PC_2 as pull-down(actice low)(好像不需要  其實可以不用設)
	//ldr r0,=GPIOC_PUPDR
	//movs r1,#0x2A
	//str r1,[r0]

	BX LR

Display0toF:
	//TODO: Display 0 to F at first digit on 7-SEG LED. Display one per second.
	//occupy r10~r12
	push {r0, r1, r2, lr}
	movs r12,#0
	ldr r10,=arr
in_loop:
	ldrb r11,[r10,r12] //r12 is offset of arr
	ldr r0,=0x1
	movs r1,r11
	bl MAX7219Send
	bl Delay
	adds r12,r12,#1
	cmp r12,#16
	blt in_loop
	pop {r0, r1, r2, lr}
	BX LR
MAX7219Send:
   //input parameter: r0 is ADDRESS , r1 is DATA
   //occupy r0~r7
	//TODO: Use this function to send a message to max7219
	lsl r0, r0, #8
	add r0, r0, r1//merge r0,r1 to a 16bit data
	ldr r1, =GPIOC
	ldr r2, =CS
	ldr r3, =DIN
	ldr r4, =CLK
	ldr r5, =GPIO_BSRR
	ldr r6, =GPIO_BRR
	mov r7, #16//r7 = i
max7219send_loop:
//occupy r8~r9
	mov r8, #1 //for selecting 'the bit' we want to deliver
	sub r9, r7, #1
	lsl r8, r8, r9 // r8 = mask
	str r4, [r1,r6]//set clk=0
	tst r0, r8 //1=not equal 1, 0=equal 1
	beq bit_not_set//bit not set
	str r3, [r1,r5] //deliver '1' to DIN
	b if_done
bit_not_set:
	str r3, [r1,r6] //deliver '0' to DIN
if_done:
	str r4, [r1,r5]//set clk=1
	subs r7, r7, #1//i--
//	cmp r7,#-1
	bgt max7219send_loop
	str r2, [r1,r6] //set cs=1
	str r2, [r1,r5] //set cs=0
	BX LR
max7219_init:
	//TODO: Initialize max7219 registers

	push {r0, r1, r2, lr}
	ldr r0, =DECODE_MODE
	ldr r1, =#0x00//close decode mode
	BL MAX7219Send
	ldr r0, =DISPLAY_TEST
	ldr r1, =#0x0//close display_test
	BL MAX7219Send
	ldr r0, =SCAN_LIMIT
	ldr r1, =#0x0//show digit0
	BL MAX7219Send
	ldr r0, =INTENSITY
	ldr r1, =#0x1//brightness 19/32
	BL MAX7219Send
	ldr r0, =SHUTDOWN
	ldr r1, =#0x1//normal operation
	BL MAX7219Send
	pop {r0, r1, r2, pc}

	BX LR

Delay:
	//TODO: Write a delay 1sec function
	ldr r5,=million//2
d_loop:
	subs r5,r5,#1//1
	cmp r5,#0//1
	bgt d_loop//1 or 3

	BX LR


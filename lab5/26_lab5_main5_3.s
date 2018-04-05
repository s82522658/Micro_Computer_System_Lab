.syntax unified
	.cpu cortex-m4
	.thumb
.data

.text
	.global main
	.equ thousand, 1000
	.equ million, 250000
	.equ billion,100000000
	.equ ten,10
	.equ A,0xF3FFFFD5
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
	//DIN=PC_0,CS=PC_1,CLK=PC_2,User_Button=PC_13
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
    bl	 fib_init
	b User_button
GPIO_init:
	//TODO: Initialize three GPIO pins as output for max7219 DIN, CS and CLK
//enable clk on port c
	ldr r0,=RCC_AHB2ENR
	movs r1,#0x4
	str r1,[r0]
//set mode of port c
	ldr r0,=GPIOC_MODER
	ldr r2,=A
	str r2,[r0]
//set PC_0~PC_2 as high speed mode (why?
	ldr r0,=GPIOC_OSPEEDR
	movs r1,#0x2A
	str r1,[r0]
//set PC_0~PC_2 as pull-down(actice low)(好像不需要  其實可以不用設)
	//ldr r0,=GPIOC_PUPDR
	//movs r1,#0x2A
	//str r1,[r0]

	BX LR

User_button:
	//push {r0,r1,r2,lr}
	ldr r0,=GPIOC_IDR
	ldr r3,[r0]
	movs r4,#1
	lsl r4,#13
	ands r3,r4
	beq debounce//0=push,1=unpush
	b User_button
	//pop {r0,r1,r2,lr}
debounce://for unpush->push
	bl Delay
	ldr r0,=GPIOC_IDR
	ldr r3,[r0]
	movs r4,#1
	lsl r4,#13
	ands r3,r4
	beq do_pushed //pushed
	b User_button //unpush
debounce_2://for short push->unpush
	bl Delay
	ldr r0,=GPIOC_IDR
	ldr r3,[r0]
	movs r4,#1
	lsl r4,#13
	ands r3,r4
	bne short_pushed //unpush
	beq do_loop //push
debounce_3://for long push->unpush
	bl Delay
	ldr r0,=GPIOC_IDR
	ldr r3,[r0]
	movs r4,#1
	lsl r4,#13
	ands r3,r4
	bne User_button //unpush
	beq l_loop //push
do_pushed:
//r10=fibo's n
	ldr r5,=million//2
do_loop:
	ldr r0,=GPIOC_IDR
	ldr r3,[r0]
	movs r4,#1
	lsl r4,#13
	ands r3,r4
	bne debounce_2

	subs r5,r5,#1//1
//	cmp r5,#0//1
	bgt do_loop//1 or 3
	//no debounce for longpush!
long_pushed:
	bl fib_init
	//movs r4,#0
	//movs r0,#0x1
	//movs r1,#0
	//bl MAX7219Send
l_loop:
	ldr r0,=GPIOC_IDR
	ldr r3,[r0]
	movs r4,#1
	lsl r4,#13
	ands r3,r4
	bne debounce_3
	b l_loop
short_pushed:
	adds r10,r10,#1
	bl fib
	cmp r4,#-1
	beq overflow
check:
//before display,check where is the highest bit
//r2=100000000,r3=10
	ldr r2,=billion
	ldr r3,=ten
	movs r6,#9
c_loop:
	subs r6,r6,#1
	udiv r2,r2,r3
	udiv r5,r4,r2
	mul r7,r5,r2
	subs r4,r4,r7
	cmp r5,#0
	beq c_loop
Display:
	//TODO: Display fibo's answer
	push {r2,r3,r4,r6}
	movs r1,r5
	movs r0,r6
	bl MAX7219Send
	pop {r2,r3,r4,r6}
	b devide
devide:
	subs r6,r6,#1
	cmp r6,#0
	beq User_button //finish display
	udiv r2,r2,r3
	udiv r5,r4,r2
	mul r7,r5,r2
	subs r4,r4,r7
	b Display
overflow:
	movs r0,#0x2
	movs r1,#0xA
	bl MAX7219Send
	movs r0,#0x1
	movs r1,#1
	bl MAX7219Send
	movs r0,#0x3
	movs r1,#0xF
	bl MAX7219Send
	movs r0,#0x4
	movs r1,#0xF
	bl MAX7219Send
	movs r0,#0x5
	movs r1,#0xF
	bl MAX7219Send
	movs r0,#0x6
	movs r1,#0xF
	bl MAX7219Send
	movs r0,#0x7
	movs r1,#0xF
	bl MAX7219Send
	movs r0,#0x8
	movs r1,#0xF
	bl MAX7219Send
	b User_button
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
	ldr r1, =#0xFF//open codeB decode mode
	BL MAX7219Send
	ldr r0, =DISPLAY_TEST
	ldr r1, =#0x0//close display_test
	BL MAX7219Send
	ldr r0, =SCAN_LIMIT
	ldr r1, =#0x7//show digit0~7
	BL MAX7219Send
	ldr r0, =INTENSITY
	ldr r1, =#0x3//brightness 19/32
	BL MAX7219Send
	ldr r0, =SHUTDOWN
	ldr r1, =#0x1//normal operation
	BL MAX7219Send
	pop {r0, r1, r2, pc}

	BX LR
Delay:
	//TODO: Write a delay 20msec function
	ldr r5,=thousand//2
d_loop:
	subs r5,r5,#1//1
	//cmp r5,#0//1
	bgt d_loop//1 or 3

	BX LR

Delay_sec:
	//TODO: Write a delay 1sec function
	ldr r5,=million//2
dsec_loop:
	subs r5,r5,#1//1
//	cmp r5,#0//1
	bgt dsec_loop//1 or 3

	BX LR
fib_init:
//n stores in r10
//r11=n-2,r12=n-1,r4=answer
	push {lr}
	movs r10,#0
	movs r11,#0
	movs r12,#1
	movs r2,#8
f_loop:
	push {r2}
	movs r0,r2
	movs r1,#0xf
	bl MAX7219Send
	pop {r2}
	subs r2,r2,#1
	bgt f_loop

	movs r0,#0x1
	movs r1,#0
	bl MAX7219Send
	pop {lr}
	bx lr
first:
	movs r4,#1
	bx lr
fib:
	cmp r10,#1
	beq first
    adds r4,r11,r12
    //if fib(n)>=100000000,r4=-1
    ldr r0,=billion
    cmp r4,r0
    bge ov

	movs r11,r12
	movs r12,r4

    bx lr
ov://overflow
	movs r4,-1
	bx lr

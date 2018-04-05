	.syntax unified
	.cpu cortex-m4
	.thumb

.text
	.global GPIO_init
	.global Display
	.global max7219_init
	.global enable_fpu
	.equ RCC_AHB2ENR,0x4002104C
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
	.equ CS,0x2
	.equ DIN,0x1
	.equ CLK,0x4
	.equ DECODE_MODE, 0x9
	.equ DISPLAY_TEST,0xF
	.equ INTENSITY, 0xA
	.equ SCAN_LIMIT, 0xB
	.equ SHUTDOWN, 0xC
	.equ million,1000000
	.equ thousand,1000
	.equ billion,100000000
	.equ ten,10
enable_fpu:
    //Your code start from here
	//; CPACR is located at address 0xE000ED88
	LDR.W R0, =0xE000ED88
	//; Read CPACR
	LDR R1, [R0]
	// Set bits 20-23 to enable CP10 and CP11 coprocessors
	ORR R1, R1, #(0xF << 20)
	//; Write back the modified value to the CPACR
	STR R1, [R0]

    bx lr

GPIO_init:
	push {r4-r11,lr}
	//enable clk on port C
	movs r0,#0x4
	ldr r1,=RCC_AHB2ENR
	str r0,[r1]

	//set mode of port c
	ldr r0,=GPIOC_MODER
	ldr r2,=A
	str r2,[r0]
	
	//set PC_0~PC_2 as high speed mode (why?
	ldr r0,=GPIOC_OSPEEDR
	movs r1,#0x2A
	str r1,[r0]

	pop {r4-r11,pc}

MAX7219Send:
   //input parameter: r0 is ADDRESS , r1 is DATA
   //occupy r0~r7
	//TODO: Use this function to send a message to max7219
	
	//push {r4-r11,lr}
	
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
	//pop {r4-r11,pc}

max7219_init:
	//TODO: Initialize max7219 registers

	push {r4-r11,lr}
	ldr r0, =DECODE_MODE
	ldr r1, =#0xFF//open codeB decode mode
	BL MAX7219Send
	ldr r0, =DISPLAY_TEST
	ldr r1, =#0x0//close display_test
	BL MAX7219Send
	ldr r0, =SCAN_LIMIT
	ldr r1, =#0x6//show digit0~7
	BL MAX7219Send
	ldr r0, =INTENSITY
	ldr r1, =#0x3//brightness 19/32
	BL MAX7219Send
	ldr r0, =SHUTDOWN
	ldr r1, =#0x1//normal operation
	BL MAX7219Send
	
	bl stopwatch_init

	pop {r4-r11,pc}
stopwatch_init:
	push {lr}
	//print the initial value
	movs r0,#0x1
	movs r1,#0
	bl MAX7219Send
	movs r0,#0x2
	movs r1,#0
	bl MAX7219Send
	movs r0,#0x3
	movs r1,#0x80
	bl MAX7219Send
	movs r0,#0x4
	movs r1,#0xf
	bl MAX7219Send
	movs r0,#0x5
	movs r1,#0xf
	bl MAX7219Send
	movs r0,#0x6
	movs r1,#0xf
	bl MAX7219Send
	movs r0,#0x7
	movs r1,#0xf
	bl MAX7219Send

	pop {lr}
	bx lr	
add_dot:
	movs r9,#0x80
	adds r5,r5,r9
	b display2
Display:
	push {r4-r11,lr}
	movs r4,r0//r4=display value
	cmp r4,#0
	beq overflow
	bl check
display:
	cmp r6,#3
	beq add_dot
display2:
	movs r1,r5
	movs r0,r6
	push {r2,r3,r4,r6}
	bl MAX7219Send
	pop {r2,r3,r4,r6}
	b devide
finish:	
	pop {r4-r11,pc}
overflow:
	bl stopwatch_init
	b finish
devide:
	subs r6,r6,#1
	//cmp r6,#0
	ble finish //finish display
	udiv r2,r2,r3
	udiv r5,r4,r2
	mul r7,r5,r2
	subs r4,r4,r7
	b display
check:
//before display,check where is the highest bit
//r2=100000000,r3=10,r4=display value
	ldr r2,=billion
	ldr r3,=ten
	movs r6,#9
c_loop:
	subs r6,r6,#1
	ble end
	udiv r2,r2,r3
	udiv r5,r4,r2
	mul r7,r5,r2
	subs r4,r4,r7
	cmp r5,#0
	beq c_loop
	bx lr
end: bx lr

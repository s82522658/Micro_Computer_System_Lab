.syntax unified
.cpu cortex-m4
.thumb

.data
    leds: .byte 		0
    control_led: .word	0xFE7F
    active: .word		1
	password: .byte		0xC // password
	cur_state: .byte	0

.text
    .global main
	.equ RCC_AHB2ENR, 	0x4002104C
	.equ GPIOA_MODER, 	0x48000000
	.equ GPIOA_OTYPER, 	0x48000004
	.equ GPIOA_OSPEEDR, 0x48000008
	.equ GPIOA_PUPDR, 	0x4800000C
	.equ GPIOA_ODR, 	0x48000014

	.equ GPIOB_MODER, 	0x48000400
	.equ GPIOB_OTYPER, 	0x48000404
	.equ GPIOB_OSPEEDR, 0x48000408
	.equ GPIOB_PUPDR, 	0x4800040C
	.equ GPIOB_ODR, 	0x48000414

	.equ GPIOC_MODER, 	0x48000800
	.equ GPIOC_OTYPER, 	0x48000804
	.equ GPIOC_OSPEEDR, 0x48000808
	.equ GPIOC_PUPDR, 	0x4800080C
	.equ GPIOC_IDR, 	0x48000810
	.equ GPIOC_ODR, 	0x48000814

	//.equ N,				3
	// .equ N,				2000000
	.equ N,				1000000
	.equ DEB,			1000


GPIO_init:
    // Enable AHB2 clock
	movs	r0, #0x7
	ldr		r1, =RCC_AHB2ENR
	str		r0, [r1]

	// PB
	ldr		r1, =GPIOB_MODER
	mov		r2, #0
	orr		r2, #0xFFFFD57F
	str		r2, [r1]

	// Set GPIOC Pin13, Pin0 ~ Pin3 as input mode
	ldr		r1, =GPIOC_MODER
	ldr		r0, [r1]
	ldr 	r2, =#0xF3FFFF00
	and 	r0, r2
	str 	r0, [r1]

	// set PB_3 ~ PB_6 as high speed mode
	movs	r0, #0x2A80 // 10 10 10 10 00 00 00
	ldr		r1, =GPIOB_OSPEEDR
	str		r0, [r1]

	// set PB_3 ~ PB_6 as pull-down mode
	ldr		r1, =GPIOB_PUPDR
	mov		r2, #0
	orr		r2, #0x2A80
	str		r2, [r1]

	// set PC_3 ~ PC_6 as pull-down mode
	ldr		r1, =GPIOC_PUPDR
	ldr		r0, [r1]
	mov 	r2, #0xaa
	orr		r2, r2, r0
	str		r2, [r1]

	// r0 = GPIOB_ODR
	ldr 	r2, =GPIOB_ODR

	// turn off PB3~6
	movs	r0, #0
	orr		r0, #0xFFFF
	str		r0, [r2]
	// ldr		r2, =GPIOC_IDR
	// str		r0, [r2]

    BX LR


main:
    BL GPIO_init

Loop:
    // check if push
	ldr		r1, =cur_state
	ldrb	r0, [r1]

	// get DIP switch state
	ldr		r3, =GPIOC_IDR
	ldrb	r2, [r3] // GPIOC_IDR[0:3], first byte of GPIOC_IDR
	mvn		r2, r2
	and		r2, #0xF

	cmp		r0, r2
	beq		v_end // unchange

	// update cur_state
	strb	r2, [r1]

	// compare DIP with password
	ldr		r1, =password
	ldrb	r0, [r1]

	cmp		r0, r2
	beq		correct
	bne		wrong

wrong:
 // blink one time
	ldr 	r2, =GPIOB_ODR

	// turn on PB3~6
	movs	r0, #0
	orr		r0, #0xFF87
	str		r0, [r2]

	BL		Delay

	// only blink one time
	// turn off PB3~6
	movs	r0, #0
	orr		r0, #0xFFFF
	str		r0, [r2]

	b 		v_end

correct: // blink three times
	ldr 	r2, =GPIOB_ODR

	// turn on PB3~6
	movs	r0, #0
	orr		r0, #0xFF87
	str		r0, [r2]

	BL		Delay

	// turn off PB3~6
	movs	r0, #0
	orr		r0, #0xFFFF
	str		r0, [r2]

	// delay 1
	BL		Delay

	// turn on PB3~6
	movs	r0, #0
	orr		r0, #0xFF87
	str		r0, [r2]

	BL		Delay

	// turn off PB3~6
	movs	r0, #0
	orr		r0, #0xFFFF
	str		r0, [r2]

	// delay 2
	BL		Delay

	// turn on PB3~6
	movs	r0, #0
	orr		r0, #0xFF87
	str		r0, [r2]

	BL		Delay

	// turn off PB3~6
	movs	r0, #0
	orr		r0, #0xFFFF
	str		r0, [r2]

v_end:
    B Loop

Delay:
	ldr		r4, =[N]
	mov 	r5, #0
d_loop:
	subs	r4, r4, #1
	bne		d_loop
	bx		lr

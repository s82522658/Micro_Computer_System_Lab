.syntax unified
.cpu cortex-m4
.thumb

.data
    leds: .byte 		0
    control_led: .word	0xFE7F

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

	.equ N,			2000000

main:
    BL GPIO_init
    MOVS R1, #1
    LDR R0, =leds
    STRB R1, [R0]

Loop:
    //TODO: Write the display pattern into leds variable
    BL DisplayLED
    BL Delay
    B Loop

GPIO_init:
    // Enable AHB2 clock
	movs	r0, #0x2
	ldr		r1, =RCC_AHB2ENR
	str		r0, [r1]

	// PB
	ldr		r1, =GPIOB_MODER
	mov		r2, #0
	orr		r2, #0xFFFFD57F
	str		r2, [r1]

	// set PB_3 ~ PB_6 as high speed mode
	movs	r0, #0x2A80 // 10 10 10 10 00 00 00
	ldr		r1, =GPIOB_OSPEEDR
	str		r0, [r1]

	// set PB_3 ~ PB_6 as pull-down mode
	ldr		r1, =GPIOB_PUPDR
	mov		r2, #0
	orr		r2, #0x2A80
	str		r2, [r1]

	// r0 = GPIOB_ODR
	ldr 	r2, =GPIOB_ODR

	// turn off PB3~6
	movs	r0, #0
	orr		r0, #0xFFFF
	str		r0, [r2]

    BX LR

DisplayLED :
	ldr		r1, =GPIOB_ODR
	ldr		r2, =leds
	ldrb	r0, [r2]

	// update leds
	ldr		r3, =control_led
	ldr		r3, [r3]
	mov		r4, r0
	cmp		r0, #5
	ble		pos

	mov		r6, #10
	sub		r4, r6, r0
	cmp		r0, #8
	beq		back
	add		r0, r0, #1
	strb	r0, [r2]
	b		refresh

back:
	mov		r0, #1
	strb	r0, [r2]
	b		refresh

pos:
	mov		r4, r0
	add		r0, r0, #1
	strb	r0, [r2]

refresh:
	lsr		r4, r3, r4
	str		r4, [r1]
	// shift left or right
    BX LR


Delay:
	ldr		r4, =[N]
	mov 	r5, #0
d_loop:
	subs	r4, r4, #1
	bne		d_loop
	bx		lr

    BX LR


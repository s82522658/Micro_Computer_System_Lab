.syntax unified
.cpu cortex-m4
.thumb

.data
    leds: .byte 		0
    control_led: .word	0xFE7F
    active: .word		1

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
	.equ N,				666666
	.equ DEB,			40000

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
	movs	r0, #0x7
	ldr		r1, =RCC_AHB2ENR
	str		r0, [r1]

	// PB
	ldr		r1, =GPIOB_MODER
	mov		r2, #0
	orr		r2, #0xFFFFD57F
	str		r2, [r1]

	// Set GPIOC Pin13 as input mode
	ldr		r1, =GPIOC_MODER
	ldr		r0, [r1]
	ldr 	r2, =#0xF3FFFFFF
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

	// r0 = GPIOB_ODR
	ldr 	r2, =GPIOB_ODR

	// turn off PB3~6
	movs	r0, #0
	orr		r0, #0xFFFF
	str		r0, [r2]
	ldr		r2, =GPIOC_IDR
	str		r0, [r2]

    BX LR

DisplayLED :
	ldr		r1, =active
	ldr		r1, [r1]
	cmp		r1, #0
	beq		display_end

	ldr		r1, =GPIOB_ODR
	ldr		r2, =leds
	ldrb	r0, [r2]

	// update led
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
display_end:
    BX LR

Delay:
	ldr		r4, =[N]
	mov 	r5, #0
	ldr r2, =GPIOC_IDR
	mov		r9, #0
d_loop:
	ldr r3, [r2]
	cmp r3, #0
	beq	debounce
	subs	r4, r4, #1
	bne		d_loop
	bx		lr

    BX LR

r_continue:
	subs	r4, r4, #1
	bne		r_continue
	bx		lr


debounce: // wait 20ms, then read again
	// back up r4
	mov 	r8, r4
	ldr		r4, =[DEB]
	mov 	r5, #0
b_loop:
	subs	r4, r4, #1
	bne		b_loop

	ldr r3, [r2]
	cmp r3, #0
	beq do_pushed
	mov		r4, r8
	b r_continue

do_pushed:
	ldr r6, =active
	ldr r7, [r6]
	cmp r7, #1
	beq off
	mov r7, #1
	b b_end
off:
	mov r7, #0

b_end:
	str r7, [r6]
	mov		r4, r8
	b r_continue

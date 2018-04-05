	.syntax unified
	.cpu cortex-m4
	.thumb
.data
    result: .byte 0
.text
    .global main
	.equ X, 0x55AA
	.equ Y, 0xAA55

hamm:
    //TODO
    eor r3,r0,r1
	str r3,[r2]
    bx lr
main:
    ldr r0, =X //This code will cause assemble error. Why? And how to fix.
    ldr r1, =Y
    ldr r2, =result
    bl hamm
L: b L

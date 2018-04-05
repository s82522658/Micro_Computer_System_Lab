	.syntax unified
	.cpu cortex-m4
	.thumb
.data


	X: .word 5
	Y: .word 10
	Z: .word 0
.text
	.global main
	.equ ten, 0x0A

main:
//r1=x
	ldr r0, =X
	ldr r1, [r0]
//r2=y
	ldr r0, =Y
	ldr r2, [r0]
//x=x*10+y
	movs r3, #ten
	mul r1,r1,r3
	adds r1,r1,r2

	ldr r0,=X
	str r1,[r0]
//z=y-x
	//r4=z
	ldr r0,=Z
	ldr r4,[r0]
	subs r4,r2,r1

	str r4,[r0]



L: B L

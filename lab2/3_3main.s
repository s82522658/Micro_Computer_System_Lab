	.syntax unified
	.cpu cortex-m4
	.thumb
.data
	arr1: .byte 0x19, 0x34, 0x14, 0x32, 0x52, 0x23, 0x61, 0x29
	arr2: .byte 0x18, 0x17, 0x33, 0x16, 0xFA, 0x20, 0x55, 0xAC
.text
    .global main

//r0 array addr
//r1 i
//r2 j
//r3
//r4 data[j]
//r5 data[j+1]


swap:

	strb r4,[r0]
	subs r0,r0,1
	strb r5,[r0]
	adds r0,r0,1

	b loop2b
loop2:

	ldrb r4,[r0]
	adds r0,r0,1
	ldrb r5,[r0]

//if data[j+1]<data[j],swap
	cmp r5,r4
	blt swap

loop2b:
	adds r2,r2,1
//if j<i,loop2
	cmp r2,r1
	blt loop2

	bx lr

loop1:
	movs r2,0//r2=j
	bl loop2
	subs r0,r0,r2//point r0 to head of str
	subs r1,r1,1//i--

//when r1>=0,loop1
	movs r3,0
	cmp r3,r1
	blt loop1

	pop {pc}//load return addr

do_sort:
    //TODO
	movs r1,7//r1=i

	push {lr}//store return addr
	b loop1

    bx lr
main:
    ldr r0, =arr1
    bl do_sort
    ldr r0, =arr2
    bl do_sort
L: b L

	.syntax unified
	.cpu cortex-m4
	.thumb

.text
    .global main
	.equ N, 60
	.equ lb,99
	.equ sb,1

fib:
    //TODO
    //n stores in r0
    //r2,r3,r4 for calculate

    adds r4,r2,r3
    cmp r4,r2
    blt ov

	movs r2,r3
	movs r3,r4

	subs r0,r0,1
	cmp r0,0
	bne fib

    bx lr
ofr://out of range
	movs r4,-1
	b L
ov://overflow
	movs r4,-2
	b L
main:
    movs r0, #N
    movs r5, #lb
    movs r6, #sb
    //if N<1 || N>100, jump to ofr
    cmp r0, r6
    blt ofr
    cmp r5,r0
    blt ofr

	subs r0,r0,2
	movs r2,#sb
    movs r3,#sb

    bl fib
L: b L

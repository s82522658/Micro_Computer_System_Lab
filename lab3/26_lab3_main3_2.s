	.syntax unified
	.cpu cortex-m4
	.thumb
.data
	result: .word  0
	max_size:  .word  0

.text
	.global main
	m: .word  0x5E
	n: .word  0x60
	.equ two,2

main:
	//push m,n to stack

	movs r9,#0
	movs r8,#0

	ldr r0,=m
	ldr r1,[r0]
	ldr r0,=n
	ldr r2,[r0]
	push {r1,r2}
	adds r8,r8,#2
	cmp r8,r9
    it gt
    movgt r9,r8

	bl gcd
	ldr r0,=result
	str r4,[r0]
	ldr r0,=max_size
	str r9,[r0]

	b program_end
gcd:
    //TODO: Implement your GCD function
    //r4 for result
    //r9 for staksize
    //push return addr
    movs r1,#0
    movs r2,#0
    pop {r1,r2}
    push {lr}

    subs r8,r8,#1


	//if(a==0) return b
	cmp r1,#0
	it eq
	moveq r4,r2
	beq store
	//if(b==0) return a
	cmp r2,#0
	it eq
	moveq r4,r1
	beq store
	//a%2
	and r3,r1,#1
	cmp r3,#0
	bne aobn
	//b%2
	and r3,r2,#1
	cmp r3,#0
	bne aebo

	//a%2==0 && b%2==0
	//a>>1,b>>1
	lsr r1,#1
	lsr r2,#1

	push {r1,r2}
	adds r8,r8,#2
	cmp r8,r9
    it gt
    movgt r9,r8

	bl gcd
	ldr r3,=two
	mul r4,r4,r3
	b store

	subs r8,r8,#1
	pop {pc}

aobn://a is odd,b is unknown
	//b%2
	and r3,r2,#1
	//a,b are both odd,b to aobo
	cmp r3,#0
	bne aobo
	//a is odd,b is even
	//b>>1
	lsr r2,#1

	push {r1,r2}
	adds r8,r8,#2
	cmp r8,r9
    it gt
    movgt r9,r8

	bl gcd
	b store
aobo://a,b are both odd
	cmp r1,r2
	blt min
	//a,b both odd,and a>=b
	sub r1,r1,r2

	push {r1,r2}
	adds r8,r8,#2
	cmp r8,r9
    it gt
    movgt r9,r8

	bl gcd
	b store

min://a,b both odd,and a<b
	//r1<r2
	sub r3,r2,r1
	movs r2,r1
	movs r1,r3

	push {r1,r2}
	adds r8,r8,#2
	cmp r8,r9
    it gt
    movgt r9,r8

	bl gcd
	b store

aebo://a is even,b is odd
	//a>>1
	lsr r1,#1

	push {r1,r2}
	adds r8,r8,#2
	cmp r8,r9
    it gt
    movgt r9,r8

	bl gcd
	b store

store:
	subs r8,r8,#1
	pop {pc}

program_end:
	b program_end

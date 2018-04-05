	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	user_stack:	.zero 128
	expr_result: .word   0


.text
	.global main
	postfix_expr: .asciz  "3 5 4 - +"
	.equ ten, 10
	.equ zero, 0

main:
	LDR	R0, =postfix_expr
	ldr r1, =user_stack
	adds r1,r1,#128
	msr msp,r1
	bl atoi
	ldr r0,=expr_result
	pop {r5}
	str r5,[r0]

	b program_end
//TODO: Setup stack pointer to end of user_stack and calculate the expression using PUSH, POP operators, and store the result into expr_result

atoi:
    //TODO: implement a “convert string to integer” function

	//r1 for string,r2 for native
	//r5,r6 for calculating
	//r4 for int,r9 for counter
	//r8 for ten or zero
	//push {lr}
	movs r9,#-1//counter
	movs r4,#0//int buffer
	movs r2,#0//native flag
loop:
	adds r9,r9,#1//move ptr to next digit
	ldrb r1,[r0,r9]
	//judge wether it's end of ascii string
	cmp r1,#0
	beq end_str

	//judge wether it's native/minus
	cmp r1,#'-'
	itt eq
	addeq r2,r2,#1//flag(r2)=1
	beq loop
	//judge wether it's space
	cmp r1,#' '
	beq space
	//judge wether r1<0
	cmp r1,#'0'
	blt sign
	//judge wether r1>9
	cmp r1,#'9'
	bgt sign
	//data=data*10
	ldr r8,=ten
	mul r4,r4,r8

	//digits stay
	//data=input[i]-'0'
	subs r1,r1,#'0'

	//sum+=data
	adds r4,r4,r1

	b loop


space:
	//judge wether ‘-’ exists
	cmp r2,#1
	//not '-'
	bne  space_b
	//'-'
	//clear flag
	mov r2,#0
	cmp r4,#0
	itt eq
	moveq r1,#'-'
	beq sign
	ldr r8,=zero
	sub r4,r8,r4
	b digit
space_b:
	cmp r4,#0
	beq loop//space with r4=0

digit:
	push {r4}
	movs r4,#0
	b loop

sign:
	pop {r5,r6}
	cmp r1,#'+'
	it eq
	addeq r5,r6,r5
	cmp r1,#'-'
	it eq
	subeq r5,r6,r5
	cmp r1,#'*'
	it eq
	muleq r5,r6,r5


	push {r5}
	b loop
final_minus:
	pop {r5,r6}
	sub r5,r6,r5
	mov r2, #0
	push {r5}
end_str:
	cmp r2,#1
	beq final_minus
	bx lr
program_end:
	B		program_end

	.include "2440addr.inc"

	.code	32
	.text

	.global		Push_Stack_Int_3
Push_Stack_Int_3:
	stmdb r0!,{r1,r2,r3}
	mov pc,lr

	.global		Pop_Stack_Int_3
Pop_Stack_Int_3:
	stmfd sp!,{r4-r6,lr}

	ldmia r0!,{r4,r5,r6}
	str r4,[r1]
	str r5,[r2]
	str r6,[r3]

	ldmfd sp!,{r4-r6,pc}


	.end


	.end











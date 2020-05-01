/*----------------------------------------------------------------------------
LAB EXERCISE:SQUARE ROOT APPROXIMATION
--------------------------------------
Write an assembly code subroutine to approximate the square root of an argument 
using the bisection method. All math is done with integers, so the resulting 
square root will also be an integer
 *----------------------------------------------------------------------------*/

__asm int my_sqrt(int x)
{
	push	{r4, r5}
	ldr		r1, =0
	ldr		r2, =0xffff
	ldr		r3, =0xffffffff
	
Loop
	mov		r5, r3
	adds	r3, r1, r2
	asrs	r3, #1
	mov		r4, r3
	muls	r4, r3, r4
	cmp		r4, r0
	beq		Done
	bgt		Higher
	mov		r1, r3
	b			Cont
	
Higher
	mov		r2, r3

Cont
	cmp		r5, r3
	bne		Loop

Done
	mov		r0, r3
	pop		{r4,r5}
	bx		lr
	
}

int main(void)
{
		volatile int r, j=0;
		int i;
		r = my_sqrt(0); 	// should be 0
		r = my_sqrt(36); 	// should be 6
		r = my_sqrt(145); // should be 12
	
		for (i=0; i<10000; i++) {
			r = my_sqrt(i);
			j+=r;
		}
		

		while (1)
			;
}
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   

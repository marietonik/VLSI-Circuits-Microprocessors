/*------------------------ MARIA ARETI GERMANOU 57807 -------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>


__asm void VenMachine(int money_input) {

		LDR R2,= 0x1FFFF100 // Address of every type of money that can be accepted. I do not use a function with pointers like Lab's 1 code.
		
		MOVS R3,#0x05 // The hex value of 5 euros is copied in R3. I do this for every accepted value.
		STR R3,[R2] // Take the address in R2, and then store the value from the register R3 to the memory pointed to by R2.
		ADDS R2,R2,#0x04 // I increase the pointer of address value in R2 so i can store the other 4 values. 
		
		//I choose a 0x04 icreament because i have a 32-bit memory.
		
		MOVS R3,#0xA // The hex value of 10 euros is copied in R3. I do this for every accepted value.
		STR R3,[R2] // Again store the value from the register R3 to the memory pointed to by R2.
		ADDS R2,R2,#0x04 // Again i increase the pointer of address value in R2 so i can store the other 3 values.
	
		MOVS R3,#0x14 // The hex value of 20 euros is copied in R3. I do this for every accepted value.
		STR R3,[R2] // Again store the value from the register R3 to the memory pointed to by R2.
		ADDS R2,R2,#0x04 // Again i increase the pointer of address value in R2 so i can store the other 2 values.
	
		MOVS R3,#0x32 // The hex value of 50 euros is copied in R3. I do this for every accepted value.
		STR R3,[R2] // Again store the value from the register R3 to the memory pointed to by R2.
		ADDS R2,R2,#0x04 // Again i increase the pointer of address value in R2 so i can store the other 1 value.
	
		MOVS R3,#0x64 // The hex value of 100 euros is copied in R3. I do this for every accepted value.
		STR R3,[R2] // Again store the value from the register R3 to the memory pointed to by R2.
		
		LDR R2,= 0x01FFFF100 // Start from the first item. 

		// Intilisations of input signals.
		
		LDR R4,= 0x01FFFF400 // Here is the address of the first of the input signals. 
		
		MOVS R3,#0 // Copy the value 0 in R3 before store it in the memory. This signal is for the acceptance of money inserted in the machine.
		STR R3,[R4] // Store the value from the register R3 to the memory pointed to by R4.
		ADDS R4,R4,#0x04 // Again i increase the pointer of address value in R4 so i can store the other 1 input.

		MOVS R3,#0 // Copy the value 0 in R3 before store it in the memory. This signal is for the on or off switch for pump.
		STR R3,[R4] // Store the value from the register R3 to the memory pointed to by R4.
		
		// After defining input signals the inserted amount must be calculated.
		
		LDR R4,= 0x01FFFF400 // Check if the start sensor is activated.
		CMP R1,#1 // If the the sensor for accepting money is activated, start calculating the inserted money.
		BEQ start_scenario // if the value in R1 is equal go to scenario_1.
		
		
start_scenario
				
				MOVS R5,#0 // Intialise the sum of money to 0.
				MOVS R3,#1 // The sensor for input is activated and we copy the activation value to R3.
				STR R3,[R4] // The activation value is stored in pointed address of R4.
				
				LDR R2,= 0x01FFFF100 // Start from the first item. 
				
loop_money // This loop compares every element stored in address in R2 with the input stored in R0.

				LDR R3,[R2] // Load the value of pointer in R2 address.	
				ADDS R2,R2,#0x04 // Again i increase the pointer of address value in R2 so the loop checks the next value that pointer points.
				CMP R0,R3 // Compare the value of euros with input money in R0.
				BNE loop_money // if not equal, return to the loop.
				ADDS R5,R3 // Copy the value of euros in register with the final amount of money.
				
				STR R5,[R4,#0x08] // I copy the result of money somewhere in address stored in R4 to be sure that i won't lost it. This step is optional.
				
				MOVS R3,#0 // The sensor for input is de-activated and we copy the de-activation value to R3.
				STR R3,[R4] // The activation value is stored in pointed address of R4.
								
// calculate liters
				
				LDR R6,= 0x1FFFF200 // Prepare to store results in R6's address pointed.
				MOVS R3,#0x02 // Clear the value of R3 and prepare it for other use. Let's store the value of 2.
				MOVS R2,#0 // Clear the pointer to perform calculations.

				// Remeber the money amount inserted is stored in R5 and we want to calculate liters for all values except 5.
				MOVS R1,#0xFA // Copy the value 250 to avoid decimals.
				LSLS R1,R1,#0x02 // Multiply by 4 to form 1000.
				MULS R5,R1,R5 // I multiply the number by 1000 before i divide to avoid decimals numbers. So my results will be in liters and mseconds.

div_loop // This loop is a simple division algorithm with a zero remnant (modulo 0). I can do that cause inputs are even numbers.
				
				ADDS R2,R2,#1 // Increase the counter. So the value in R3 fits at least counter's times.
				SUBS R5,R5,R3 // Decrease the input value so i can do the comparison once again.
				CMP R5,R3 // Compare the decreased value with the "divided".
				BHS div_loop // Return to loop if the R5's value is higher or equal to R3's value.
								
				STR R2,[R6] // The division's result is stored in R6's address.
				
// calculate time
				
				MOVS R3,#0x05 // I store the value 5 to multiply it with the liters found before.
				MULS R2,R3,R2 // Multiply to find the total seconds for pump function.
				STR R2,[R6,#0x04] // The final value of seconds is stored 4 bytes next to total liters calculated.
				LDR R4,= 0x01FFFF404 // Load the value of pump's sensor.
				MOVS R1,#1 // Activate sensor.
				STR R1,[R4] // Where the activations value is stored.
				
countdown_loop // System countdowns the mseconds.		
				
				SUBS R5,R5,#1 // Decrease the input value so i can do the comparison once again. By one msec.
				CMP R5,#1 // Compare the decreased value with the "divided".
				BHS countdown_loop
				
				MOVS R1,#0 // Deactivate sensor.
				STR R1,[R4] // Where the de-activations value is stored.
				BX lr
}


int main(void)
{
	int a = 5, b = 10, c = 20, d = 50, e = 100;
	
	VenMachine(a);
	VenMachine(b);
	VenMachine(c);
	VenMachine(d);
	VenMachine(e);
	
	while (1)
			;
}
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   

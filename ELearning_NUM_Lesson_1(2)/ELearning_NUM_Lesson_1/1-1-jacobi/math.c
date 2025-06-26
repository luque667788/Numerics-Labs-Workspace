/*
 * JACOBI ITERATIVE METHOD FOR SOLVING LINEAR SYSTEMS
 * 
 * GENERAL OVERVIEW:
 * This program implements the Jacobi iterative method to solve a system of linear equations.
 * The Jacobi method is an iterative algorithm used to solve systems of linear equations of the form Ax = b.
 * 
 * MATHEMATICAL BACKGROUND:
 * - We have the system: 7x₁ - x₂ = 5
 *                       3x₁ - 5x₂ = -7
 * - The exact algebraic solution is (x₁, x₂) = (1, 2)
 * 
 * JACOBI METHOD PRINCIPLE:
 * - Split the coefficient matrix A into A = D + L + U where:
 *   D = diagonal elements, L = lower triangular, U = upper triangular
 * - Rearrange each equation to solve for one variable:
 *   x₁ = (5 + x₂) / 7     (from first equation)
 *   x₂ = (7 + 3x₁) / 5    (from second equation)
 * - Use previous iteration values to compute new values
 * - Continue until convergence (values stop changing significantly)
 * 
 * CONVERGENCE:
 * The method converges because the coefficient matrix is diagonally dominant.
 * Each iteration should get closer to the true solution (1, 2).
 */

#include <stdio.h>        // For printf() function
#include <stdlib.h>       // For EXIT_SUCCESS constant
#include <stdbool.h>      // For boolean type (true/false)

#include <time.h>         // For clock() function and time-related constants

/*
 * WAIT FUNCTION - Creates a time delay
 * 
 * PURPOSE: Pauses program execution for a specified duration
 * PARAMETER: time - duration to wait in seconds (double precision)
 * 
 * MATHEMATICAL/ALGORITHMIC EXPLANATION:
 * - Records current CPU clock ticks when function starts
 * - Calculates target number of ticks to wait (time × ticks_per_second)
 * - Continuously checks current clock until target time elapsed
 * - This creates a busy-wait loop (CPU keeps running but waits)
 */
static void WAIT(double time)
{
	int cnt = clock();                           // Get current CPU clock ticks
	int ticks = (int) (time*CLOCKS_PER_SEC);    // Convert seconds to clock ticks
	while (true)                                 // Infinite loop until time elapsed
	{
		if (clock() > (cnt + ticks))            // Check if enough time has passed
			return;                             // Exit function when time elapsed
	}
}

/*
 * MAIN FUNCTION - Implements Jacobi Method Iteration
 */
int main(void)
{
	// VARIABLE INITIALIZATION
	float x = 0.0;    // x₁ variable (first unknown) - starting guess = 0
	float y = 0.0;    // x₂ variable (second unknown) - starting guess = 0
	int n = 1;        // Iteration counter starting from 1
	
	// JACOBI ITERATION LOOP
	// Performs 9 iterations (n goes from 1 to 9)
	// Each iteration uses previous values to compute new approximations
	while (n < 10)
	{
		// DISPLAY CURRENT ITERATION RESULTS
		printf("xy(%2d) = (% 1.2f, % 1.2f)\n", n, x, y);
		// Format: iteration number, x-value with 2 decimals, y-value with 2 decimals
		// The % 1.2f format adds space for positive numbers to align with negative ones

		WAIT(1.0);    // Pause for 1 second to observe convergence visually

		// MATHEMATICAL SYSTEM BEING SOLVED:
		// Linear System: 7x₁ - x₂ = 5     ... (equation 1)
		//                3x₁ - 5x₂ = -7    ... (equation 2)
		// Algebraic Solution: (x₁, x₂) = (1, 2)
		// 
		// JACOBI METHOD FORMULAS:
		// From equation 1: 7x₁ - x₂ = 5  →  x₁ = (5 + x₂)/7
		// From equation 2: 3x₁ - 5x₂ = -7  →  x₂ = (7 + 3x₁)/5
		//
		// KEY PRINCIPLE: Use OLD values on right side to compute NEW values

		// COMPUTE NEW x₁ using OLD y (which represents x₂)
		float xn = (5.0 + y)/7.0;
		// Mathematical reasoning: From 7x₁ - x₂ = 5, solve for x₁
		// 7x₁ = 5 + x₂, therefore x₁ = (5 + x₂)/7

		// COMPUTE NEW x₂ using OLD x (which represents x₁)  
		float yn = (7.0 + 3.0*x)/5.0;
		// Mathematical reasoning: From 3x₁ - 5x₂ = -7, solve for x₂
		// -5x₂ = -7 - 3x₁, therefore 5x₂ = 7 + 3x₁, so x₂ = (7 + 3x₁)/5

		// UPDATE VARIABLES FOR NEXT ITERATION
		x = xn;    // Store new x₁ value
		y = yn;    // Store new x₂ value
		
		n++;       // Increment iteration counter
	}
	// MATHEMATICAL EXPECTATION:
	// As iterations continue, (x,y) should converge to (1,2)
	// This happens because the coefficient matrix is diagonally dominant

    return EXIT_SUCCESS;    // Program completed successfully
}

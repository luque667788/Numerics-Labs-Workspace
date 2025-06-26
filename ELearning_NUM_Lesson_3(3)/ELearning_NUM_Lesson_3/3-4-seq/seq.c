/*
 * MATHEMATICAL SEQUENCE GENERATOR
 * ===============================
 * 
 * GENERAL OVERVIEW:
 * This program generates and displays terms of the alternating harmonic series.
 * The mathematical formula being computed is:
 * 
 *     x_n = (-1)^(n+1) / n
 * 
 * This creates the sequence: 1, -1/2, 1/3, -1/4, 1/5, -1/6, ...
 * 
 * MATHEMATICAL SIGNIFICANCE:
 * - This is known as the alternating harmonic series
 * - It converges to ln(2) ≈ 0.693147...
 * - The series demonstrates alternating convergence behavior
 * - Each term gets smaller in absolute value as n increases
 * - The signs alternate between positive and negative
 * 
 * PROGRAM STRUCTURE:
 * 1. Define a structure to hold the sequence data
 * 2. Initialize the sequence array
 * 3. Calculate sequence terms using the mathematical formula
 * 4. Display the computed terms
 */

// filepath: /home/luque/Documents/UNI/sem3/numeriks/labs workspaces/ELearning_NUM_Lesson_3(3)/ELearning_NUM_Lesson_3/3-4-seq/seq.c

// Standard input/output library for printf function
#include <stdio.h>
// Standard library for EXIT_SUCCESS constant
#include <stdlib.h>
// Math library for pow() function to compute powers
#include <math.h>

// Define maximum number of sequence terms we can store
// Using 100 as a reasonable limit for this demonstration
#define SEQ_MAX		100

/*
 * Structure to represent our mathematical sequence
 * This encapsulates both the data (array of terms) and metadata (count)
 */
struct SEQ
{
	double x[SEQ_MAX+1];  // Array to store sequence terms (x[1] to x[SEQ_MAX])
	                      // Index 0 is unused to match mathematical notation (1-based indexing)
	int n;                // Current number of computed terms in the sequence
};

// Global instance of our sequence structure
// Using global scope for simplicity in this educational example
struct SEQ seq;

/*
 * SEQUENCE CALCULATION FUNCTION
 * =============================
 * 
 * Computes the first n terms of the alternating harmonic series:
 * x_k = (-1)^(k+1) / k  for k = 1, 2, 3, ..., n
 * 
 * MATHEMATICAL BREAKDOWN:
 * - (-1)^(k+1): Creates alternating signs
 *   * When k=1: (-1)^2 = +1 → positive term
 *   * When k=2: (-1)^3 = -1 → negative term  
 *   * When k=3: (-1)^4 = +1 → positive term
 *   * Pattern: +, -, +, -, +, ...
 * - Division by k: Creates decreasing magnitude (1, 1/2, 1/3, 1/4, ...)
 * 
 * CONVERGENCE BEHAVIOR:
 * As n → ∞, the partial sum approaches ln(2) ≈ 0.693147
 */
void calc(int n)
{
	// Safety check: ensure we don't exceed our array bounds
	// This prevents buffer overflow and maintains program stability
	if (n > SEQ_MAX)
		return;
	
	// Initialize sequence counter to 1 (mathematical sequences typically start at 1)
	seq.n = 1;

	// Set first term explicitly (though it will be overwritten in the loop)
	// This demonstrates the initialization pattern
	seq.x[seq.n] = 0.0;

	// Main calculation loop: compute each term of the sequence
	// Loop continues while we haven't calculated the requested number of terms
	while (seq.n < n+1)
	{
		/*
		 * CORE MATHEMATICAL COMPUTATION:
		 * x_n = (-1)^(n+1) / n
		 * 
		 * Breaking down this formula:
		 * 1. pow((-1.0), (seq.n + 1.0)): Computes (-1)^(n+1)
		 *    - Uses pow() function from math.h
		 *    - seq.n + 1.0 ensures alternating pattern starts positive
		 *    - Results in: +1, -1, +1, -1, ...
		 * 
		 * 2. Division by seq.n: Creates harmonic sequence denominators
		 *    - seq.n takes values: 1, 2, 3, 4, ...
		 *    - Results in fractions: 1/1, 1/2, 1/3, 1/4, ...
		 * 
		 * 3. Combined result: +1/1, -1/2, +1/3, -1/4, +1/5, ...
		 */
		seq.x[seq.n] = pow((-1.0), (seq.n + 1.0))/(seq.n);

		// Increment counter to move to next term
		// Parentheses around seq.n are unnecessary but emphasize the operation
		(seq.n)++;
	}
}

/*
 * INITIALIZATION FUNCTION
 * =======================
 * 
 * Prepares the sequence structure for fresh calculations by:
 * 1. Zeroing out all array elements (clean slate)
 * 2. Resetting the term counter
 * 
 * This is a good programming practice to ensure predictable behavior
 * and avoid using uninitialized memory values.
 */
void init(void)
{
	int n;  // Local loop counter variable

	// Zero-initialize the entire array
	// This loop covers all possible array positions to ensure clean state
	for (n=0; n < SEQ_MAX; n++)
		seq.x[n] = 0.0;  // Set each element to exactly zero
	
	// Reset the sequence length counter
	// Starting at 0 indicates no terms have been calculated yet
	seq.n = 0;
}

/*
 * MAIN PROGRAM EXECUTION
 * ======================
 * 
 * Demonstrates the complete workflow:
 * 1. Initialize the sequence structure
 * 2. Calculate the first 10 terms of the alternating harmonic series
 * 3. Display the results in a formatted table
 * 
 * EXPECTED OUTPUT:
 * x( 1) =  1.00    (= +1/1)
 * x( 2) = -0.50    (= -1/2)  
 * x( 3) =  0.33    (= +1/3)
 * x( 4) = -0.25    (= -1/4)
 * x( 5) =  0.20    (= +1/5)
 * ... and so on
 * 
 * MATHEMATICAL INSIGHT:
 * Notice how the terms alternate in sign and decrease in magnitude.
 * The partial sums of this series converge to ln(2).
 */
int main(void)
{
	// Step 1: Initialize the sequence data structure
	// This ensures we start with a clean, predictable state
	init();

	// Step 2: Calculate the first 10 terms of the alternating harmonic series
	// The number 10 is chosen as a reasonable demonstration size
	calc(10);

	// Step 3: Display the computed sequence terms
	int n;  // Loop counter for output display

	// Iterate through all computed terms (from 1 to seq.n-1)
	// Note: seq.n contains the count AFTER the last increment in calc()
	//       so we use seq.n-1 as the upper bound, or equivalently n < seq.n
	for (n=1; n < seq.n; n++)
	{
		/*
		 * FORMATTED OUTPUT EXPLANATION:
		 * printf("x(%2d) = % 1.2lf\n", n, seq.x[n]);
		 * 
		 * Format string breakdown:
		 * - "x(%2d)": Print "x(" followed by integer n with minimum 2 digits, then ")"
		 *   * %2d: Right-align integer in 2-character field
		 *   * Results in: "x( 1)", "x( 2)", "x(10)", etc.
		 * 
		 * - " = % 1.2lf": Print " = " followed by the floating-point value
		 *   * % 1.2lf: Space for positive numbers (% flag), 2 decimal places
		 *   * lf specifier for double precision
		 *   * Results in: " = 1.00", " = -0.50", etc.
		 * 
		 * - \n: Newline character for line separation
		 */
		printf("x(%2d) = % 1.2lf\n", n, seq.x[n]);
	}

	// Return success status to the operating system
	// EXIT_SUCCESS is more portable than returning 0
    return EXIT_SUCCESS;
}

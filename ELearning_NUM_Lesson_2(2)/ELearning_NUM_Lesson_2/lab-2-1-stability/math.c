
/*
===============================================================================
                    NUMERICAL STABILITY DEMONSTRATION PROGRAM
===============================================================================

MATHEMATICAL PURPOSE:
--------------------
This program demonstrates the critical concept of NUMERICAL STABILITY by 
exploring the iterative sequence: x_{n+1} = x_n² - 1

MATHEMATICAL THEORY:
-------------------
The iteration x_{n+1} = x_n² - 1 creates different behaviors depending on 
the starting value x₀:

1. FIXED POINTS: Values where x = x² - 1, solving x² - x - 1 = 0
   Solutions: x = (1 ± √5)/2 ≈ 1.618 (golden ratio φ) and ≈ -0.618

2. STABILITY ANALYSIS:
   - If |x₀| < φ: sequence converges to one of the fixed points
   - If |x₀| > φ: sequence diverges to infinity
   - If x₀ = ±φ: sequence remains at fixed point

3. STARTING VALUE x₀ = -1.5:
   Since |-1.5| = 1.5 < φ ≈ 1.618, we expect bounded behavior, but the
   specific dynamics depend on the iteration's mathematical properties.

NUMERICAL STABILITY CONCEPTS DEMONSTRATED:
------------------------------------------
1. ITERATION CONVERGENCE: How sequences behave over time
2. FIXED POINT DYNAMICS: Mathematical attractors and repellers  
3. SENSITIVITY TO INITIAL CONDITIONS: Small changes can have big effects
4. FLOATING POINT PRECISION: How computer arithmetic affects results
5. COMPUTATIONAL LIMITS: When mathematical theory meets reality

EDUCATIONAL OBJECTIVES:
----------------------
- Understand iterative mathematical processes
- Observe numerical convergence/divergence patterns
- Learn about stability in computational mathematics
- See how simple formulas can create complex behavior
- Explore the relationship between theory and computation

===============================================================================
*/

// Standard C library headers for input/output and system functions
#include <stdio.h>      // For printf() - formatted output to console
#include <stdlib.h>     // For EXIT_SUCCESS - standard program termination

/*
===============================================================================
                              MAIN PROGRAM
===============================================================================
This program implements and observes the iterative sequence x_{n+1} = x_n² - 1
starting from x₀ = -1.5, demonstrating numerical stability concepts.

MATHEMATICAL SEQUENCE ANALYSIS:
------------------------------
Starting with x₀ = -1.5, let's predict the first few terms:

x₁ = (-1.5)² - 1 = 2.25 - 1 = 1.25
x₂ = (1.25)² - 1 = 1.5625 - 1 = 0.5625  
x₃ = (0.5625)² - 1 = 0.31640625 - 1 = -0.68359375
x₄ = (-0.68359375)² - 1 = 0.4673 - 1 = -0.5327
...

EXPECTED BEHAVIOR:
-----------------
Since |-1.5| < φ ≈ 1.618, the sequence should remain bounded and potentially
converge to one of the fixed points or exhibit oscillatory behavior.

NUMERICAL CONSIDERATIONS:
------------------------
- Using 'float' (32-bit) provides ~7 decimal digits of precision
- Accumulated rounding errors may affect long-term behavior
- Computer arithmetic differs from exact mathematical computation
===============================================================================
*/
int main(void)
{
	// MATHEMATICAL VARIABLES:
	// x: Current term in the sequence x_n
	// Starting value x₀ = -1.5 chosen to demonstrate bounded iteration
	// Mathematical significance: |-1.5| < golden ratio φ ≈ 1.618
	// Prediction: sequence should remain bounded (not diverge to infinity)
	float x = -1.5;
	
	// n: Iteration counter (term index in the sequence)
	// We'll compute terms x₁, x₂, x₃, ..., x₉ (9 iterations total)
	// Mathematical notation: n represents the subscript in x_n
	int n = 1;
	
	// MAIN ITERATION LOOP: Compute and display sequence terms
	// Mathematical process: Generate x_n for n = 1, 2, 3, ..., 9
	// Stopping condition: n < 10 means we compute 9 terms of the sequence
	while (n < 10)
	{
		// DISPLAY CURRENT TERM:
		// Mathematical output: Show x_n before computing x_{n+1}
		// Format: "x( n) = value" with 2-digit field width for n
		// "% 1.2f" means: space for positive, 1 digit before decimal, 2 after
		// This creates neat columnar output for easy pattern observation
		printf("x(%2d) = % 1.2f\n", n, x);

		// CORE MATHEMATICAL ITERATION:
		// Implement the recurrence relation: x_{n+1} = x_n² - 1
		// Mathematical breakdown:
		//   1. x*x: Compute x_n²  (quadratic term)
		//   2. - 1.0: Subtract constant term
		//   3. Store result as new x (becomes x_{n+1})
		// 
		// NUMERICAL ANALYSIS:
		// - Squaring amplifies values > 1 and compresses values < 1
		// - Subtracting 1 shifts the entire function downward
		// - This creates complex dynamics around the fixed points
		x = x*x - 1.0;
		
		// INCREMENT COUNTER:
		// Move to next term in sequence: n → n+1
		// Mathematical progression: x_n → x_{n+1}
		// Loop control: ensures we compute exactly 9 iterations
		n++;
	}
	
	/*
	MATHEMATICAL OBSERVATIONS TO MAKE:
	---------------------------------
	When you run this program, observe:
	
	1. CONVERGENCE PATTERN: Does the sequence approach a fixed value?
	2. OSCILLATION: Does it alternate between values?
	3. BOUNDED BEHAVIOR: Do values stay within a reasonable range?
	4. NUMERICAL PRECISION: How do rounding errors accumulate?
	
	EXPECTED INSIGHTS:
	-----------------
	- The sequence may converge to a fixed point near -0.618
	- Or it might oscillate between two values
	- Floating-point errors may cause slight variations from exact math
	- This demonstrates how simple iterations can create complex patterns
	
	STABILITY IMPLICATIONS:
	----------------------
	- Small changes in x₀ might lead to different long-term behavior
	- Computer precision limits affect the accuracy of results
	- Understanding these effects is crucial in numerical mathematics
	
	MATHEMATICAL CONNECTION:
	-----------------------
	This iteration is related to:
	- Dynamical systems theory
	- Chaos theory (though this particular case is stable)
	- Fixed point analysis
	- The mathematics behind fractals (Julia sets)
	*/

	// PROGRAM TERMINATION:
	// Return success status to operating system
	// Mathematical completion: We've successfully demonstrated the
	// iterative sequence and its numerical stability properties
    return EXIT_SUCCESS;
}

/*
===============================================================================
                         EXTENDED MATHEMATICAL ANALYSIS
===============================================================================

THEORETICAL BACKGROUND:
----------------------
The iteration x_{n+1} = x_n² - 1 is a simplified version of iterations used
in fractal geometry and chaos theory. It's related to:

1. JULIA SETS: Complex version z_{n+1} = z_n² + c (with c = -1)
2. MANDELBROT SET: When c varies, this creates the famous fractal
3. DYNAMICAL SYSTEMS: Study of how systems evolve over time

FIXED POINT ANALYSIS:
--------------------
To find fixed points, solve: x = x² - 1
Rearranging: x² - x - 1 = 0
Using quadratic formula: x = (1 ± √5)/2

Fixed points:
- x₁ = (1 + √5)/2 ≈ 1.618 (golden ratio φ)
- x₂ = (1 - √5)/2 ≈ -0.618 (negative reciprocal of φ)

STABILITY OF FIXED POINTS:
-------------------------
The derivative f'(x) = 2x determines stability:
- At x₁ ≈ 1.618: f'(x₁) ≈ 3.236 > 1 (unstable)
- At x₂ ≈ -0.618: f'(x₂) ≈ -1.236, |f'(x₂)| > 1 (unstable)

Both fixed points are unstable, but the iteration can still be bounded!

NUMERICAL STABILITY LESSONS:
---------------------------
1. Mathematical stability ≠ Numerical stability
2. Computer arithmetic introduces small errors
3. These errors can accumulate over many iterations
4. Understanding both theory and computation is essential

EXTENSIONS TO EXPLORE:
---------------------
1. Try different starting values (x₀)
2. Use double precision instead of float
3. Increase the number of iterations
4. Plot the sequence graphically
5. Explore the complex version for Julia sets

This simple program opens the door to rich mathematical territory!
===============================================================================
*/

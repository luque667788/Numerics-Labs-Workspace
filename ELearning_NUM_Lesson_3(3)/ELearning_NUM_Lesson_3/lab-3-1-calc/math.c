/*
 * POLYNOMIAL INTERPOLATION BARYCENTRIC WEIGHTS CALCULATOR
 * =======================================================
 * 
 * GENERAL OVERVIEW:
 * This program demonstrates the computation of barycentric weights for polynomial interpolation
 * using equispaced nodes. It combines several mathematical concepts:
 * 
 * 1. **Equispaced Node Generation**: Creates uniformly distributed points in [-1, 1]
 * 2. **Barycentric Weight Calculation**: Computes weights for Lagrange interpolation
 * 3. **Function Evaluation**: Samples the Runge function at interpolation nodes
 * 4. **Binomial Coefficients**: Used in the weight formula
 * 
 * MATHEMATICAL SIGNIFICANCE:
 * =========================
 * 
 * **BARYCENTRIC INTERPOLATION FORMULA:**
 * For a polynomial interpolation through points (x₀,y₀), (x₁,y₁), ..., (xₙ,yₙ):
 * 
 *     P(x) = Σᵢ₌₀ⁿ wᵢ × yᵢ / (x - xᵢ) / Σⱼ₌₀ⁿ wⱼ / (x - xⱼ)
 * 
 * **EQUISPACED BARYCENTRIC WEIGHTS:**
 * For equispaced nodes, the weights are: wₖ = (-1)ᵏ × C(n,k)
 * Where C(n,k) is the binomial coefficient "n choose k"
 * 
 * **THE RUNGE FUNCTION:**
 * f(x) = 1/(1 + 16x²)
 * This is a classic test function that demonstrates the Runge phenomenon:
 * - Well-behaved function (smooth, bounded)
 * - Causes oscillations when interpolated with equispaced nodes
 * - Used to illustrate why Chebyshev nodes are often preferred
 * 
 * **RUNGE PHENOMENON:**
 * When using equispaced nodes for high-degree polynomial interpolation,
 * the interpolating polynomial often exhibits large oscillations near
 * the boundaries, even though the function being interpolated is smooth.
 * 
 * PRACTICAL APPLICATIONS:
 * ======================
 * - Numerical interpolation and approximation
 * - Function reconstruction from discrete samples
 * - Spectral methods in numerical analysis
 * - Computer graphics and curve fitting
 * - Signal processing and data interpolation
 * 
 * PROGRAM STRUCTURE:
 * ==================
 * 1. factorial() - Computes factorials for binomial coefficients
 * 2. binomial() - Calculates C(n,k) for barycentric weights
 * 3. f() - Defines the Runge test function
 * 4. main() - Orchestrates node generation, weight calculation, and function sampling
 */

// filepath: /home/luque/Documents/UNI/sem3/numeriks/labs workspaces/ELearning_NUM_Lesson_3(3)/ELearning_NUM_Lesson_3/lab-3-1-calc/math.c

// Standard input/output library for printf function
#include <stdio.h>
// Standard library for EXIT_SUCCESS constant
#include <stdlib.h>
// Math library for pow() function (used in both weights and Runge function)
#include <math.h>

/*
 * FACTORIAL FUNCTION (SAME AS PREVIOUS IMPLEMENTATIONS)
 * ===================================================
 * 
 * This function is identical to the enhanced version from the binomial-fixed program.
 * It computes n! with long long precision to avoid overflow issues.
 * 
 * See previous implementations for detailed mathematical and programming explanations.
 */
static long long factorial(int n)
{
    // Enhanced precision factorial calculation (see previous implementations for details)
    long long res = 1;
    int k;
    for (k=2; k<=n; k++)
        res *= k;
    return res;
}

/*
 * BINOMIAL COEFFICIENT FUNCTION (SAME AS ENHANCED VERSION)
 * =======================================================
 * 
 * Computes C(n,k) = n!/(k!(n-k)!) for use in barycentric weight calculation.
 * This is the same enhanced implementation from the binomial-fixed program.
 * 
 * In the context of this interpolation program:
 * - Used to calculate barycentric weights: wₖ = (-1)ᵏ × C(n,k)
 * - Essential for the equispaced interpolation formula
 * - Provides the combinatorial foundation for Lagrange interpolation weights
 */
static long long binomial(int n, int k)
{
    // Same enhanced implementation with intermediate variables for debugging
    long long a = factorial(n);
    long long b = factorial(k);
    long long c = factorial(n-k);
    long long res = a / b / c;
    return res;
}

/*
 * RUNGE FUNCTION DEFINITION
 * =========================
 * 
 * Defines the classic Runge function: f(x) = 1/(1 + 16x²)
 * 
 * MATHEMATICAL PROPERTIES:
 * - Domain: All real numbers (no singularities)
 * - Range: (0, 1] (always positive, maximum value is 1 at x=0)
 * - Symmetry: Even function, f(-x) = f(x)
 * - Behavior: Bell-shaped curve, rapidly decreasing away from x=0
 * 
 * SIGNIFICANCE IN INTERPOLATION:
 * - Smooth and well-behaved function (infinitely differentiable)
 * - Despite its smoothness, causes severe oscillations when interpolated
 *   with high-degree polynomials using equispaced nodes
 * - Classic example of the Runge phenomenon
 * - Demonstrates why equispaced nodes can be problematic for interpolation
 * 
 * FUNCTION VALUES AT KEY POINTS:
 * - f(0) = 1/(1 + 0) = 1.0 (maximum value)
 * - f(±0.25) = 1/(1 + 16×0.0625) = 1/2 = 0.5
 * - f(±0.5) = 1/(1 + 16×0.25) = 1/5 = 0.2
 * - f(±1) = 1/(1 + 16) = 1/17 ≈ 0.059
 * 
 * WHY THIS FUNCTION IS CHOSEN:
 * - Simple to evaluate
 * - Well-known behavior in numerical analysis literature
 * - Perfect test case for demonstrating interpolation challenges
 * - Motivates the use of Chebyshev nodes instead of equispaced nodes
 */
static double f(double x)
{
	/*
	 * RUNGE FUNCTION CALCULATION
	 * ==========================
	 * 
	 * Mathematical formula: f(x) = 1 / (1 + 16x²)
	 * 
	 * Implementation breakdown:
	 * - 16.0*x*x: Computes 16x² (the quadratic term)
	 * - 1.0 + ...: Adds 1 to create the denominator (1 + 16x²)
	 * - 1.0/(...): Takes reciprocal to complete the fraction
	 * 
	 * Numerical considerations:
	 * - Uses double precision for accuracy
	 * - No risk of division by zero (denominator always ≥ 1)
	 * - Numerically stable for all finite x values
	 */
	return 1.0/(1.0 + 16.0*x*x);
}

/*
 * MAIN PROGRAM: INTERPOLATION SETUP AND DEMONSTRATION
 * ===================================================
 * 
 * This program demonstrates the complete setup for polynomial interpolation
 * using equispaced nodes and barycentric weights. It performs four main tasks:
 * 
 * 1. **EQUISPACED NODE GENERATION**: Creates n+1 uniformly distributed points in [-1,1]
 * 2. **BARYCENTRIC WEIGHT CALCULATION**: Computes weights for interpolation formula
 * 3. **FUNCTION SAMPLING**: Evaluates the Runge function at all nodes
 * 4. **RESULT DISPLAY**: Shows nodes, weights, and function values in tabular format
 * 
 * MATHEMATICAL PROCESS OVERVIEW:
 * =============================
 * 
 * For polynomial interpolation, we need:
 * - Interpolation nodes: x₀, x₁, ..., xₙ (where to sample the function)
 * - Function values: f₀, f₁, ..., fₙ (what the function equals at each node)
 * - Barycentric weights: w₀, w₁, ..., wₙ (coefficients for interpolation formula)
 * 
 * The resulting interpolating polynomial P(x) satisfies:
 * P(xᵢ) = fᵢ for all i = 0, 1, ..., n
 * 
 * EQUISPACED vs CHEBYSHEV NODES:
 * ==============================
 * This program uses equispaced nodes, which are simple but problematic:
 * - Equispaced: -1, -0.875, -0.75, ..., 0.75, 0.875, 1 (for n=15)
 * - Problem: Leads to Runge phenomenon (oscillations at boundaries)
 * - Alternative: Chebyshev nodes minimize interpolation error
 * 
 * PROGRAM OUTPUT EXPLANATION:
 * ===========================
 * The program will display three rows:
 * Row 1: Interpolation nodes (x-coordinates where function is sampled)
 * Row 2: Barycentric weights (coefficients for interpolation formula)
 * Row 3: Function values (f(x) evaluated at each node)
 */
int main(void)
{
	// Set the degree of interpolation polynomial
	// n=15 means we'll have 16 nodes (indices 0 through 15)
	// This creates a polynomial of degree 15
	int n = 15;
	
	// Loop counter variable used throughout the program
	int k;
	
	/*
	 * STEP 1: EQUISPACED NODE GENERATION
	 * ==================================
	 * 
	 * Goal: Create n+1 equally spaced points in the interval [-1, 1]
	 * 
	 * Mathematical formula for equispaced nodes:
	 * xₖ = -1 + 2k/n  for k = 0, 1, 2, ..., n
	 * 
	 * This creates the sequence:
	 * x₀ = -1, x₁ = -1+2/n, x₂ = -1+4/n, ..., xₙ = -1+2n/n = 1
	 * 
	 * For n=15, this gives 16 points:
	 * -1.00, -0.87, -0.73, -0.60, -0.47, -0.33, -0.20, -0.07,
	 *  0.07,  0.20,  0.33,  0.47,  0.60,  0.73,  0.87,  1.00
	 */
	// EquispacedNodes
	double xnodes[n+1];  // Array to store interpolation nodes
	
	// First loop: Initialize with integer indices 0, 1, 2, ..., n
	for (k=0; k<=n; k++) xnodes[k] = k;
	
	// Second loop: Transform indices to equispaced points in [-1, 1]
	// Mathematical transformation: k → -1 + 2k/n
	for (k=0; k<=n; k++) xnodes[k] = -1.0+xnodes[k]*2.0/n;
	
	/*
	 * STEP 2: BARYCENTRIC WEIGHT CALCULATION
	 * ======================================
	 * 
	 * Goal: Compute weights for the barycentric interpolation formula
	 * 
	 * Mathematical formula for equispaced barycentric weights:
	 * wₖ = (-1)ᵏ × C(n,k)
	 * 
	 * Where:
	 * - (-1)ᵏ creates alternating signs: +, -, +, -, ...
	 * - C(n,k) is the binomial coefficient "n choose k"
	 * 
	 * THEORETICAL BACKGROUND:
	 * These weights are derived from the Lagrange interpolation formula.
	 * For equispaced nodes, the weights have this specific combinatorial form.
	 * 
	 * EXAMPLE CALCULATION (n=15, k=8):
	 * w₈ = (-1)⁸ × C(15,8) = (+1) × 6435 = 6435
	 * 
	 * SIGN PATTERN:
	 * The weights alternate in sign, starting positive:
	 * w₀ > 0, w₁ < 0, w₂ > 0, w₃ < 0, ...
	 */
	// EquispacedBarWeights  
	double w[n+1];  // Array to store barycentric weights
	
	for (k=0; k<=n; k++)
    	w[k] = pow(-1.0, k) * binomial(n, k);
	
	/*
	 * STEP 3: FUNCTION SAMPLING
	 * =========================
	 * 
	 * Goal: Evaluate the Runge function at each interpolation node
	 * 
	 * This creates the function values f₀, f₁, ..., fₙ where fₖ = f(xₖ)
	 * These values, combined with the nodes and weights, completely
	 * determine the interpolating polynomial.
	 * 
	 * MATHEMATICAL SIGNIFICANCE:
	 * The sampled values represent the "target" that our interpolating
	 * polynomial must match. The polynomial P(x) will satisfy:
	 * P(xₖ) = f(xₖ) = f_sample[k] for all k
	 * 
	 * RUNGE FUNCTION BEHAVIOR:
	 * Since f(x) = 1/(1+16x²), the values will be:
	 * - Largest at x=0 (center): f(0) = 1.0
	 * - Smallest at x=±1 (endpoints): f(±1) ≈ 0.059
	 * - Symmetric pattern due to even function property
	 */
	double f_sample[n+1];  // Array to store function values at nodes
	
	for (k=0; k<=n; k++)
		f_sample[k] = f(xnodes[k]);

	/*
	 * STEP 4: RESULTS DISPLAY
	 * =======================
	 * 
	 * Display the computed data in three rows for easy analysis:
	 * 
	 * Row 1: Interpolation nodes (x-coordinates)
	 * Row 2: Barycentric weights (interpolation coefficients)  
	 * Row 3: Function values (sampled Runge function)
	 * 
	 * OUTPUT FORMAT:
	 * - %2.2lf: Display numbers with 2 decimal places, minimum width 2
	 * - Space separation for readability
	 * - Each row on separate line
	 * 
	 * INTERPRETING THE OUTPUT:
	 * - Look for symmetry in nodes and function values
	 * - Notice alternating signs in the weights
	 * - Observe how function values decrease toward boundaries
	 * - This data completely specifies the interpolating polynomial
	 */
	
	// Display interpolation nodes
	for (k=0; k<=n; k++)
		printf("%2.2lf ", xnodes[k]);
	printf("\n");
	
	// Display barycentric weights
	for (k=0; k<=n; k++)
		printf("%2.2lf ", w[k]);
	printf("\n");
	
	// Display function values
	for (k=0; k<=n; k++)
		printf("%2.2lf ", f_sample[k]);
	printf("\n");

	// Return success status
    return EXIT_SUCCESS;
}

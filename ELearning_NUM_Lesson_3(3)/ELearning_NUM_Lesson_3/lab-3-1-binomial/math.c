/*
 * BINOMIAL COEFFICIENT CALCULATOR
 * ===============================
 * 
 * GENERAL OVERVIEW:
 * This program calculates binomial coefficients, also known as "n choose k" or C(n,k).
 * The mathematical formula being computed is:
 * 
 *     C(n,k) = n! / (k! × (n-k)!)
 * 
 * Also written as: (n k) or nCk
 * 
 * MATHEMATICAL SIGNIFICANCE:
 * - Binomial coefficients count the number of ways to choose k items from n items
 * - They appear in the binomial theorem: (x+y)^n = Σ C(n,k) × x^(n-k) × y^k
 * - Used extensively in combinatorics, probability theory, and statistics
 * - They form Pascal's triangle when arranged in rows
 * - Symmetric property: C(n,k) = C(n,n-k)
 * 
 * REAL-WORLD APPLICATIONS:
 * - Probability calculations (e.g., coin flips, lottery odds)
 * - Combinatorial optimization problems
 * - Polynomial expansions in algebra
 * - Statistical distributions (binomial, hypergeometric)
 * - Computer science algorithms (dynamic programming)
 * 
 * EXAMPLE CALCULATION:
 * C(15,8) = 15! / (8! × 7!) = 6435
 * This means there are 6435 ways to choose 8 items from 15 items.
 * 
 * PROGRAM STRUCTURE:
 * 1. factorial() - Computes n! (n factorial)
 * 2. binomial() - Computes C(n,k) using factorials
 * 3. main() - Demonstrates the calculation with specific values
 */

// filepath: /home/luque/Documents/UNI/sem3/numeriks/labs workspaces/ELearning_NUM_Lesson_3(3)/ELearning_NUM_Lesson_3/lab-3-1-binomial/math.c

// Standard input/output library for printf function
#include <stdio.h>
// Standard library for EXIT_SUCCESS constant
#include <stdlib.h>

/*
 * FACTORIAL FUNCTION
 * ==================
 * 
 * Computes the factorial of a non-negative integer n.
 * 
 * MATHEMATICAL DEFINITION:
 * n! = n × (n-1) × (n-2) × ... × 2 × 1
 * 
 * Special cases:
 * - 0! = 1 (by mathematical convention)
 * - 1! = 1
 * 
 * EXAMPLES:
 * - 5! = 5 × 4 × 3 × 2 × 1 = 120
 * - 8! = 8 × 7 × 6 × 5 × 4 × 3 × 2 × 1 = 40,320
 * 
 * ALGORITHM:
 * Uses iterative approach starting from 2 (since multiplying by 1 is redundant)
 * and building up the product incrementally.
 * 
 * NOTE: This implementation uses 'static' keyword, meaning the function
 * is only visible within this source file (internal linkage).
 * 
 * LIMITATIONS:
 * - Integer overflow can occur for large values of n
 * - Maximum safe value depends on system (typically n ≤ 12 for 32-bit int)
 */
static int factorial(int n)

static int factorial(int n)
{
    // Initialize result accumulator to 1
    // Starting with 1 is crucial because:
    // 1. It handles the base case (0! = 1, 1! = 1)
    // 2. Multiplication identity: anything × 1 = anything
    int res = 1;
    
    // Loop counter variable
    int k;
    
    /*
     * ITERATIVE FACTORIAL CALCULATION
     * ===============================
     * 
     * Mathematical process: res = 1 × 2 × 3 × ... × n
     * 
     * Loop explanation:
     * - Starts at k=2 (since 1×1 = 1, we can skip multiplying by 1)
     * - Continues while k ≤ n
     * - Each iteration multiplies current result by k
     * - k increments by 1 each time
     * 
     * Step-by-step example for n=5:
     * Initial: res = 1
     * k=2: res = 1 × 2 = 2
     * k=3: res = 2 × 3 = 6  
     * k=4: res = 6 × 4 = 24
     * k=5: res = 24 × 5 = 120
     * Final: 5! = 120
     */
    for (k=2; k<=n; k++)
        res *= k;  // Equivalent to: res = res * k
    
    // Return the computed factorial value
    return res;
}

/*
 * BINOMIAL COEFFICIENT FUNCTION
 * =============================
 * 
 * Computes the binomial coefficient C(n,k) = "n choose k"
 * 
 * MATHEMATICAL FORMULA:
 * C(n,k) = n! / (k! × (n-k)!)
 * 
 * COMBINATORIAL INTERPRETATION:
 * - Represents the number of ways to choose k objects from n objects
 * - Order doesn't matter (combinations, not permutations)
 * - Also called "combinations" in probability theory
 * 
 * MATHEMATICAL PROPERTIES:
 * - Symmetry: C(n,k) = C(n,n-k)
 * - Pascal's identity: C(n,k) = C(n-1,k-1) + C(n-1,k)
 * - Boundary conditions: C(n,0) = C(n,n) = 1
 * - Sum property: Σ C(n,k) for k=0 to n equals 2^n
 * 
 * EXAMPLES:
 * - C(4,2) = 4!/(2!×2!) = 24/(2×2) = 6
 *   (Ways to choose 2 items from {A,B,C,D}: AB, AC, AD, BC, BD, CD)
 * - C(15,8) = 15!/(8!×7!) = 6435
 * 
 * ALGORITHM IMPLEMENTATION:
 * This uses the straightforward factorial-based approach.
 * Note: More efficient algorithms exist for large values to avoid overflow.
 */
static int binomial(int n, int k)
{
    /*
     * BINOMIAL COEFFICIENT CALCULATION
     * ================================
     * 
     * Mathematical breakdown:
     * C(n,k) = n! / (k! × (n-k)!)
     * 
     * The expression is computed as:
     * factorial(n) / factorial(k) / factorial(n-k)
     * 
     * Division order explanation:
     * - First: factorial(n) / factorial(k) 
     * - Then: result / factorial(n-k)
     * - This is mathematically equivalent to: factorial(n) / (factorial(k) * factorial(n-k))
     * 
     * Example for C(15,8):
     * - n! = 15! = 1,307,674,368,000
     * - k! = 8! = 40,320
     * - (n-k)! = 7! = 5,040
     * - Result = 1,307,674,368,000 / 40,320 / 5,040 = 6,435
     * 
     * POTENTIAL ISSUES:
     * - Integer overflow for large factorials
     * - Loss of precision due to large intermediate values
     * - Inefficient for large n (calculates full factorials)
     */
    return factorial(n) / factorial(k) / factorial(n-k);
}

/*
 * VERIFICATION COMMENT
 * ===================
 * 
 * Expected result for the demonstration case:
 * C(15,8) should equal 6435
 * 
 * Mathematical verification:
 * C(15,8) = 15! / (8! × 7!)
 *         = 1,307,674,368,000 / (40,320 × 5,040)
 *         = 1,307,674,368,000 / 203,212,800
 *         = 6,435 ✓
 * 
 * Alternative verification using symmetry:
 * C(15,8) = C(15,7) = 6,435 ✓
 */
// binomial(15, 8) = 6435

/*
 * MAIN PROGRAM EXECUTION AND DEMONSTRATION
 * ========================================
 * 
 * Demonstrates the binomial coefficient calculation with a concrete example.
 * This program answers the question: "In how many ways can we choose 8 items from 15 items?"
 * 
 * REAL-WORLD SCENARIO:
 * Imagine you have 15 students and need to select 8 for a committee.
 * The binomial coefficient tells you there are 6,435 different ways to make this selection.
 * 
 * MATHEMATICAL CONTEXT:
 * - This is a classic combinatorics problem
 * - The result appears in row 15 of Pascal's triangle at position 8
 * - It's also the coefficient of x^8×y^7 in the expansion of (x+y)^15
 */
int main(void)
{
    // Define the parameters for our binomial coefficient calculation
    // n = total number of items to choose from
    int n = 15;
    
    // k = number of items to choose
    int k = 8;

    /*
     * FORMATTED OUTPUT AND FUNCTION CALL
     * ==================================
     * 
     * Display the binomial coefficient using mathematical notation.
     * 
     * printf format explanation:
     * - "(%i %i)": Shows the binomial coefficient in the format (n k)
     *   * %i: Integer format specifier
     *   * This represents the mathematical notation for "n choose k"
     * 
     * - " = %i": Shows the equals sign and the calculated result
     *   * %i: Integer format for the binomial coefficient value
     * 
     * - \n: Newline character for proper output formatting
     * 
     * Expected output: (15 8) = 6435
     * 
     * This output format mimics standard mathematical notation for binomial coefficients.
     */
    printf("(%i %i) = %i\n", n, k, binomial(n, k));

    // Return success status to the operating system
    // EXIT_SUCCESS indicates the program completed without errors
    return EXIT_SUCCESS;
}

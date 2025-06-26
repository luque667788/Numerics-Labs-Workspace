/*
 * BINOMIAL COEFFICIENT CALCULATOR - IMPROVED VERSION
 * ==================================================
 * 
 * GENERAL OVERVIEW:
 * This is an enhanced version of the binomial coefficient calculator that addresses
 * potential overflow issues present in the original implementation.
 * The mathematical formula being computed remains the same:
 * 
 *     C(n,k) = n! / (k! × (n-k)!)
 * 
 * MAJOR IMPROVEMENTS FROM PREVIOUS VERSION:
 * =========================================
 * 
 * 1. **DATA TYPE UPGRADE:**
 *    - Previous: Used 'int' (typically 32-bit, range ±2.1 billion)
 *    - Current: Uses 'long long' (typically 64-bit, range ±9.2 × 10^18)
 *    - Benefit: Can handle much larger factorial values without overflow
 * 
 * 2. **INTERMEDIATE VARIABLE STORAGE:**
 *    - Previous: Direct calculation in return statement
 *    - Current: Stores intermediate factorial results in separate variables
 *    - Benefit: Better debugging capability and clearer program flow
 * 
 * 3. **NUMERICAL STABILITY:**
 *    - Previous: Risk of integer overflow for n ≥ 13
 *    - Current: Can safely handle larger values (approximately n ≤ 20)
 * 
 * MATHEMATICAL SIGNIFICANCE (UNCHANGED):
 * - Binomial coefficients count combinations ("n choose k")
 * - Used in probability, statistics, and combinatorics
 * - Forms Pascal's triangle
 * - Appears in binomial theorem expansions
 * 
 * REMAINING LIMITATIONS:
 * - Still susceptible to overflow for very large n
 * - Inefficient algorithm (calculates full factorials)
 * - Better algorithms exist: Pascal's recurrence, multiplicative approach
 * 
 * PROGRAM STRUCTURE:
 * 1. factorial() - Enhanced with long long return type
 * 2. binomial() - Improved with intermediate variable storage
 * 3. main() - Demonstration with the same test case
 */

// filepath: /home/luque/Documents/UNI/sem3/numeriks/labs workspaces/ELearning_NUM_Lesson_3(3)/ELearning_NUM_Lesson_3/lab-3-1-binomial-fixed/math.c

// Standard input/output library for printf function
#include <stdio.h>
// Standard library for EXIT_SUCCESS constant
#include <stdlib.h>

/*
 * ENHANCED FACTORIAL FUNCTION
 * ===========================
 * 
 * Computes the factorial of a non-negative integer n with improved precision.
 * 
 * KEY IMPROVEMENT: LONG LONG RETURN TYPE
 * =====================================
 * 
 * Data type comparison:
 * - int (previous):     32-bit signed, range: -2,147,483,648 to 2,147,483,647
 * - long long (current): 64-bit signed, range: -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807
 * 
 * Practical impact:
 * - Previous version: 12! = 479,001,600 (safe), 13! = 6,227,020,800 (overflow!)
 * - Current version: Can safely compute up to approximately 20! = 2,432,902,008,176,640,000
 * 
 * MATHEMATICAL DEFINITION (UNCHANGED):
 * n! = n × (n-1) × (n-2) × ... × 2 × 1
 * 
 * ALGORITHM (UNCHANGED):
 * Still uses the same efficient iterative approach.
 */
static long long factorial(int n)
{
    // Initialize result accumulator with enhanced precision
    // CHANGE: 'int res = 1' → 'long long res = 1'
    // This allows storing much larger factorial values without overflow
    long long res = 1;
    
    // Loop counter remains int (sufficient for reasonable values of n)
    int k;
    
    /*
     * ITERATIVE FACTORIAL CALCULATION (ALGORITHM UNCHANGED)
     * ===================================================== 
     * 
     * The core algorithm remains identical to the previous version:
     * - Start at k=2 (optimization: skip multiplying by 1)
     * - Continue while k ≤ n
     * - Multiply accumulator by current k value
     * 
     * PRECISION IMPROVEMENT:
     * The only change is that 'res' can now store much larger values.
     * 
     * Comparison of maximum safe values:
     * - Previous (int):      12! = 479,001,600 ✓, 13! = overflow ✗
     * - Current (long long): 20! = 2,432,902,008,176,640,000 ✓, 21! = overflow ✗
     * 
     * Example calculation for n=15 (our test case):
     * 15! = 1,307,674,368,000 (safely fits in long long)
     */
    for (k=2; k<=n; k++)
        res *= k;  // Now performs long long arithmetic instead of int arithmetic
    
    // Return the computed factorial with enhanced precision
    return res;
}

/*
 * ENHANCED BINOMIAL COEFFICIENT FUNCTION
 * ======================================
 * 
 * Computes C(n,k) with improved precision and better debugging capability.
 * 
 * MAJOR IMPROVEMENTS FROM PREVIOUS VERSION:
 * ========================================
 * 
 * 1. **RETURN TYPE UPGRADE:**
 *    - Previous: static int binomial(int n, int k)
 *    - Current:  static long long binomial(int n, int k)
 *    - Benefit: Can return larger binomial coefficient values
 * 
 * 2. **INTERMEDIATE VARIABLE STORAGE:**
 *    - Previous: return factorial(n) / factorial(k) / factorial(n-k);
 *    - Current:  Separate variables for each factorial calculation
 *    - Benefits: 
 *      * Easier debugging (can inspect individual factorial values)
 *      * More readable code structure
 *      * Potential for future optimizations
 * 
 * 3. **NUMERICAL PRECISION:**
 *    - All arithmetic now performed in long long precision
 *    - Reduced risk of intermediate overflow during calculation
 * 
 * MATHEMATICAL FORMULA (UNCHANGED):
 * C(n,k) = n! / (k! × (n-k)!)
 * 
 * ALGORITHM STRUCTURE (IMPROVED):
 * The calculation is now broken down into clear, debuggable steps.
 */
static long long binomial(int n, int k)
{
    /*
     * STEP-BY-STEP CALCULATION WITH INTERMEDIATE STORAGE
     * =================================================
     * 
     * Breaking down the calculation into separate steps provides several advantages:
     * 1. Debugging: Can examine each factorial value individually
     * 2. Clarity: Makes the algorithm structure more explicit
     * 3. Maintenance: Easier to modify or optimize individual parts
     * 
     * Mathematical breakdown for C(15,8):
     * - a = 15! = 1,307,674,368,000
     * - b = 8!  = 40,320
     * - c = 7!  = 5,040
     * - res = a / b / c = 1,307,674,368,000 / 40,320 / 5,040 = 6,435
     */
    
    // Calculate n! with enhanced precision
    // IMPROVEMENT: Result stored in long long instead of computed inline
    long long a = factorial(n);
    
    // Calculate k! with enhanced precision  
    // IMPROVEMENT: Separate variable allows for debugging and inspection
    long long b = factorial(k);
    
    // Calculate (n-k)! with enhanced precision
    // IMPROVEMENT: Clear variable naming makes the algorithm more readable
    long long c = factorial(n-k);
    
    // Perform the final division with enhanced precision
    // IMPROVEMENT: All arithmetic now in long long, reducing overflow risk
    // Mathematical note: Order of division doesn't matter for exact integer arithmetic
    long long res = a / b / c;
    
    // Return the computed binomial coefficient
    return res;
}

/*
 * VERIFICATION COMMENT (UNCHANGED)
 * ================================
 * 
 * Expected result remains the same as the previous version:
 * C(15,8) should equal 6435
 * 
 * RELIABILITY IMPROVEMENT:
 * While the expected result is identical, this version is much more reliable
 * because it won't suffer from integer overflow that could occur in the
 * previous version for larger inputs.
 * 
 * Mathematical verification (same as before):
 * C(15,8) = 15! / (8! × 7!)
 *         = 1,307,674,368,000 / (40,320 × 5,040)
 *         = 1,307,674,368,000 / 203,212,800
 *         = 6,435 ✓
 */
// binomial(15, 8) = 6435

/*
 * MAIN PROGRAM EXECUTION - ENHANCED VERSION
 * =========================================
 * 
 * Demonstrates the improved binomial coefficient calculation.
 * The functionality is identical to the previous version, but with enhanced reliability.
 * 
 * KEY DIFFERENCES FROM PREVIOUS VERSION:
 * =====================================
 * 
 * 1. **PRINTF FORMAT SPECIFIER ISSUE:**
 *    - CURRENT CODE: printf("(%i %i) = %i\n", n, k, binomial(n, k));
 *    - PROBLEM: Using %i to print a long long result from binomial()
 *    - SHOULD BE: printf("(%i %i) = %lli\n", n, k, binomial(n, k));
 *    - IMPACT: May truncate output or cause undefined behavior on some systems
 * 
 * 2. **ENHANCED INTERNAL PRECISION:**
 *    - All calculations now performed in long long arithmetic
 *    - Much more robust against overflow for larger inputs
 *    - Same mathematical result for the current test case
 * 
 * 3. **IMPROVED DEBUGGING CAPABILITY:**
 *    - Intermediate factorial values stored in variables
 *    - Easier to troubleshoot if issues arise
 * 
 * TESTING SCOPE:
 * - Current test: C(15,8) = 6,435 (well within safe range for both versions)
 * - Extended capability: Can now safely handle larger values like C(20,10)
 */
int main(void)
{
    // Test parameters (unchanged from previous version)
    // These values are safely handled by both versions
    int n = 15;
    int k = 8;

    /*
     * OUTPUT FORMATTING
     * =================
     * 
     * IMPORTANT NOTE: There's a format specifier mismatch in this code!
     * 
     * Current code uses: printf("(%i %i) = %i\n", n, k, binomial(n, k));
     * Problem: binomial() now returns long long, but %i expects int
     * 
     * Correct version should be: printf("(%i %i) = %lli\n", n, k, binomial(n, k));
     * 
     * Format specifier breakdown:
     * - %i: Integer format (for n and k parameters)
     * - %lli: Long long integer format (for binomial result)
     * 
     * Current output: (15 8) = 6435 (may work due to luck, but technically incorrect)
     * Correct approach: Use proper format specifier for long long
     * 
     * This demonstrates why code reviews and testing are important!
     */
    printf("(%i %i) = %i\n", n, k, binomial(n, k));

    // Return success status (unchanged)
    return EXIT_SUCCESS;
}

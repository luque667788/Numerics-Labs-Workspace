/*
 * ARITHMETIC MEAN CALCULATOR
 * ===========================
 * 
 * GENERAL OVERVIEW:
 * This program demonstrates the calculation of the arithmetic mean (average) of a dataset.
 * The mathematical formula being computed is:
 * 
 *     mean = (x₁ + x₂ + x₃ + ... + xₙ) / n
 * 
 * This is also written as: mean = (1/n) * Σᵢ₌₁ⁿ xᵢ
 * 
 * MATHEMATICAL SIGNIFICANCE:
 * - The arithmetic mean is a measure of central tendency
 * - It represents the "typical" or "average" value in a dataset
 * - It's the most commonly used type of average in statistics
 * - The mean minimizes the sum of squared deviations from data points
 * - It's sensitive to outliers (extreme values can shift the mean significantly)
 * 
 * PROGRAM STRUCTURE:
 * 1. Define a function to calculate the arithmetic mean of an array
 * 2. Demonstrate the function with a sample dataset
 * 3. Display the computed result
 * 
 * PRACTICAL APPLICATIONS:
 * - Statistical analysis and data summarization
 * - Quality control in manufacturing
 * - Performance measurement and evaluation
 * - Scientific data analysis
 * - Financial calculations (average prices, returns, etc.)
 */

// filepath: /home/luque/Documents/UNI/sem3/numeriks/labs workspaces/ELearning_NUM_Lesson_3(3)/ELearning_NUM_Lesson_3/3-5-func/func.c

// Standard input/output library for printf function
#include <stdio.h>
// Standard library for EXIT_SUCCESS constant  
#include <stdlib.h>

/*
 * ARITHMETIC MEAN CALCULATION FUNCTION
 * ====================================
 * 
 * Computes the arithmetic mean (average) of an array of double-precision numbers.
 * 
 * MATHEMATICAL FORMULA:
 * mean = (x₁ + x₂ + x₃ + ... + xₙ) / n
 * 
 * Where:
 * - x₁, x₂, ..., xₙ are the individual data values
 * - n is the total number of data points
 * - The result is the arithmetic mean of the dataset
 * 
 * ALGORITHM STEPS:
 * 1. Initialize sum accumulator to zero
 * 2. Iterate through all array elements
 * 3. Add each element to the running sum
 * 4. Divide the total sum by the number of elements
 * 5. Return the calculated mean
 * 
 * PARAMETERS:
 * - x[]: Array of double values (the dataset)
 * - n: Integer representing the number of elements in the array
 * 
 * RETURNS:
 * - Double value representing the arithmetic mean of the input data
 * 
 * MATHEMATICAL PROPERTIES:
 * - Linear operator: mean(a*x + b*y) = a*mean(x) + b*mean(y)
 * - Translation invariant: mean(x + c) = mean(x) + c
 * - Scale invariant: mean(c*x) = c*mean(x)
 */
double func(double x[], int n)
{
  // Initialize accumulator variable to store the running sum
  // Starting with 0.0 ensures we begin with a clean slate for summation
  double res = 0.0;
  
  // Loop counter variable to iterate through all array elements
  int k;
  
  /*
   * SUMMATION LOOP
   * ==============
   * 
   * Mathematical operation: Σᵢ₌₀ⁿ⁻¹ xᵢ (sum from i=0 to n-1)
   * 
   * This loop implements the numerator of the arithmetic mean formula.
   * We iterate through each element and add it to our running total.
   * 
   * Loop mechanics:
   * - k starts at 0 (first array index)
   * - Continues while k < n (standard array bounds)
   * - k increments by 1 each iteration
   * - Accesses array elements: x[0], x[1], x[2], ..., x[n-1]
   */
  for (k=0; k<n; k++)
    res += x[k];  // Add current array element to running sum
                  // Equivalent to: res = res + x[k]
  
  /*
   * DIVISION STEP
   * =============
   * 
   * Complete the arithmetic mean calculation by dividing the sum by n.
   * This implements: mean = (sum of all values) / (number of values)
   * 
   * Mathematical significance:
   * - Converts total sum into average value
   * - Normalizes the sum by the dataset size
   * - Results in a value with the same units as the input data
   */
  res /= n;  // Equivalent to: res = res / n
  
  // Return the calculated arithmetic mean
  return res;
}

/*
 * MAIN PROGRAM EXECUTION AND DEMONSTRATION
 * ========================================
 * 
 * Demonstrates the arithmetic mean calculation with a concrete example.
 * This section shows how to use the func() function with real data.
 * 
 * EXAMPLE CALCULATION:
 * Given the dataset: {1, 2, 3, 4}
 * Expected result: (1 + 2 + 3 + 4) / 4 = 10 / 4 = 2.5
 * 
 * STEP-BY-STEP MATHEMATICAL PROCESS:
 * 1. Sum calculation: 1 + 2 + 3 + 4 = 10
 * 2. Count of elements: n = 4
 * 3. Mean calculation: 10 ÷ 4 = 2.5
 * 
 * This demonstrates that the mean (2.5) lies between the minimum (1)
 * and maximum (4) values, which is a fundamental property of the arithmetic mean.
 */
int main(void)
{
	// Define sample dataset for demonstration
	// This array contains four double-precision floating-point numbers
	// Mathematical interpretation: This represents a small dataset for analysis
	double x[] = {1, 2, 3, 4};
	
	// Store the number of elements in the dataset
	// This must match the actual number of elements in the array
	// Using a separate variable makes the code more maintainable and readable
	int n = 4;

	/*
	 * FUNCTION CALL AND OUTPUT
	 * ========================
	 * 
	 * Call our arithmetic mean function and display the result.
	 * 
	 * printf format explanation:
	 * - %lf: Format specifier for double (long float)
	 *   * Displays the result as a decimal number
	 *   * Default precision is typically 6 decimal places
	 * - \n: Newline character for proper output formatting
	 * 
	 * Expected output: 2.500000
	 * 
	 * Mathematical verification:
	 * mean = (1 + 2 + 3 + 4) / 4 = 10 / 4 = 2.5 ✓
	 */
	printf("%lf\n", func(x, n));

	// Return success status to the operating system
	// EXIT_SUCCESS indicates the program completed without errors
    return EXIT_SUCCESS;
}

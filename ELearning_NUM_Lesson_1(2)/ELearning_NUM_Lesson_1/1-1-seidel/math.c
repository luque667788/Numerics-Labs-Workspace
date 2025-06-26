/*
 * GAUSS-SEIDEL ITERATIVE METHOD FOR SOLVING LINEAR SYSTEMS
 *
 * GENERAL OVERVIEW:
 * This program implements the Gauss-Seidel iterative method to solve a system of linear equations.
 * The Gauss-Seidel method is an improvement over the Jacobi method, using the most recent values as soon as they are available.
 *
 * MATHEMATICAL BACKGROUND:
 * - System: 7x₁ - x₂ = 5
 *           3x₁ - 5x₂ = -7
 * - Exact solution: (x₁, x₂) = (1, 2)
 *
 * GAUSS-SEIDEL PRINCIPLE:
 * - Rearrange equations to solve for each variable:
 *   x₁ = (5 + x₂) / 7
 *   x₂ = (7 + 3x₁) / 5
 * - In each iteration, use the newest value of x₁ immediately when computing x₂.
 * - This typically leads to faster convergence than Jacobi.
 *
 * CONVERGENCE:
 * The method converges for diagonally dominant matrices, as in this example.
 * Each iteration should approach the true solution (1, 2).
 */

#include <stdio.h>        // For printf() function
#include <stdlib.h>       // For EXIT_SUCCESS
#include <stdbool.h>      // For boolean type
#include <time.h>         // For clock() and timing

/*
 * WAIT FUNCTION - Creates a time delay
 * PURPOSE: Pauses program execution for a specified duration (in seconds)
 * - Uses CPU clock ticks to implement a busy-wait loop
 */
static void WAIT(double time)
{
    int cnt = clock();                           // Record current CPU clock ticks
    int ticks = (int) (time*CLOCKS_PER_SEC);     // Convert seconds to clock ticks
    while (true)                                 // Infinite loop until time elapsed
    {
        if (clock() > (cnt + ticks))            // Check if enough time has passed
            return;                             // Exit function when time elapsed
    }
}

/*
 * MAIN FUNCTION - Implements Gauss-Seidel Iteration
 */
int main(void)
{
    float x = 0.0;    // x₁ variable (first unknown), initial guess = 0
    float y = 0.0;    // x₂ variable (second unknown), initial guess = 0
    int n = 1;        // Iteration counter
    
    // GAUSS-SEIDEL ITERATION LOOP
    // Performs 9 iterations (n from 1 to 9)
    while (n < 10)
    {
        // DISPLAY CURRENT ITERATION RESULTS
        printf("xy(%2d) = (% 1.2f, % 1.2f)\n", n, x, y);
        // Format: iteration number, x-value, y-value (2 decimals, aligned)

        WAIT(1.0);    // Pause for 1 second to observe convergence

        // MATHEMATICAL SYSTEM BEING SOLVED:
        // 7x₁ - x₂ = 5     (equation 1)
        // 3x₁ - 5x₂ = -7   (equation 2)
        // Algebraic solution: (x₁, x₂) = (1, 2)
        //
        // GAUSS-SEIDEL FORMULAS:
        // x₁ = (5 + x₂) / 7
        // x₂ = (7 + 3x₁) / 5
        //
        // KEY DIFFERENCE FROM JACOBI: Use the NEW x₁ value immediately for x₂

        x = (5.0 + y)/7.0;         // Compute new x₁ using previous y (x₂)
        // From 7x₁ - x₂ = 5 → x₁ = (5 + x₂)/7

        y = (7.0 + 3.0*x)/5.0;     // Compute new x₂ using the NEW x₁
        // From 3x₁ - 5x₂ = -7 → x₂ = (7 + 3x₁)/5
        // This is the main improvement over Jacobi: y uses the latest x

        n++;                       // Increment iteration counter
    }
    // As iterations continue, (x, y) should converge to (1, 2)

    return EXIT_SUCCESS;           // Program completed successfully
}

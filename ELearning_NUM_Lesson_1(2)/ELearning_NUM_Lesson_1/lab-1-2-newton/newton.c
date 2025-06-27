/*
 * NEWTON'S METHOD FOR SQUARE ROOTS
 *
 * GENERAL OVERVIEW:
 * This program implements Newton's method (also called the Newton-Raphson method) to approximate the square root of a number.
 * Newton's method is an iterative numerical technique for finding roots of equations. For square roots, it solves x^2 = f.
 *
 * MATHEMATICAL BACKGROUND:
 * - Newton's method for solving f(x) = 0 uses the iteration:
 *     x(n+1) = x(n) - f(x(n)) / f'(x(n))
 *   where f'(x) is the derivative of f(x).
 * - To find sqrt(f), solve x^2 = f, or equivalently, x = sqrt(f).
 * - Newton's iteration formula for this is:
 *     x(n+1) = 0.5 * (x(n) + f / x(n))
 * - Start with an initial guess (here, x = f), and repeat until the change is smaller than a set tolerance.
 * - The method converges rapidly for positive f.
 *
 * CODE EXPLANATION:
 * - Handles f == 0 as a special case (sqrt(0) = 0).
 * - Uses a loop to repeatedly improve the guess using Newton's formula.
 * - Stops when the difference between successive guesses is less than Tolerance.
 * - Compares with the standard library sqrt for verification.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>           // for comparison only

/* Lab 1.2 Write a function to find the square root by using Newton's method */

const float Tolerance = 0.001; // Stop iterating when error is less than this

static float Sqrt(float f)
{
    if (f == 0.0)
        return 0.0; // sqrt(0) = 0
    
    // x(n) is the current guess, x(n+1) is the next guess
    float x = f;    // Start with initial guess x = f
    float xn;       // Will hold the next guess
    
    // Take absolute value of x (for robustness, though f should be >= 0)
    if (x < 0)
        x *= -1.0;
    
    while (true)
    {
        // Newton's iteration: x(n+1) = 0.5 * (x(n) + f / x(n))
        xn = 0.5*(x + f/x);

        // Compute the difference between guesses (approximation error)
        float e = x - xn;

        // Take absolute value of error
        if (e < 0)
            e *= -1.0;
        
        // If error is less than tolerance, stop iterating
        if (e < Tolerance)
            break;
        
        x = xn; // Update guess for next iteration
    }

    return x; // Return the approximated square root
}

int main(void)
{
    printf("% 6.3f\n", Sqrt(2.0));      // Print Newton's method result for sqrt(2)
    printf("% 6.3f\n", sqrt(2.0));      // Print standard library result for comparison
    return EXIT_SUCCESS;
}

// math.c - Implements Newton-Raphson method for solving pow(x, x) = 100
//
// This file defines a function f(x) = x^x - 100 and its derivative fp(x),
// and uses the Newton-Raphson method to find a root of f(x) = 0.
// The main function demonstrates the root-finding starting from x0 = 1.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// f(x): The function whose root we want to find, here f(x) = x^x - 100
static double f(double x)
{
    // pow(x, x) computes x raised to the power x
    // Subtract 100 to set up the equation x^x = 100
    return pow(x, x) - 100;
}

// fp(x): The derivative of f(x) with respect to x
static double fp(double x)
{
    // The derivative of x^x is x^x * (log(x) + 1)
    // This is used in the Newton-Raphson update step
    return pow(x, x) * (log(x) + 1);
}

// NewtonRaphson(x0): Finds a root of f(x) = 0 using the Newton-Raphson method
// x0: Initial guess for the root
// Returns: Approximate root of f(x) = 0
static double NewtonRaphson(double x0)
{
    int maxIter = 1000;      // Maximum number of iterations to prevent infinite loops
    double tol = 1e-8;       // Tolerance for convergence (stop if |f(x)| < tol)
    int iter = 0;            // Iteration counter
    double x = x0;           // Current guess for the root
    double fx = f(x);        // Current value of f(x)
    // Iterate until the function value is close to zero or max iterations reached
    while ((fabs(fx) > tol) && (iter < maxIter))
    {
        // Newton-Raphson update: x_{n+1} = x_n - f(x_n)/f'(x_n)
        x = x - fx/fp(x);
        fx = f(x);           // Update f(x) for the new x
        iter++;
    }
    // Return the computed root (may not be exact if maxIter reached)
    return x;
}

// main: Entry point of the program
int main(void)
{
    // Call NewtonRaphson with initial guess x0 = 1 and print the result
    printf("%2.2f\n", NewtonRaphson(1));
    return EXIT_SUCCESS;
}

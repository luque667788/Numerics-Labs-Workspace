// This program numerically approximates the integral of f(x) = exp(cos^3(x)) over [0, 2π] using the trapezoidal rule.
// It demonstrates basic numerical integration and use of math functions in C.
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function to integrate: f(x) = exp(cos(x)^3)
static double f(double x)
{
    return exp(cos(x)*cos(x)*cos(x)); // Calculate exp(cos^3(x))
}

// Trapezoidal rule for numerical integration
// a: lower limit, b: upper limit, n: number of subintervals
static double f_trap(double a, double b, int n)
{
    double h = (b - a)/n; // Step size
    double res = h * ( f(a) + f(b) ) / 2; // Start with endpoints
    int k;
    for (k=1; k<n; k++)
    {
        double xk = (b-a) * k/n + a; // Interior point
        res += h * f(xk); // Add area of each trapezoid
    }
    return res; // Return the integral approximation
}

int main(void)
{
    // Integrate f(x) from 0 to 2π with 100 intervals, print result with 2 decimals
    printf("%2.2f\n", f_trap(0, 2*acos(-1.0), 100));
    return EXIT_SUCCESS;
}

/*
 * TAYLOR SERIES APPROXIMATION FOR sin(x)
 *
 * GENERAL OVERVIEW:
 * This program implements a function to approximate the sine of a number using its Taylor series expansion.
 * The Taylor series for sin(x) is: sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
 * This code uses helper functions for raising to a power and computing factorials, and sums the first four terms of the series.
 *
 * CODE LOGIC:
 * - RaisePower: Computes f^power for integer power (handles negative powers as reciprocals).
 * - Factorial: Computes n! (product of all positive integers up to n).
 * - Sin: Sums the first four terms of the Taylor series for sin(x).
 * - main: Compares the custom Sin function to the standard library sin function.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>           // for comparison only

/* Lab 1.3 Write your own function to evaluate the trigonometric function sin(x) */

/*
 * RaisePower: computes f^power for integer power (handles negative powers)
 */
static float RaisePower(float f, int power)
{
    if (power == 0)
        return 1.0;
    
    int sign = 1;

    if (power < 0)              // if power is negative then make positive and later invert result
    {
        sign = -1;
        power *= sign;
    }
    
    float result = f;

    while (power > 1)
    {
        result *= f;

        power--;
    }

    if (sign < 0)
        result = 1.0/result;    // if power was negative then invert result
    
    return result;
}

/*
 * Factorial: computes n! (product of all positive integers up to n)
 */
static int Factorial(int n) 
{
    int f = 1;

    while (n > 0)
    {
         f *= n;

         n--;
    }
    
    return f;
}

/*
 * sin(x) = sum(i=0,n) (-1)^i x^(2i+1) / (2i+1)!
 * sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
 */
static float Sin(float x)
{
    float result = x; // First term of Taylor series

    for (int i = 1; i <= 3; i++)
        result += ( RaisePower(-1.0, i) * RaisePower(x, 2*i+1) / Factorial(2*i+1) );
    
    return result;
}

float Pi = 3.1415;

int main(void)
{
    printf("% 6.3f\n", Sin(Pi/2.0 + 0.1));   // Print custom Sin function result

    printf("% 6.3f\n", sin(Pi/2.0 + 0.1));   // Print standard library result for comparison

    return EXIT_SUCCESS;
}

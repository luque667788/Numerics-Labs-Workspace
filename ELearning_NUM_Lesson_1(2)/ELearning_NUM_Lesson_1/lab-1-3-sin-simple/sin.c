/*
 * SIMPLE TAYLOR SERIES APPROXIMATION FOR sin(x)
 *
 * GENERAL OVERVIEW:
 * This program implements a simple function to approximate the sine of a number using the first four terms of its Taylor series expansion.
 * The Taylor series for sin(x) is: sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
 * This version hardcodes the denominators for factorials and does not use a loop or a separate factorial function.
 *
 * CODE LOGIC:
 * - RaisePower: Computes f^power for integer power (handles negative powers as reciprocals).
 * - Sin: Sums the first four terms of the Taylor series for sin(x) using hardcoded factorial denominators.
 * - main: Compares the custom Sin function to the standard library sin function.
 *
 * DIFFERENCE FROM FULL VERSION:
 * - This version does not use a loop or a general factorial function.
 * - Only the first four terms are included, and the denominators are written explicitly.
 * - The full version is more general and can easily be extended to more terms.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>           // for comparison only

/* Lab 1.3 Write your own function to evaluate the trigonometric function sin(x) */

/* RaisePower: computes f^power for integer power (handles negative powers) */
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

// sin(x) = x - x^3/3! + x^5/5! - x^7/7!
static float Sin(float x)
{
    return ( x - RaisePower(x, 3)/6 + RaisePower(x, 5)/120 - RaisePower(x, 7)/5040 );
    // 3! = 6, 5! = 120, 7! = 5040
}

float Pi = 3.1415;

int main(void)
{
    printf("% 6.3f\n", Sin(Pi/2.0 + 0.1));   // Print custom Sin function result

    printf("% 6.3f\n", sin(Pi/2.0 + 0.1));   // Print standard library result for comparison

    return EXIT_SUCCESS;
}

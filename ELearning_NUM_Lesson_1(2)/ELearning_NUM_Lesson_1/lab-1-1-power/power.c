/*
 * FULL POWER FUNCTION (lab-1-1-power)
 *
 * This version of RaisePower handles both positive and negative integer exponents correctly.
 * - If power == 0, returns 1.0 (as any number to the 0th power is 1).
 * - If power < 0, computes the positive power and then returns the reciprocal (1.0/result).
 * - Otherwise, multiplies f by itself 'power' times.
 *
 * DIFFERENCES FROM THE SIMPLE VERSION (lab-1-1-power-simple/power.c):
 * - This version returns the mathematically correct result for negative exponents (e.g., 2^-3 = 0.125).
 * - The simple version just returns 0.0 for negative exponents (not correct).
 * - This version is more robust and general.
 * - Both versions use the same loop for positive exponents.
 */

#include <stdio.h>
#include <stdlib.h>

/* Lab 1.1 Write a function to raise a floating point number to an integer power */

static float RaisePower(float f, int power)
{
    if (power == 0)
        return 1.0; // Any number to the 0th power is 1
    
    int sign = 1;

    if (power < 0)              // If power is negative, make positive and invert result later
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
        result = 1.0/result;    // If power was negative, invert result
    
    return result;
}

int main(void)
{
    printf("% 5.1f\n", RaisePower(2.0, 3));    // 2^3 = 8.0
    printf("% 5.1f\n", RaisePower(9.0, 2));    // 9^2 = 81.0
    printf("% 7.3f\n", RaisePower(2.0, -3));   // 2^-3 = 0.125
    printf("% 7.3f\n", RaisePower(9.0, -2));   // 9^-2 = 0.012
    return EXIT_SUCCESS;
}

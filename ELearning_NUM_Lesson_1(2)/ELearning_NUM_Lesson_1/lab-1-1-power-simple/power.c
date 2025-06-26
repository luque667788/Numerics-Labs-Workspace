/*
 * SIMPLE POWER FUNCTION (lab-1-1-power-simple)
 *
 * This version of RaisePower only handles non-negative integer exponents.
 * - If power == 0, returns 1.0 (as any number to the 0th power is 1).
 * - If power < 0, returns 0.0 (does NOT handle negative exponents correctly).
 * - Otherwise, multiplies f by itself 'power' times.
 *
 * DIFFERENCES FROM THE FULL VERSION (lab-1-1-power/power.c):
 * - The full version correctly handles negative exponents by returning the reciprocal (1.0/result).
 * - The full version always returns a mathematically correct result for any integer exponent.
 * - This simple version returns 0.0 for negative exponents, which is not mathematically correct.
 * - The rest of the logic (for positive exponents) is the same.
 */

#include <stdio.h>
#include <stdlib.h>

/* Lab 1.1 Write a function to raise a floating point number to an integer power */

static float RaisePower(float f, int power)
{
    if (power == 0)
        return 1.0; // Any number to the 0th power is 1
    
    if (power < 0)          // This version just returns 0.0 for negative powers (not correct)
        return 0.0;
    
    float result = f;

    while (power > 1)
    {
        result *= f;
        power--;
    }
    
    return result;
}

int main(void)
{
    printf("% 5.1f\n", RaisePower(2.0, 3));   // 2^3 = 8.0
    printf("% 5.1f\n", RaisePower(9.0, 2));   // 9^2 = 81.0
    // Negative exponents would return 0.0 in this version
    return EXIT_SUCCESS;
}

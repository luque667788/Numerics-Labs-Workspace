#include <stdio.h>
#include <stdlib.h>

/* Lab 1.1 Write a function to raise a floating point number to an integer power */

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

int main(void)
{
    printf("% 5.1f\n", RaisePower(2.0, 3));

    printf("% 5.1f\n", RaisePower(9.0, 2));

    printf("% 7.3f\n", RaisePower(2.0, -3));

    printf("% 7.3f\n", RaisePower(9.0, -2));

    return EXIT_SUCCESS;
}

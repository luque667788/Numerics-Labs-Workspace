#include <stdio.h>
#include <stdlib.h>

#include <math.h>           // for comparison only

/* Lab 1.3 Write your own function to evaluate the trigonometric function sin(x) */

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

// sin(x) = sum(i=0,n) (-1)^i x^(2i+1) / (2i+1)!
// sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...

static float Sin(float x)
{
    return ( x - RaisePower(x, 3)/6 + RaisePower(x, 5)/120 - RaisePower(x, 7)/5040 );
}

float Pi = 3.1415;

int main(void)
{
    printf("% 6.3f\n", Sin(Pi/2.0 + 0.1));

    printf("% 6.3f\n", sin(Pi/2.0 + 0.1));

    return EXIT_SUCCESS;
}

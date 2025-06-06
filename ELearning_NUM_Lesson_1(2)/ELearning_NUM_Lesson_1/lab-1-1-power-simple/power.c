#include <stdio.h>
#include <stdlib.h>

/* Lab 1.1 Write a function to raise a floating point number to an integer power */

static float RaisePower(float f, int power)
{
    if (power == 0)
        return 1.0;
    
    if (power < 0)          // if power is negative then return
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
    printf("% 5.1f\n", RaisePower(2.0, 3));

    printf("% 5.1f\n", RaisePower(9.0, 2));

    return EXIT_SUCCESS;
}

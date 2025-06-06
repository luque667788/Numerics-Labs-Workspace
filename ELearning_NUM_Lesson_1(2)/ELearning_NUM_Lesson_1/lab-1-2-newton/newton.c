#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>           // for comparison only

/* Lab 1.2 Write a function to find the square root by using Newton's method */

const float Tolerance = 0.001;

static float Sqrt(float f)
{
    if (f == 0.0)
        return 0.0;
    
    // x(n) x(n+1)
    float x = f;
    float xn;
    
    // fabs(x)
    if (x < 0)
        x *= -1.0;
    
    while (true)
    {
        // x(n+1) = 0.5*(x(n) + f/x(n));
        xn = 0.5*(x + f/x);

        // approximation error
        float e = x - xn;

        // fabs(e)
        if (e < 0)
            e *= -1.0;
        
        // ? done
        if (e < Tolerance)
            break;
        
        x = xn;
    }

    return x;
}

int main(void)
{
    printf("% 6.3f\n", Sqrt(2.0));

    printf("% 6.3f\n", sqrt(2.0));

    return EXIT_SUCCESS;
}

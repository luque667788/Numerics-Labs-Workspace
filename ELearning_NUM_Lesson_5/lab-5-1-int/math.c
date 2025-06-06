#include <stdio.h>
#include <stdlib.h>

#include <math.h>

static double f(double x)
{
	return exp(cos(x)*cos(x)*cos(x));
}

static double f_trap(double a, double b, int n)
{
    double h = (b - a)/n;
    double res = h * ( f(a) + f(b) ) / 2;
	int k;
    for (k=1; k<n; k++)
	{
        double xk = (b-a) * k/n + a;
        res += h * f(xk);
    }
    return res;
}

int main(void)
{
	printf("%2.2f\n", f_trap(0, 2*acos(-1.0), 100));

    return EXIT_SUCCESS;
}

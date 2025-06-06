#include <stdio.h>
#include <stdlib.h>

#include <math.h>

double simple(double x)
{
	return round(x*x);
}

int main(void)
{
	double x = 2.2;

	double y = simple(x);

	printf("quad(% 1.2lf) = % 1.2lf\n", x, y);

    return EXIT_SUCCESS;
}

#include <stdio.h>
#include <stdlib.h>

double func(double x[], int n)
{
  double res = 0.0;
  int k;
  for (k=0; k<n; k++)
    res += x[k];
  res /= n;
  return res;
}

int main(void)
{
	double x[] = {1, 2, 3, 4};
	int n = 4;

	printf("%lf\n", func(x, n));

    return EXIT_SUCCESS;
}

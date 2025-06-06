#include <stdio.h>
#include <stdlib.h>

#include <math.h>

static long long factorial(int n)
{
    long long res = 1;
    int k;
    for (k=2; k<=n; k++)
        res *= k;
    return res;
}

static long long binomial(int n, int k)
{
    long long a = factorial(n);
    long long b = factorial(k);
    long long c = factorial(n-k);
    long long res = a / b / c;
    return res;
}

static double f(double x)
{
	return 1.0/(1.0 + 16.0*x*x);
}

int main(void)
{
	int n = 15;
	int k;
	// EquispacedNodes
	double xnodes[n+1];
	for (k=0; k<=n; k++) xnodes[k] = k;
	for (k=0; k<=n; k++) xnodes[k] = -1.0+xnodes[k]*2.0/n;
	// EquispacedBarWeights
	double w[n+1];
	for (k=0; k<=n; k++)
    	w[k] = pow(-1.0, k) * binomial(n, k);
	
	double f_sample[n+1];
	for (k=0; k<=n; k++)
		f_sample[k] = f(xnodes[k]);

	for (k=0; k<=n; k++)
		printf("%2.2lf ", xnodes[k]);
	printf("\n");
	for (k=0; k<=n; k++)
		printf("%2.2lf ", w[k]);
	printf("\n");
	for (k=0; k<=n; k++)
		printf("%2.2lf ", f_sample[k]);
	printf("\n");

    return EXIT_SUCCESS;
}

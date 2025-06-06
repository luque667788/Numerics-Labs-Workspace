#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#define e	0.000000000000001

static double LagrangeInterp1D(double fvals[], double xnodes[], int n, double barw[], double t)
{
    double numt = 0.0;
    double denomt = 0.0;
    
	int j;
    for (j=0; j<=n; j++)
	{
        double tdiff = t - xnodes[j];
        numt += barw[j] / tdiff * fvals[j];
        denomt += + barw[j] / tdiff;
        
        if (fabs(tdiff) < e)
		{
            numt = fvals[j];
            denomt = 1.0;
            break;
		}
	}
    return numt / denomt;
}

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

	double t = -1.0;
	double f_interp = LagrangeInterp1D(f_sample, xnodes, n, w, t);

	printf("LagrangeInterp1D(% 1.2f) = % 1.2f\n", t, f_interp);

    return EXIT_SUCCESS;
}

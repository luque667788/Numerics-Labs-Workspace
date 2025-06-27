#include <stdio.h>
#include <stdlib.h>

#include <math.h>

void euler(double x[], double u[], double a, double x0, double T, int n);

int main(void)
{
	// sampling time
	double T = 0.1;
	int n = 100;

	int k;
	double t[n+1];
	for (k=0; k<=n; k++)
		t[k] = 0.0 + k*T;
	
	double u[n+1];
	for (k=0; k<=n; k++)
		u[k] = exp(-pow(t[k] - 5.0, 10.0));

	double a = 1.5;
	double x0 = 0.0;
	double x[n+1];

	euler(x, u, a, x0, T, n);

	double x1[n+1];
	double x2[n+1];

	euler(x1, u, a, x0, T, n);      // First stage
	euler(x2, x1, a, x0, T, n);     // Second stage

	printf("t\tu\tx1\tx2\n");
	for (k=0; k<=n; k++)
		printf("%2.2lf\t%lf\t%lf\t%lf\n", t[k], u[k], x1[k], x2[k]);

    return EXIT_SUCCESS;
}

void euler(double x[], double u[], double a, double x0, double T, int n)
{
	int k;
	x[0] = x0;
	for (k=0; k<n; k++)
		x[k+1] = (1-a*T)*x[k] + T*u[k];
}

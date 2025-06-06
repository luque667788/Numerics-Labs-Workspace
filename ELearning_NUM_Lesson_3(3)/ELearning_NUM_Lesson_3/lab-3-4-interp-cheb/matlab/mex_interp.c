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

static double f(double x)
{
	return 1.0/(1.0 + 16.0*x*x);
}

double calc(double t)
{
	int n = 32;
	int k;
	// ClosedChebNodes
	double xnodes[n+1];
	for (k=0; k<=n; k++) xnodes[k] = k;
	for (k=0; k<=n; k++) xnodes[k] = cos(xnodes[k]*acos(-1.0)/n);
	// ClosedChebBarWeights
	double w[n+1];
	w[0] = 0.5;
	for (k=1; k<=n-1; k++)
    		w[k] = pow(-1.0, k);
	w[n] = 0.5*pow(-1.0, n);
	
	double f_sample[n+1];
	for (k=0; k<=n; k++)
		f_sample[k] = f(xnodes[k]);

	double f_interp = LagrangeInterp1D(f_sample, xnodes, n, w, t);

    return f_interp;
}

/*** MEX ***/

#include "mex.h"

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  /*** error ***/
  if (nrhs < 1)
  {
    plhs[0] = mxCreateDoubleScalar(0);
    return;
  }

  double x = mxGetScalar(prhs[0]);

  double y = calc(x);

  plhs[0] = mxCreateDoubleScalar(y);
}

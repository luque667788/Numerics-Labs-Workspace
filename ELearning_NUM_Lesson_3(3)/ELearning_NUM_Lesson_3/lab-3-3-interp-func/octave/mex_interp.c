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

/*** MEX ***/

#include "mex.h"

void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
  /*** error ***/
  if (nrhs < 4)
  {
    plhs[0] = mxCreateDoubleScalar(0);
    return;
  }

  int n = mxGetN(prhs[0]);

  /*** error ***/
  if (n < 1)
  {
    plhs[0] = mxCreateDoubleScalar(0);
    return;
  }

  double *fvals = mxGetPr(prhs[0]);
  double *xnodes = mxGetPr(prhs[1]);
  double *barw = mxGetPr(prhs[2]);
  double t = mxGetScalar(prhs[3]);

  double res = LagrangeInterp1D(fvals, xnodes, n, barw, t + e);

  plhs[0] = mxCreateDoubleScalar(res);
}

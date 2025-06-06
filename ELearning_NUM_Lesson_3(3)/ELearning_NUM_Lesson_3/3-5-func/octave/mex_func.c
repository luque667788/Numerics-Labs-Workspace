double func(double x[], int n)
{
  double res = 0.0;
  int k;
  for (k=0; k<n; k++)
    res += x[k];
  res /= n;
  return res;
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

  int n = mxGetN(prhs[0]);

  /*** error ***/
  if (n < 1)
  {
    plhs[0] = mxCreateDoubleScalar(0);
    return;
  }

  double *input = mxGetPr(prhs[0]);

  double output = func(input, n);

  plhs[0] = mxCreateDoubleScalar(output);
}

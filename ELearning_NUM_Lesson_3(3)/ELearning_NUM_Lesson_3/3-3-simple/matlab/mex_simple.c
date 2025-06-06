#include <math.h>

double simple(double x)
{
  return round(x*x);
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

  double y = simple(x);

  plhs[0] = mxCreateDoubleScalar(y);
}

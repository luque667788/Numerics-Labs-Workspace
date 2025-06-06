#include <math.h>

#define SEQ_MAX		100

struct SEQ
{
	double x[SEQ_MAX+1];
	int n;
};

struct SEQ seq;

void calc(int n)
{
	if (n > SEQ_MAX)
		return;
	
	seq.n = 1;

	seq.x[seq.n] = 0.0;

	while (seq.n < n+1)
	{
		seq.x[seq.n] = pow((-1.0), (seq.n + 1.0))/(seq.n);

		(seq.n)++;
	}
}

void init(void)
{
	int n;

	for (n=0; n < SEQ_MAX; n++)
		seq.x[n] = 0.0;
	
	seq.n = 0;
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

  double input = mxGetScalar(prhs[0]);

  int n = round(input);

  /*** error ***/
  if (n > SEQ_MAX)
  {
    plhs[0] = mxCreateDoubleScalar(0);
    return;
  }

  init();

  calc(n);

  plhs[0] = mxCreateDoubleMatrix(1, n, mxREAL);

  double *output = mxGetPr(plhs[0]);

  int k;
  for (k=0; k<n; k++)
    output[k] = seq.x[k+1];
}

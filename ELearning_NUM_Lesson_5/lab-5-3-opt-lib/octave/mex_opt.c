#include <stdio.h>
#include <stdlib.h>

#include <nlopt.h>

#define TOL	1e-4

// function
// f = 143*x + 60*y

// constraints
// 120x + 210y <= 15000
// 110x + 30y <= 4000
// x + y <= 75
// 0 <= x and 0 <= y

// f = 143*x + 60*y

static double f(unsigned n, const double *x, double *grad, void *data)
{
	return -(143*x[0] + 60*x[1]);
}

// 120x + 210y <= 15000

static double constr1(unsigned n, const double *x, double *grad, void *data)
{	
	return 120*x[0] + 210*x[1] - 15000;
}

// 110x + 30y <= 4000

static double constr2(unsigned n, const double *x, double *grad, void *data)
{
	return 110*x[0] + 30*x[1] - 4000;
}

// x + y <= 75

static double constr3(unsigned n, const double *x, double *grad, void *data)
{
	return x[0] + x[1] - 75;
}

// 0 <= x and 0 <= y

static double LB[2] = { 0, 0 };

static double SOL[3] = { 0, 0, 0 };

static void calc(double x0, double x1)
{
	nlopt_opt opt;
	double fmin;

	// algorithm and dimensionality

	// NLOPT_LN_COBYLA : local optimization, no derivatives
	// NLOPT_LD_MMA    : local optimization, derivative/gradient-based
	
	opt = nlopt_create(NLOPT_LN_COBYLA, 2);
	// opt = nlopt_create(NLOPT_LD_MMA, 2);

	nlopt_set_min_objective(opt, f, NULL);

	double x[2];
	x[0] = x0;
	x[1] = x1;

	nlopt_set_xtol_rel(opt, TOL);

	nlopt_add_inequality_constraint(opt, constr1, NULL, TOL*TOL);
	nlopt_add_inequality_constraint(opt, constr2, NULL, TOL*TOL);
	nlopt_add_inequality_constraint(opt, constr3, NULL, TOL*TOL);

	nlopt_set_lower_bounds(opt, LB);

	if (nlopt_optimize(opt, x, &fmin) < 0)
	{
		SOL[0] = 0.0;
		SOL[1] = 0.0;
		SOL[2] = 0.0;
	}
	else
	{
		SOL[0] = x[0];
		SOL[1] = x[1];
		SOL[2] = fmin;
	}

	nlopt_destroy(opt);
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

  double *input = mxGetPr(prhs[0]);

  plhs[0] = mxCreateDoubleMatrix(1, 3, mxREAL);

  double *output = mxGetPr(plhs[0]);

  calc(input[0], input[1]);

  output[0] = SOL[0];
  output[1] = SOL[1];
  output[2] = SOL[2];
}

// opt.c - Constrained optimization using NLopt library (COBYLA algorithm)
//
// This program maximizes the linear function f(x, y) = 143x + 60y
// subject to the following constraints:
//   120x + 210y <= 15000
//   110x + 30y  <= 4000
//   x + y       <= 75
//   0 <= x, 0 <= y
//
// The NLopt library is used for optimization. The objective is negated (converted to minus) to
// convert the maximization problem into a minimization problem (as required by NLopt).

#include <stdio.h>
#include <stdlib.h>
#include <nlopt.h>

#define TOL 1e-4 // Tolerance for optimization and constraints

// Objective function: returns negative of f(x, y) for minimization
// x[0] = x, x[1] = y
static double f(unsigned n, const double *x, double *grad, void *data)
{
    // grad is ignored (COBYLA does not use gradients)
    return -(143*x[0] + 60*x[1]);
}

// Constraint 1: 120x + 210y <= 15000
static double constr1(unsigned n, const double *x, double *grad, void *data)
{
    // Returns positive if constraint is violated
    return 120*x[0] + 210*x[1] - 15000;
}

// Constraint 2: 110x + 30y <= 4000
static double constr2(unsigned n, const double *x, double *grad, void *data)
{
    return 110*x[0] + 30*x[1] - 4000;
}

// Constraint 3: x + y <= 75
static double constr3(unsigned n, const double *x, double *grad, void *data)
{
    return x[0] + x[1] - 75;
}

// Lower bounds for x and y (x >= 0, y >= 0)
static double LB[2] = { 0, 0 };

int main(void)
{
    nlopt_opt opt;
    double fmin;

    // Create optimizer object for 2 variables using COBYLA (derivative-free)
    opt = nlopt_create(NLOPT_LN_COBYLA, 2);
    // Set the objective function (minimize negative of f)
    nlopt_set_min_objective(opt, f, NULL);

    double x[2] = { 0.0, 0.0 }; // Initial guess for x and y

    // Set relative tolerance for stopping criterion
    nlopt_set_xtol_rel(opt, TOL);

    // Add inequality constraints (each must be <= 0)
    nlopt_add_inequality_constraint(opt, constr1, NULL, TOL*TOL);
    nlopt_add_inequality_constraint(opt, constr2, NULL, TOL*TOL);
    nlopt_add_inequality_constraint(opt, constr3, NULL, TOL*TOL);

    // Set lower bounds for variables
    nlopt_set_lower_bounds(opt, LB);

    // Run the optimization
    if (nlopt_optimize(opt, x, &fmin) < 0)
    {
        nlopt_destroy(opt);
		printf("Optimization failed.\n");
        return EXIT_FAILURE;
    }

    // Print the optimal solution (negate fmin to get maximum)
    printf("f(%.1f, %.1f) = %.1f\n", x[0], x[1], -fmin);


    nlopt_destroy(opt);
    return EXIT_SUCCESS;
}

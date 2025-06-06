// math.c - Linear Least-Squares Fit with LAPACK Example
//
// This file demonstrates how to perform a linear least-squares fit to noisy data in C,
// both by hand (using explicit matrix inversion) and using the LAPACK library for solving linear systems.
//
// Step-by-step guide to understanding and implementing your own least-squares fit:
//
// 1. **Generate Data**
///    - Choose the true parameters for your line (a0 = intercept, a1 = slope).
///    - Create an array of x values (here, evenly spaced from 0 to 10).
///    - Compute the corresponding y values using your line equation.
///    - Add random noise to y to simulate real-world measurements.
//
/// 2. **Print Data**
///    - Print x, the true y (without noise), and the noisy y arrays to see your data.
//
/// 3. **Set Up the Normal Equations**
///    - The least-squares fit minimizes the sum of squared errors between your data and the model.
///    - For a line y = a0 + a1*x, the normal equations are:
///         [ n+1      sum(x)   ] [a0] = [ sum(y)      ]
///         [ sum(x)   sum(x^2) ] [a1]   [ sum(x*y)    ]
///      where n+1 is the number of data points.
///    - Compute the sums using helper functions (sum, mulsum).
///    - Fill the matrix A and vector B with these sums.
//
/// 4. **Solve the System by Hand (for 2x2 case)**
///    - For small systems, you can invert the matrix A explicitly using the formula for a 2x2 inverse.
///    - Compute the determinant and use it to solve for a0 and a1.
///    - This is what the af[] array does in the code.
//
/// 5. **Solve the System with LAPACK**
///    - For larger or more general systems, use a library like LAPACK.
///    - LAPACKE_dgels solves least-squares problems efficiently and robustly.
///    - Pass your matrix A and vector B to LAPACKE_dgels; it overwrites B with the solution.
///    - This is the recommended way for real applications.
//
/// 6. **Print Results**
///    - Use printm to display the matrix A, vector B, and the solution af (from both methods).
///    - Compare the hand-solved and LAPACK-solved results to verify correctness.
//
/// 7. **Key Takeaways for Your Own Implementation**
///    - Always construct your sums carefully.
///    - For small problems, explicit inversion is fine; for larger ones, use a library.
///    - Understand the structure of the normal equations for your model.
///    - Use helper functions to keep your code clean and modular.
///    - Print intermediate results to debug and learn.
//
/// By following these steps, you can implement your own least-squares fit for any linear model in C.
//
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include <lapacke.h>

static void printm(char *name, int m, int n, double *pA)
{
    int i, j;
    // Print a matrix or vector with a label
    printf("\n %s\n", name);
    for (i=0; i<m; i++)
    {
        for (j=0; j<n; j++)
            printf(" %6.2lf", pA[i*n+j]);
        printf("\n");
    }
}

// Computes the sum of element-wise products (dot product) of two arrays
static double mulsum(double x[], double y[], int n)
{
    double res = 0.0;
    int k;
    for (k=0; k<=n; k++)
        res += x[k]*y[k];
    return res;
}

// Computes the sum of elements in an array
static double sum(double x[], int n)
{
    double res = 0.0;
    int k;
    for (k=0; k<=n; k++)
        res += x[k];
    return res;
}

// Linear function for generating true (noise-free) data
static double f(double x, double a0, double a1)
{
    return a1*x + a0;
}

int main(void)
{
    // --- Step 1: Generate synthetic data ---
    double a0 = 0.5; // true intercept
    double a1 = 0.5; // true slope
    int n = 20;
    int k;
    double x[n+1];
    for (k=0; k<=n; k++)
        x[k] = 0.0 + 10.0*k/n; // x values evenly spaced in [0, 10]
    double ys[n+1];
    for (k=0; k<=n; k++)
        ys[k] = f(x[k], a0, a1); // true y values
    // Print x and ys arrays
    for (k=0; k<=n; k++)
        printf("%2.2lf ", x[k]);
    printf("\n");
    for (k=0; k<=n; k++)
        printf("%2.2lf ", ys[k]);
    printf("\n");

    // --- Step 2: Add random noise to simulate measurements ---
    srand(clock());
    double y[n+1];
    for (k=0; k<=n; k++)
    {
        double d = rand();
        d /= RAND_MAX;
        y[k] = ys[k] + 2*(-0.5 + d); // noise in [-1, 1]
    }
    // Print noisy measured data
    for (k=0; k<=n; k++)
        printf("%2.2lf ", y[k]);
    printf("\n");

    // --- Step 3: Set up the normal equations for least-squares fit ---
    // Matrix A and vector B for the system A * af = B
	// The normal equations for a linear fit y = a0 + a1*x are:
	// [ N        sum(x_i)   ] [a0] = [ sum(y_i)    ]
	// [ sum(x_i) sum(x_i^2) ] [a1]   [ sum(x_i*y_i) ]
	// where N is the number of data points (n+1 in this case).

	// Matrix A components:
	// A[0][0] = N (number of data points)
	// A[0][1] = sum(x_i) (sum of all x values)
	// A[1][0] = sum(x_i) (sum of all x values)
	// A[1][1] = sum(x_i^2) (sum of squares of x values)
	double A[2][2] =
	{
		{ (double)n + 1, sum(x, n) },      // First row of matrix A
		{ sum(x, n), mulsum(x, x, n) } // Second row of matrix A
	};

	// Vector B components:
	// B[0] = sum(y_i) (sum of all y values)
	// B[1] = sum(x_i*y_i) (sum of element-wise product of x and y values)
	double B[2] =
	{
		sum(y, n),      // First element of vector B
		mulsum(x, y, n) // Second element of vector B
	};

    // --- Step 4: Solve the system by hand (explicit 2x2 inverse) ---
    // The following formulas solve for af[0] (intercept) and af[1] (slope)
    // using the inverse of a 2x2 matrix
    double af[2] =
    {
        ( A[1][1]*B[0] - A[0][1]*B[1])/(A[0][0]*A[1][1] - A[0][1]*A[1][0]),
        (-A[1][0]*B[0] + A[0][0]*B[1])/(A[0][0]*A[1][1] - A[0][1]*A[1][0])
    };
    // Print the system and the hand-solved result
    printm("A" , 2, 2, (double *) A ); // Show matrix A
    printm("B" , 2, 1, (double *) B ); // Show vector B
    printm("af", 2, 1, (double *) af); // Show solution from hand inversion

    // --- Step 5: Solve the system using LAPACK ---
    // LAPACKE_dgels solves least-squares problems for general systems
    // It overwrites B with the solution (af)
    lapack_int info;
	// LAPACKE_dgels solves the linear least-squares system A * af = B.
	// 'A' is the 2x2 matrix derived from the normal equations (containing sums of x, x^2).
	// 'B' is the 2x1 vector derived from the normal equations (containing sums of y, x*y).
	// LAPACKE_dgels finds the 'af' (a0, a1) that minimizes the sum of squared differences
	// between the observed 'y' values and the values predicted by the line a0 + a1*x.
	// The function overwrites 'B' with the solution 'af'.
	if (!(info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', 2, 2, 1, (double *) A, 2, (double *) B, 1)))
		printm("af (LAPACK)", 2, 1, (double *) B ); // Show solution from LAPACK
    // If info != 0, LAPACK failed to solve the system

    return EXIT_SUCCESS;
}

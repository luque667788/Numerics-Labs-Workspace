// math.c - Linear Least-Squares Fit Example
//
// This program demonstrates how to perform a linear least-squares fit to noisy data.
// It generates synthetic data points (x, y) along a line, adds random noise to simulate measurements,
// and then solves for the best-fit line parameters using the normal equations (matrix inversion).
//
// Key functions:
//   printm: Prints a matrix or vector with a label, for debugging and result display.
//   mulsum: Computes the sum of element-wise products of two arrays (dot product).
//   sum: Computes the sum of elements in an array.
//   f: Linear function a1*x + a0, used to generate the true (noise-free) data.
//
// Main steps in main():
//   1. Generate x values evenly spaced in [0, 10].
//   2. Compute true y values (ys) using the linear function.
//   3. Add random noise to ys to simulate measured data (y).
///   4. Print x, ys, and y arrays for inspection.
//   5. Set up the normal equations for least-squares fitting:
//      - Matrix A and vector B are constructed from sums of x and y.
//      - The solution af[] is computed using the formula for the inverse of a 2x2 matrix.
//   6. Print the matrix A, vector B, and the fitted parameters af.
//
// The result af[0] and af[1] are the estimated intercept and slope of the best-fit line.
//
// Notes:
// - The code uses arrays of size n+1 to include both endpoints (0 to n).
// - The random noise is uniformly distributed in [-1, 1].
// - The matrix inversion is done explicitly for the 2x2 case.
//
// This is a good example of basic numerical linear algebra and data fitting in C.

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

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

// Computes the sum of element-wise products (dot product) of two arrays of length n+1
static double mulsum(double x[], double y[], int n)
{
    double res = 0.0;
    int k;
    for (k=0; k<=n; k++)
        res += x[k]*y[k];
    return res;
}

// Computes the sum of elements in an array of length n+1
static double sum(double x[], int n)
{
    double res = 0.0;
    int k;
    for (k=0; k<=n; k++)
        res += x[k];
    return res;
}

// Linear function used to generate true (noise-free) data
static double f(double x, double a0, double a1)
{
    return a1*x + a0;
}

int main(void)
{
    // linear least-squares fit
    double a0 = 0.5;
    double a1 = 0.5;
    int n = 20;
    int k;
    double x[n+1];
    // Generate x values evenly spaced in [0, 10]
    for (k=0; k<=n; k++)
        x[k] = 0.0 + 10.0*k/n;
    double ys[n+1];
    // Compute true y values (without noise)
    for (k=0; k<=n; k++)
        ys[k] = f(x[k], a0, a1);
    // Print x and ys arrays
    for (k=0; k<=n; k++)
        printf("%2.2lf ", x[k]);
    printf("\n");
    for (k=0; k<=n; k++)
        printf("%2.2lf ", ys[k]);
    printf("\n");
    // simulate measured data by adding random noise to ys
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
    // linear least-squares fit using normal equations
    // Matrix A and vector B for the system A * af = B
    double A[2][2] =
    {
        n, sum(x, n),
        sum(x, n), mulsum(x, x, n)
    };
    double B[2] =
    {
        sum(y, n),
        mulsum(x, y, n)
    };
    // Solve the 2x2 system using explicit matrix inversion
    // af[0] = intercept, af[1] = slope
    double af[2] =
    {
        ( A[1][1]*B[0] - A[0][1]*B[1])/(A[0][0]*A[1][1] - A[0][1]*A[1][0]),
        (-A[1][0]*B[0] + A[0][0]*B[1])/(A[0][0]*A[1][1] - A[0][1]*A[1][0])
    };
    // Print the system and the solution
    printm("A" , 2, 2, (double *) A );
    printm("B" , 2, 1, (double *) B );
    printm("af", 2, 1, (double *) af);
    return EXIT_SUCCESS;
}

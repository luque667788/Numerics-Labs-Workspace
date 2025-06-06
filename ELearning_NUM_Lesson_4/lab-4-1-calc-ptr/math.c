// math.c - Linear Least-Squares Fit with LAPACK and Pointer-Based Arrays
//
// This program demonstrates how to perform a linear least-squares fit to noisy data in C.
// It solves the problem in two ways:
//   1. By hand: Using the explicit formula for the inverse of a 2x2 matrix.
//   2. Using LAPACK: Leveraging the DGELS routine for a robust and general solution.
//
// Key features of this version:
//   - Data generation: Creates synthetic data points (x, y) along a known line.
//   - Noise simulation: Adds random noise to the y values to mimic real-world measurements.
//   - Normal equations: Sets up the standard matrix system (A * af = B) for least-squares.
//   - Pointer-based arrays: Uses flat arrays (pA, pB) for matrix data, which is common
//     when interfacing with libraries like LAPACK. These are then cast to C-style
//     2D arrays for the manual calculation part for easier indexing.
//   - LAPACK integration: Shows how to call LAPACKE_dgels to solve the system.
//   - Output: Prints the matrices, vectors, and the solutions from both methods for comparison.
//
// This example is useful for understanding how to handle matrix operations in C,
// interface with numerical libraries, and apply least-squares fitting.

#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include <lapacke.h>

static void printm(char *name, int m, int n, double *pA)
{
    int i, j;
    // Print a matrix or vector with a label for debugging
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
    // Here, we use flat arrays (pA, pB) for LAPACK compatibility
    double pA[2*2] =
    {
        n, sum(x, n),
        sum(x, n), mulsum(x, x, n)
    };
    double pB[1*2] =
    {
        sum(y, n),
        mulsum(x, y, n)
    };

    // --- Step 4: Solve the system by hand (explicit 2x2 inverse) ---
    // Cast flat arrays to C 2D/1D arrays for easier access
    double (*A)[2] = (double (*)[2]) pA;
    double (*B)    = (double (*)   ) pB;
    // Compute the solution using the 2x2 matrix inverse formula
    double paf[1*2] =
    {
        ( A[1][1]*B[0] - A[0][1]*B[1])/(A[0][0]*A[1][1] - A[0][1]*A[1][0]),
        (-A[1][0]*B[0] + A[0][0]*B[1])/(A[0][0]*A[1][1] - A[0][1]*A[1][0])
    };
    // Print the system and the hand-solved result
    printm("A" , 2, 2, pA ); // Show matrix A
    printm("B" , 2, 1, pB ); // Show vector B
    printm("af", 2, 1, paf); // Show solution from hand inversion

    // --- Step 5: Solve the system using LAPACK ---
    // LAPACKE_dgels solves least-squares problems for general systems
    // It overwrites pB with the solution (af)
    lapack_int info;
    if (!(info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', 2, 2, 1, pA, 2, pB, 1)))
        printm("af", 2, 1, pB ); // Show solution from LAPACK
    // If info != 0, LAPACK failed to solve the system

    return EXIT_SUCCESS;
}

// math.c - Linear least-squares fit for ellipse-like data using LAPACK
// General Overview:
// This program generates synthetic data points (x, y) that follow a parametric ellipse with added noise.
// It then constructs a design matrix A and a right-hand side vector B to perform a linear least-squares fit
// using LAPACK's DGELS routine, solving for parameters of a quadratic curve that best fits the noisy data.
// The program prints the generated data, the design matrix, and the solution vector.

#include <stdio.h> // Standard I/O functions
#include <stdlib.h> // Standard library (for rand, srand, EXIT_SUCCESS)
#include <math.h>   // Math functions (cos, sin, acos)
#include <time.h>   // Time functions (for seeding random number generator)
#include <lapacke.h> // LAPACK C interface for linear algebra

// Helper function to print a matrix with a name
static void printm(char *name, int m, int n, double *pA)
{
    int i, j;
    printf("\n %s\n", name); // Print matrix name
    for (i=0; i<m; i++)
    {
        for (j=0; j<n; j++)
            printf(" %6.2lf", pA[i*n+j]); // Print each element
        printf("\n");
    }
}

// Parametric function for x-coordinate of ellipse
static double fx(double t, double a, double b, double th, double x0)
{
    // a, b: ellipse axes; th: rotation; x0: center x; t: parameter
    return a*cos(th)*cos(t) - b*sin(th)*sin(t) + x0;
}

// Parametric function for y-coordinate of ellipse
static double fy(double t, double a, double b, double th, double y0)
{
    // a, b: ellipse axes; th: rotation; y0: center y; t: parameter
    return a*sin(th)*sin(t) + b*cos(th)*cos(t) + y0;
}

int main(void)
{
    // --- 1. Set ellipse parameters ---
    double a  = 2.0; // Major axis
    double b  = 1.5; // Minor axis
    double th = acos(-1.0)/8.0; // Rotation angle (pi/8)
    double x0 = 2.0; // Center x
    double y0 = 0.0; // Center y

    int n = 100; // Number of data points (n+1 total)
    int k;
    double ts[n+1]; // Parameter t for each point
    for (k=0; k<=n; k++)
        ts[k] = 0.0 + 2.0*acos(-1.0)*k/n; // t from 0 to 2*pi
    
    // Print t values
    for (k=0; k<=n; k++)
        printf("%2.2lf ", ts[k]);
    printf("\n");

    // --- 2. Generate ideal (noise-free) ellipse points ---
    double xs[n+1];
    for (k=0; k<=n; k++)
        xs[k] = fx(ts[k], a, b, th, x0); // x-coordinates
    double ys[n+1];
    for (k=0; k<=n; k++)
        ys[k] = fy(ts[k], a, b, th, y0); // y-coordinates
    
    // Print ideal x and y
    for (k=0; k<=n; k++)
        printf("%2.2lf ", xs[k]);
    printf("\n");
    for (k=0; k<=n; k++)
        printf("%2.2lf ", ys[k]);
    printf("\n");

    // --- 3. Simulate measured data by adding random noise ---
    srand(clock()); // Seed RNG
    double x[n+1];
    for (k=0; k<=n; k++)
    {
        double d = rand();
        d /= RAND_MAX; // Uniform [0,1]
        x[k] = xs[k] + 0.5*(-0.5 + d); // Add noise in [-0.25, 0.25]
    }
    srand(clock()); // Reseed for y
    double y[n+1];
    for (k=0; k<=n; k++)
    {
        double d = rand();
        d /= RAND_MAX;
        y[k] = ys[k] + 0.5*(-0.5 + d); // Add noise in [-0.25, 0.25]
    }

    // Print noisy x and y
    for (k=0; k<=n; k++)
        printf("%2.2lf ", x[k]);
    printf("\n");
    for (k=0; k<=n; k++)
        printf("%2.2lf ", y[k]);
    printf("\n");

    // --- 4. Build design matrix A for quadratic fit ---
    // Each row: [x^2, y^2, x*y, x, y]
    double A[n+1][5];
    for (k=0; k<=n; k++)
    {
        A[k][0] = x[k]*x[k]; // x^2
        A[k][1] = y[k]*y[k]; // y^2
        A[k][2] = x[k]*y[k]; // x*y
        A[k][3] = x[k];      // x
        A[k][4] = y[k];      // y
    }

    // --- 5. Build right-hand side vector B (all ones) ---
    double B[n+1];
    for (k=0; k<=n; k++)
        B[k] = 1.0;

    // Print matrices A and B
    printm("A", n, 5, (double *) A); // Print first n rows of A (n x 5)
    printm("B", n, 1, (double *) B); // Print first n rows of B (n x 1)

    // --- 6. Solve least-squares problem A*p = B using LAPACK DGELS ---
    lapack_int info;
    // LAPACKE_dgels: solves min_p ||A*p - B||_2
    // Arguments: row-major, no transpose, n rows, 5 cols, 1 rhs, A, lda=5, B, ldb=1
    if (!(info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', n, 5, 1, (double *) A, 5, (double *) B, 1)))
        printm("p", 5, 1, (double *) B); // Print solution vector p (coefficients)
    
    return EXIT_SUCCESS;
}

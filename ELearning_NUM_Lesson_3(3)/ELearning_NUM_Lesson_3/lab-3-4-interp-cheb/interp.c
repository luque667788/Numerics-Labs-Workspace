/*
 * interp.c - Chebyshev Interpolation with Barycentric Lagrange Formula
 *
 * GENERAL OVERVIEW:
 * This program demonstrates polynomial interpolation using Chebyshev nodes and the barycentric form of the Lagrange interpolation formula.
 *
 * - The function f(x) = 1/(1+16x^2) is interpolated on [-1,1] using n+1 Chebyshev nodes (roots of Chebyshev polynomials).
 * - The barycentric weights for these nodes are computed for efficient and numerically stable interpolation.
 * - The program evaluates the interpolating polynomial at a test point and at many points for plotting, comparing the true function and the interpolant.
 * - It outputs data files for plotting the function, the interpolant, and the interpolation nodes.
 *
 * MATHEMATICAL BACKGROUND:
 * - Chebyshev nodes minimize interpolation error and avoid Runge's phenomenon.
 * - The barycentric Lagrange formula allows fast and stable evaluation of the interpolating polynomial.
 * - The weights are alternately +1 and -1 (with 0.5 at the endpoints) for Chebyshev nodes.
 * - The program demonstrates the accuracy of Chebyshev interpolation and provides data for visualization.
 */

#include <stdio.h> // For printf, FILE, fopen, fprintf, etc.
#include <stdlib.h> // For EXIT_SUCCESS, EXIT_FAILURE
#include <math.h>   // For cos, acos, pow, fabs

#define e 0.000000000000001 // Small epsilon for floating point comparison

// LagrangeInterp1D: Barycentric Lagrange interpolation formula
// fvals[]: function values at nodes
// xnodes[]: interpolation nodes
// n: degree (number of nodes - 1)
// barw[]: barycentric weights
// t: evaluation point
// Returns: interpolated value at t
static double LagrangeInterp1D(double fvals[], double xnodes[], int n, double barw[], double t)
{
    double numt = 0.0;   // Numerator of barycentric formula
    double denomt = 0.0; // Denominator of barycentric formula
    int j;
    for (j=0; j<=n; j++)
    {
        double tdiff = t - xnodes[j]; // Distance from t to node j
        numt += barw[j] / tdiff * fvals[j]; // Add weighted contribution to numerator
        denomt += + barw[j] / tdiff;        // Add weighted contribution to denominator
        // If t is (numerically) equal to a node, return f(node)
        if (fabs(tdiff) < e)
        {
            numt = fvals[j];
            denomt = 1.0;
            break;
        }
    }
    return numt / denomt; // Barycentric interpolation value
}

// f(x): Function to interpolate
// Here, f(x) = 1/(1+16x^2), a classic test function for interpolation
static double f(double x)
{
    return 1.0/(1.0 + 16.0*x*x);
}

int main(void)
{
    int n = 32; // Degree of interpolating polynomial (number of nodes - 1)
    int k;
    // ClosedChebNodes: Compute Chebyshev nodes on [-1,1]
    double xnodes[n+1];
    for (k=0; k<=n; k++) xnodes[k] = k; // Initialize with indices
    for (k=0; k<=n; k++) xnodes[k] = cos(xnodes[k]*acos(-1.0)/n); // Chebyshev nodes formula
    // ClosedChebBarWeights: Compute barycentric weights for Chebyshev nodes
    double w[n+1];
    w[0] = 0.5; // Endpoints get weight 0.5
    for (k=1; k<=n-1; k++)
        w[k] = pow(-1.0, k); // Interior nodes alternate +1/-1
    w[n] = 0.5*pow(-1.0, n); // Last endpoint weight
    // Sample f at nodes
    double f_sample[n+1];
    for (k=0; k<=n; k++)
        f_sample[k] = f(xnodes[k]);
    // Test interpolation at t = -1.0
    double t = -1.0;
    double f_interp = LagrangeInterp1D(f_sample, xnodes, n, w, t);
    printf("LagrangeInterp1D(% 1.2f) = % 1.2f\n", t, f_interp);
    // --- Output data for plotting ---
    FILE *fp = fopen("interp_plot.dat", "w"); // Open file for plot data
    if (!fp) {
        perror("Could not open interp_plot.dat for writing");
        return EXIT_FAILURE;
    }
    int Nplot = 500; // Number of plot points
    for (int i = 0; i <= Nplot; ++i) {
        double xp = -1.0 + 2.0 * i / Nplot; // Uniform points in [-1,1]
        double fp_true = f(xp); // True function value
        double fp_interp = LagrangeInterp1D(f_sample, xnodes, n, w, xp); // Interpolated value
        fprintf(fp, "% .10f % .10f % .10f\n", xp, fp_true, fp_interp); // Write to file
    }
    fclose(fp);
    // Output nodes for plotting
    FILE *fpn = fopen("interp_nodes.dat", "w"); // Open file for node data
    if (!fpn) {
        perror("Could not open interp_nodes.dat for writing");
        return EXIT_FAILURE;
    }
    for (k = 0; k <= n; ++k) {
        fprintf(fpn, "% .10f % .10f\n", xnodes[k], f_sample[k]); // Write node and value
    }
    fclose(fpn);
    // Print plotting instructions
    printf("Data for plotting written to interp_plot.dat and interp_nodes.dat\n");
    printf("You can plot with gnuplot using:\n");
    printf("  gnuplot -persist -e \"plot 'interp_plot.dat' u 1:2 w l title 'f(x)', \\\n");
    printf("    'interp_plot.dat' u 1:3 w l title 'Interpolation', \\\n");
    printf("    'interp_nodes.dat' u 1:2 w p pt 7 ps 1.5 lc rgb 'red' title 'Nodes'\"\n");
    return EXIT_SUCCESS;
}

/*
 * interp.c
 *
 * General Overview:
 * This program demonstrates barycentric Lagrange interpolation using equispaced nodes for the function f(x) = 1/(1+16x^2).
 * It computes the interpolating polynomial at a specific point (t = -1.0) and prints the result.
 *
 * Mathematical Concepts:
 * - Lagrange interpolation: Constructs a polynomial that passes through a given set of points (nodes).
 * - Barycentric form: A numerically stable and efficient way to evaluate the Lagrange interpolating polynomial using precomputed weights.
 * - Equispaced nodes: Nodes are evenly spaced in the interval [-1, 1].
 * - Barycentric weights for equispaced nodes: w_k = (-1)^k * binomial(n, k)
 *
 * Code Structure:
 * - Implements barycentric Lagrange interpolation (LagrangeInterp1D)
 * - Computes factorial and binomial coefficients for weights
 * - Defines the function to interpolate
 * - Sets up nodes, weights, samples the function, and evaluates the interpolation at t = -1.0
 */

#include <stdio.h> // Standard I/O functions
#include <stdlib.h> // Standard library (for EXIT_SUCCESS)
#include <math.h> // Math functions (fabs, pow)

#define e 0.000000000000001 // Small epsilon for floating point comparison

// Barycentric Lagrange interpolation for 1D data
// fvals: function values at nodes
// xnodes: interpolation nodes (equispaced)
// n: degree
// barw: barycentric weights
// t: interpolation point
static double LagrangeInterp1D(double fvals[], double xnodes[], int n, double barw[], double t)
{
    double numt = 0.0; // Numerator
    double denomt = 0.0; // Denominator
    int j;
    for (j=0; j<=n; j++)
    {
        double tdiff = t - xnodes[j]; // Distance from node
        numt += barw[j] / tdiff * fvals[j]; // Add to numerator
        denomt += barw[j] / tdiff; // Add to denominator
        if (fabs(tdiff) < e) // If t is exactly at a node, use node value
        {
            numt = fvals[j];
            denomt = 1.0;
            break;
        }
    }
    return numt / denomt; // Return interpolated value
}

// Compute n! (factorial)
static long long factorial(int n)
{
    long long res = 1;
    int k;
    for (k=2; k<=n; k++)
        res *= k; // Multiply by each integer up to n
    return res;
}

// Compute binomial coefficient (n choose k)
static long long binomial(int n, int k)
{
    long long a = factorial(n); // n!
    long long b = factorial(k); // k!
    long long c = factorial(n-k); // (n-k)!
    long long res = a / b / c; // n! / (k! * (n-k)!)
    return res;
}

// Function to interpolate: f(x) = 1/(1+16x^2)
static double f(double x)
{
    return 1.0/(1.0 + 16.0*x*x);
}

int main(void)
{
    int n = 15; // Degree of interpolation (number of nodes - 1)
    int k;
    // EquispacedNodes: Compute equispaced nodes in [-1,1]
    double xnodes[n+1];
    for (k=0; k<=n; k++) xnodes[k] = k; // Fill with indices
    for (k=0; k<=n; k++) xnodes[k] = -1.0 + xnodes[k]*2.0/n; // Map to [-1,1]
    // EquispacedBarWeights: Compute barycentric weights for equispaced nodes
    double w[n+1];
    for (k=0; k<=n; k++)
        w[k] = pow(-1.0, k) * binomial(n, k); // w_k = (-1)^k * binomial(n, k)
    
    double f_sample[n+1];
    for (k=0; k<=n; k++)
        f_sample[k] = f(xnodes[k]); // Sample function at nodes

    double t = -1.0; // Interpolation point
    double f_interp = LagrangeInterp1D(f_sample, xnodes, n, w, t); // Interpolated value

    printf("LagrangeInterp1D(% 1.2f) = % 1.2f\n", t, f_interp); // Print result

    // --- Output data for plotting ---
    FILE *fp = fopen("interp_plot.dat", "w");
    if (!fp) {
        perror("Could not open interp_plot.dat for writing");
        return EXIT_FAILURE;
    }
    int Nplot = 500;
    for (int i = 0; i <= Nplot; ++i) {
        double xp = -1.0 + 2.0 * i / Nplot;
        double fp_true = f(xp);
        double fp_interp = LagrangeInterp1D(f_sample, xnodes, n, w, xp);
        fprintf(fp, "% .10f % .10f % .10f\n", xp, fp_true, fp_interp);
    }
    fclose(fp);

    // Output nodes for plotting
    FILE *fpn = fopen("interp_nodes.dat", "w");
    if (!fpn) {
        perror("Could not open interp_nodes.dat for writing");
        return EXIT_FAILURE;
    }
    for (k = 0; k <= n; ++k) {
        fprintf(fpn, "% .10f % .10f\n", xnodes[k], f_sample[k]);
    }
    fclose(fpn);

    // ---
    // The code above writes two files:
    //   interp_plot.dat: contains x, f(x), and interpolated values for many points in [-1,1]
    //   interp_nodes.dat: contains the interpolation nodes and their function values
    //
    // The following gnuplot command (printed below) will plot:
    //   - The true function f(x) as a line (column 2 from interp_plot.dat)
    //   - The interpolation as a line (column 3 from interp_plot.dat)
    //   - The interpolation nodes as red points (from interp_nodes.dat)
    //
    // You can copy-paste the printed gnuplot command to visualize the results.

    printf("Data for plotting written to interp_plot.dat and interp_nodes.dat\n");
    printf("You can plot with gnuplot using:\n");
    printf("  gnuplot -persist -e \"plot 'interp_plot.dat' u 1:2 w l title 'f(x)', \\\n");
    printf("    'interp_plot.dat' u 1:3 w l title 'Interpolation', \\\n");
    printf("    'interp_nodes.dat' u 1:2 w p pt 7 ps 1.5 lc rgb 'red' title 'Nodes'\"\n");

    return EXIT_SUCCESS;
}

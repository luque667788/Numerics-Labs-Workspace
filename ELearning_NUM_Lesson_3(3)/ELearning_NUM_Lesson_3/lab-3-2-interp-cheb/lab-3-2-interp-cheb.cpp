/*
 * lab-3-2-interp-cheb.cpp
 *
 * General Overview:
 * This program)j visualizes the interpolation of the function f(x) = 1/(1+16x^2) using Chebyshev nodes and barycentric Lagrange interpolation.
 * It uses the FLTK library to create a window and draw two curves:
 *   - The original function (in blue)
 *   - The interpolated function (in red) using Chebyshev nodes
 * Chebyshev nodes are used to minimize Runge's phenomenon and provide better interpolation accuracy at the interval edges.
 * The barycentric weights are used for efficient and numerically stable Lagrange interpolation.
 * The program samples both the original and interpolated functions at many points and draws them as lines, also marking the Chebyshev nodes.
 *
 * Mathematical Concepts:
 * - Chebyshev nodes: x_k = cos(pi * k / n), k = 0..n, are used for interpolation to reduce oscillations at the interval edges.
 * - Barycentric Lagrange interpolation: A numerically stable way to compute the Lagrange interpolating polynomial using precomputed weights.
 * - The function f(x) = 1/(1+16x^2) is a classic test function for interpolation, as it is smooth but exhibits Runge's phenomenon with equispaced nodes.
 *
 * The code is organized as follows:
 * - Definition of a GRAPH struct to hold sampled points.
 * - Definition of a custom FLTK widget (GRAPHBOX) to draw the graph.
 * - Implementation of barycentric Lagrange interpolation.
 * - Main function: computes Chebyshev nodes, barycentric weights, samples the function and its interpolation, and runs the FLTK event loop.
 */

#include <FL/Fl.H> // FLTK core header
#include <FL/Fl_Double_Window.H> // FLTK double-buffered window
#include <FL/Fl_Box.H> // FLTK box widget
#include <FL/fl_draw.H> // FLTK drawing functions

#include <math.h> // math functions

#define GRAPH_MAX 100 // Maximum number of points to plot

// Structure to hold graph data: x, y1 (original), y2 (interpolated), and number of points
struct GRAPH
{
    double  x[GRAPH_MAX+1]; // x-coordinates
    double y1[GRAPH_MAX+1]; // y-coordinates for original function
    double y2[GRAPH_MAX+1]; // y-coordinates for interpolated function
    int n; // number of points
};

static struct GRAPH Graph; // Global graph data

// Custom FLTK widget to draw the graph
class GRAPHBOX : public Fl_Box
{
    private : void draw(void)
    {
        // redraw background
        Fl_Box::draw();

        // Draw the original function in blue
        fl_color(fl_rgb_color(0, 0, 255));

        // Set initial line start coordinates at the left middle of the box
        int xk0 = x();
        int yk0 = y() + h()/2;

        int n;
        for (n = 0; n < Graph.n; n++)
        {
            // Calculate line end coordinates for the next point
            int xk1 = x() + 250*(1 + Graph.x[n]); // Map x in [-1,1] to [0,500]
            int yk1 = y() + h()/2 - 250*Graph.y1[n]; // Map y in [-1,1] to vertical position

            fl_line(xk0, yk0, xk1, yk1); // Draw line segment

            // Next start coordinates become current end coordinates
            xk0 = xk1;
            yk0 = yk1;
        }

        // Draw the interpolated function in red
        fl_color(fl_rgb_color(255, 0, 0));

        // Reset line start coordinates
        xk0 = x();
        yk0 = y() + h()/2;

        for (n = 0; n < Graph.n; n++)
        {
            // Calculate line end coordinates for the next point (interpolated)
            int xk1 = x() + 250*(1 + Graph.x[n]);
            int yk1 = y() + h()/2 - 250*Graph.y2[n];

            fl_line(xk0, yk0, xk1, yk1); // Draw line segment

            // Next start coordinates become current end coordinates
            xk0 = xk1;
            yk0 = yk1;
        }

        // Draw circles at the Chebyshev nodes (interpolated points)
        for (n = 0; n < Graph.n; n++)
        {
            int xk = x() + 250*(1 + Graph.x[n]);
            int yk = y() + h()/2 - 250*Graph.y2[n];

            fl_begin_line();
            fl_circle(xk, yk, 5); // Draw a small circle
            fl_end_line();
        }

        // Draw axes and ticks
        fl_color(fl_rgb_color(0, 0, 0));
        fl_font(FL_COURIER, 16);
        char str[256];

        // x-axis
        fl_line(x(), y()+h()/2, x()+w(), y()+h()/2);
        // x-ticks and labels
        for (int xpos=0; xpos<=10; xpos++)
        {
            fl_line(x()+50*xpos, y()+h()/2-5, x()+50*xpos, y()+h()/2+5); // Tick
            sprintf(str, "%1.1lf", -1.0+((double) xpos)/5.0); // Label value
            fl_draw(str, x()+50*xpos, y()+h()/2+20); // Draw label
        }
        // y-axis
        fl_line(x(), y(), x(), 10+h());
        // y-ticks and labels
        fl_draw("+1", x(), y()+h()/2-250+10); // Top label
        fl_draw("-1", x(), y()+h()/2+250); // Bottom label
    }

    public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
    {
        box(FL_FLAT_BOX); // Flat background
        color(fl_rgb_color(127, 127, 127)); // Gray background
    }
};

#define e 0.000000000000001 // Small epsilon for floating point comparison

// Barycentric Lagrange interpolation for 1D data
// fvals: function values at nodes
// xnodes: interpolation nodes (Chebyshev)
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
        denomt += + barw[j] / tdiff; // Add to denominator

        if (fabs(tdiff) < e) // If t is exactly at a node, use node value
        {
            numt = fvals[j];
            denomt = 1.0;
            break;
        }
    }
    return numt / denomt; // Return interpolated value
}

// Function to interpolate: f(x) = 1/(1+16x^2)
static double f(double x)
{
    return 1.0/(1.0 + 16.0*x*x);
}

int main(void)
{
    Fl_Double_Window window(530, 530, "Graph"); // Create window
    GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20); // Create graph box inside window

    window.show(); // Show window

    int n = 32; // Degree of interpolation (number of Chebyshev nodes - 1)
    int k;
    // ClosedChebNodes: Compute Chebyshev nodes in [-1,1]
    double xnodes[n+1];
    for (k=0; k<=n; k++) xnodes[k] = k; // Fill with indices
    for (k=0; k<=n; k++) xnodes[k] = cos(xnodes[k]*acos(-1.0)/n); // Compute Chebyshev nodes
    // ClosedChebBarWeights: Compute barycentric weights
    double w[n+1];
    w[0] = 0.5;
    for (k=1; k<=n-1; k++)
        w[k] = pow(-1.0, k); // Alternate sign
    w[n] = 0.5*pow(-1.0, n); // Last weight
    
    double f_sample[n+1];
    for (k=0; k<=n; k++)
        f_sample[k] = f(xnodes[k]); // Sample function at Chebyshev nodes

    int i = 0;
    double t;
    // Sample both original and interpolated function at 51 points in [-1,1]
    for (t = -1.0; t <= 1.0 + e; t += 1.0/25.0)
    {
        double x = t;
        double y = f(x); // True function value
        double f_interp = LagrangeInterp1D(f_sample, xnodes, n, w, t); // Interpolated value

        Graph.x[i] = x;
        Graph.y1[i] = y;
        Graph.y2[i] = f_interp;
        i++;
    }
    Graph.n = i; // Number of points sampled

    Fl::run(); // Start FLTK event loop
    return EXIT_SUCCESS;
}

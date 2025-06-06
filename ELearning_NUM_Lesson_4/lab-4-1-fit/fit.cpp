// fit.cpp - Visual Linear Least-Squares Fit with FLTK
//
// This program demonstrates linear least-squares fitting and visualizes the results using the FLTK GUI library.
//
// General Overview:
// - Generates synthetic data points (x, y) along a line, adds random noise to simulate measurements.
// - Computes the best-fit line using the normal equations and explicit matrix inversion.
// - Visualizes the true line, noisy data, and fitted line in a GUI window.
//
// Key Components:
// 1. Data Structures:
//    - struct GRAPH: Holds arrays for x values, true y (y1), noisy y (y2), fitted y (y3), and the number of points.
// 2. Visualization:
//    - GRAPHBOX: Custom FLTK widget that draws the graph, axes, and tick marks.
//    - Colors: Red for the true line, blue for noisy data (as circles), cyan for the fitted line.
// 3. Data Generation:
//    - x[]: Evenly spaced in [0, 10].
//    - ys[]: True y values from the model.
//    - y[]: Noisy measurements (ys + random noise in [-1, 1]).
// 4. Least-Squares Fit:
//    - Sets up the normal equations (A * af = B) for the best-fit line.
//    - Solves for af (intercept and slope) using the explicit 2x2 matrix inverse.
//    - Computes fitted y values (yf[]) using the found parameters.
// 5. Data Assignment:
//    - Populates the GRAPH struct with all x, y1, y2, y3 values for visualization.
// 6. GUI:
//    - Opens a window and runs the FLTK event loop to display the graph.
//
// This example is useful for learning how to combine numerical methods with interactive visualization in C++.
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include <time.h>

#define GRAPH_MAX 100 // Maximum number of data points

// Data structure to hold all graph data
struct GRAPH {
    double x[GRAPH_MAX+1];   // x values
    double y1[GRAPH_MAX+1];  // true y values (model)
    double y2[GRAPH_MAX+1];  // noisy y values (measured)
    double y3[GRAPH_MAX+1];  // fitted y values (from least-squares)
    int n;                   // number of points
};

static struct GRAPH Graph; // Global graph data

// Custom FLTK widget for drawing the graph
class GRAPHBOX : public Fl_Box {
private:
    void draw(void) {
        // Redraw background
        Fl_Box::draw();

        // Draw the true line (red)
        fl_color(fl_rgb_color(255, 0, 0));
        int xk0 = x(); // initial x pixel
        int yk0 = y() + h()/2; // initial y pixel (centered vertically)
        int n;
        for (n = 0; n < Graph.n; n++) {
            int xk1 = x() + 50*Graph.x[n]; // scale x for display
            int yk1 = y() + h()/2 - 25*Graph.y1[n]; // scale y for display
            fl_line(xk0, yk0, xk1, yk1); // draw line segment
            xk0 = xk1; // update start for next segment
            yk0 = yk1;
        }

        // Draw noisy data points (blue circles)
        fl_color(fl_rgb_color(0, 0, 255));
        for (n = 0; n < Graph.n; n++) {
            int xk = x() + 50*Graph.x[n];
            int yk = y() + h()/2 - 25*Graph.y2[n];
            fl_begin_line();
            fl_circle(xk, yk, 5); // draw circle of radius 5
            fl_end_line();
        }

        // Draw fitted line (cyan)
        fl_color(fl_rgb_color(0, 255, 255));
        xk0 = x();
        yk0 = y() + h()/2;
        for (n = 0; n < Graph.n; n++) {
            int xk1 = x() + 50*Graph.x[n];
            int yk1 = y() + h()/2 - 25*Graph.y3[n];
            fl_line(xk0, yk0, xk1, yk1);
            xk0 = xk1;
            yk0 = yk1;
        }

        // Draw axes and ticks
        fl_color(fl_rgb_color(0, 0, 0));
        fl_font(FL_COURIER, 16);
        char str[256];
        fl_line(x(), y()+h()/2, x()+w(), y()+h()/2); // x-axis
        for (int xpos=0; xpos<=10; xpos++) {
            fl_line(x()+50*xpos, y()+h()/2-5, x()+50*xpos, y()+h()/2+5); // x-tick
            sprintf(str, "%i", xpos);
            fl_draw(str, x()+50*xpos, y()+h()/2+20); // x-label
        }
        fl_line(x(), y(), x(), 10+h()); // y-axis
        fl_draw("+10", x(), y()+h()/2-250+10); // y-label top
        fl_draw("-10", x(), y()+h()/2+250);    // y-label bottom
    }
public:
    GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l) {
        box(FL_FLAT_BOX);
        color(fl_rgb_color(127, 127, 127));
    }
};

// Helper function: sum of element-wise products (dot product)
static double mulsum(double x[], double y[], int n) {
    double res = 0.0;
    int k;
    for (k=0; k<=n; k++)
        res += x[k]*y[k];
    return res;
}

// Helper function: sum of elements in an array
static double sum(double x[], int n) {
    double res = 0.0;
    int k;
    for (k=0; k<=n; k++)
        res += x[k];
    return res;
}

// Linear function for generating true (noise-free) data
static double f(double x, double a0, double a1) {
    return a1*x + a0;
}

int main(void) {
    // Create FLTK window and graph widget
    Fl_Double_Window window(530, 530, "Graph");
    GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);
    window.show();

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

    // --- Step 2: Add random noise to simulate measurements ---
    srand(clock());
    double y[n+1];
    for (k=0; k<=n; k++) {
        double d = rand();
        d /= RAND_MAX;
        y[k] = ys[k] + 2*(-0.5 + d); // noise in [-1, 1]
    }

    // --- Step 3: Set up the normal equations for least-squares fit ---
    // Matrix A and vector B for the system A * af = B
    double A[2][2] = {
        (double) n, sum(x, n),
        sum(x, n), mulsum(x, x, n)
    };
    double B[2] = {
        sum(y, n),
        mulsum(x, y, n)
    };

    // --- Step 4: Solve the system by hand (explicit 2x2 inverse) ---
    // The following formulas solve for af[0] (intercept) and af[1] (slope)
    // using the inverse of a 2x2 matrix
    double af[2] = {
        ( A[1][1]*B[0] - A[0][1]*B[1])/(A[0][0]*A[1][1] - A[0][1]*A[1][0]),
        (-A[1][0]*B[0] + A[0][0]*B[1])/(A[0][0]*A[1][1] - A[0][1]*A[1][0])
    };

    // --- Step 5: Compute fitted y values using the found parameters ---
    double yf[n+1];
    for (k=0; k<=n; k++)
        yf[k] = f(x[k], af[0], af[1]);

    // --- Step 6: Assign all data to the GRAPH struct for visualization ---
    for (k=0; k<=n; k++) {
        Graph.x[k]  = x[k];    // x values
        Graph.y1[k] = ys[k];   // true y values
        Graph.y2[k] = y[k];    // noisy y values
        Graph.y3[k] = yf[k];   // fitted y values
    }
    Graph.n = k; // number of points

    // --- Step 7: Start the FLTK event loop to display the graph ---
    Fl::run();
    return EXIT_SUCCESS;
}

/*
 * ELLIPSE FITTING PROGRAM USING LEAST SQUARES METHOD
 * ===================================================
 * 
 * GENERAL OVERVIEW:
 * This program demonstrates ellipse fitting using the least squares method with LAPACK.
 * It generates synthetic ellipse data with added noise, then uses linear algebra to fit
 * an ellipse to the noisy data points. The program visualizes:
 * 1. The original theoretical ellipse (red line)
 * 2. The noisy measured data points (blue circles)  
 * 3. The fitted ellipse contour (cyan points)
 * 
 * MATHEMATICAL APPROACH:
 * - Uses the general conic equation: Ax² + By² + Cxy + Dx + Ey + F = 0
 * - For ellipse fitting, we set F = -1 and solve for [A,B,C,D,E]
 * - Constructs overdetermined system: [x² y² xy x y] * [A B C D E]' = 1
 * - Solves using LAPACK's DGELS (least squares solver)
 * - Finds ellipse contour by evaluating the fitted equation
 * 
 * VISUALIZATION:
 * - Uses FLTK graphics library for real-time display
 * - Custom GRAPHBOX widget handles all drawing operations
 * - Interactive window showing all three data sets simultaneously
 */

// FLTK GUI library headers - Fast Light Toolkit for cross-platform GUI
#include <FL/Fl.H>              // Main FLTK header
#include <FL/Fl_Double_Window.H> // Double-buffered window for smooth graphics
#include <FL/Fl_Box.H>           // Basic widget container
#include <FL/fl_draw.H>          // Drawing functions (lines, circles, text)

// Standard C math and utility libraries
#include <math.h>    // Mathematical functions (cos, sin, acos)
#include <time.h>    // Time functions for random seed

// LAPACKE - C interface to LAPACK linear algebra library
#include <lapacke.h> // For solving least squares problems

// Maximum number of data points that can be stored in graph arrays
#define	GRAPH_MAX		1000

/*
 * GRAPH STRUCTURE - Container for three data sets
 * ===============================================
 * Stores three different representations of ellipse data:
 * - (x1,y1): Original theoretical ellipse points (red line)
 * - (x2,y2): Noisy measured data points (blue circles)
 * - (x3,y3): Fitted ellipse contour points (cyan dots)
 */
struct GRAPH
{
	double x1[GRAPH_MAX+1];  // X coordinates of theoretical ellipse
	double y1[GRAPH_MAX+1];  // Y coordinates of theoretical ellipse
	double x2[GRAPH_MAX+1];  // X coordinates of noisy data
	double y2[GRAPH_MAX+1];  // Y coordinates of noisy data
	double x3[GRAPH_MAX+1];  // X coordinates of fitted ellipse
	double y3[GRAPH_MAX+1];  // Y coordinates of fitted ellipse
	int n;                   // Number of valid data points
};

// Global instance of graph data structure
static struct GRAPH Graph;

/*
 * GRAPHBOX CLASS - Custom FLTK widget for graphics display
 * ========================================================
 * Inherits from Fl_Box and overrides draw() method to render:
 * - Coordinate axes with tick marks and labels
 * - Three different data visualizations
 * - Automatic scaling and positioning
 */
class GRAPHBOX : public Fl_Box
{
    private : void draw(void)
	{
		// STEP 1: Redraw the widget background (gray color)
		Fl_Box::draw();

		// STEP 2: Draw the theoretical ellipse as connected red line segments
		fl_color(fl_rgb_color(255, 0, 0));  // Set color to red

		// Starting point for line drawing
		int xk0 = x();                      // Widget's left edge
		int yk0 = y() + h()/2;             // Widget's vertical center

		int n;
		// Draw connected line segments for theoretical ellipse
		for (n = 0; n < Graph.n; n++)
		{
			// Convert data coordinates to screen coordinates
			// Scale by 100 and offset by widget position
			int xk1 = x() + 100*Graph.x1[n];        // Scale X coordinate
			int yk1 = y() + h()/2 - 100*Graph.y1[n]; // Scale Y (flip for screen coords)

			// Draw line segment from previous point to current point
			fl_line(xk0, yk0, xk1, yk1);

			// Update starting point for next line segment
			xk0 = xk1;
			yk0 = yk1;
		}

		// STEP 3: Draw noisy data points as blue circles
		fl_color(fl_rgb_color(0, 0, 255));  // Set color to blue

		for (n = 0; n < Graph.n; n++)
		{
			// Convert data coordinates to screen coordinates
			int xk = x() + 100*Graph.x2[n];         // Scale X coordinate
			int yk = y() + h()/2 - 100*Graph.y2[n]; // Scale Y coordinate

			// Draw circle with radius 5 pixels around each data point
			fl_begin_line();
			fl_circle(xk, yk, 5);
			fl_end_line();
		}

		// STEP 4: Draw fitted ellipse contour as cyan points
		fl_color(fl_rgb_color(0, 255, 255)); // Set color to cyan

		for (n = 0; n < Graph.n; n++)
		{
			// Convert data coordinates to screen coordinates
			int xk = x() + 100*Graph.x3[n];         // Scale X coordinate
			int yk = y() + h()/2 - 100*Graph.y3[n]; // Scale Y coordinate

			// Draw single pixel point
			fl_point(xk, yk);
		}

		// STEP 5: Draw coordinate system (axes, ticks, labels)
		fl_color(fl_rgb_color(0, 0, 0));    // Set color to black
		fl_font(FL_COURIER, 16);            // Set font for labels
		char str[256];                      // Buffer for label text

		// Draw horizontal X-axis through center of widget
		fl_line(x(), y()+h()/2, x()+w(), y()+h()/2);
		
		// Draw X-axis tick marks and labels (0.0 to 5.0 in steps of 0.5)
		for (int xpos=0; xpos<=10; xpos++)
		{
			// Draw vertical tick mark
			fl_line(x()+50*xpos, y()+h()/2-5, x()+50*xpos, y()+h()/2+5);
			// Create and draw numerical label
			sprintf(str, "%.1f", ((double) xpos)/2.0);
			fl_draw(str, x()+50*xpos, y()+h()/2+20);
		}
		
		// Draw vertical Y-axis from top to bottom of widget
		fl_line(x(), y(), x(), 10+h());
		
		// Draw Y-axis labels at extremes (+2.5 and -2.5)
		fl_draw("+2.5", x(), y()+h()/2-250+10);  // Top label
		fl_draw("-2.5", x(), y()+h()/2+250);     // Bottom label
    }

	// GRAPHBOX constructor - initializes the widget appearance
	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		box(FL_FLAT_BOX);                    // Set widget border style
		color(fl_rgb_color(127, 127, 127));  // Set background to gray
    }
};

/*
 * PARAMETRIC ELLIPSE FUNCTIONS
 * =============================
 * These functions generate points on an ellipse using parametric equations:
 * x(t) = a*cos(θ)*cos(t) - b*sin(θ)*sin(t) + x₀
 * y(t) = a*sin(θ)*cos(t) + b*cos(θ)*sin(t) + y₀
 * 
 * Parameters:
 * - t: parameter angle (0 to 2π)
 * - a: semi-major axis length
 * - b: semi-minor axis length  
 * - th: rotation angle of ellipse
 * - x0,y0: center coordinates
 */

// Calculate X coordinate of ellipse point at parameter t
static double fx(double t, double a, double b, double th, double x0)
{
	return a*cos(th)*cos(t) - b*sin(th)*sin(t) + x0;
}

// Calculate Y coordinate of ellipse point at parameter t
static double fy(double t, double a, double b, double th, double y0)
{
	return a*sin(th)*cos(t) + b*cos(th)*sin(t) + y0;
}

int main(void)
{
	// STEP 1: Create FLTK window and graphics widget
	Fl_Double_Window window(530, 530, "Graph");              // 530x530 pixel window
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20); // Graphics area with 10px border

	window.show();  // Display the window

	// STEP 2: Define ellipse parameters for theoretical model
	double a  = 2.0;                 // Semi-major axis length
	double b  = 1.5;                 // Semi-minor axis length
	double th = acos(-1.0)/8.0;      // Rotation angle (π/8 radians = 22.5°)
	double x0 = 2.0;                 // Center X coordinate
	double y0 = 0.0;                 // Center Y coordinate

	// STEP 3: Generate theoretical ellipse points using parametric equations
	int n = 100;                     // Number of points to generate
	int k;
	
	// Create array of parameter values from 0 to 2π
	double ts[n+1];
	for (k=0; k<=n; k++)
		ts[k] = 0.0 + 2.0*acos(-1.0)*k/n;  // acos(-1.0) = π
	
	// Generate theoretical X coordinates
	double xs[n+1];
	for (k=0; k<=n; k++)
    	xs[k] = fx(ts[k], a, b, th, x0);
    	
	// Generate theoretical Y coordinates
	double ys[n+1];
	for (k=0; k<=n; k++)
    	ys[k] = fy(ts[k], a, b, th, y0);
	
	// STEP 4: Simulate noisy measured data by adding random perturbations
	srand(clock());  // Seed random number generator with current time
	
	// Add noise to X coordinates
	double x[n+1];
	for (k=0; k<=n; k++)
	{
		double d = rand();           // Generate random integer
		d /= RAND_MAX;              // Normalize to [0,1]
    	x[k] = xs[k] + 0.5*(-0.5 + d);  // Add noise in range [-0.25, +0.25]
	}
	
	// Add noise to Y coordinates (re-seed for different sequence)
	srand(clock());
	double y[n+1];
	for (k=0; k<=n; k++)
	{
		double d = rand();           // Generate random integer
		d /= RAND_MAX;              // Normalize to [0,1]
    	y[k] = ys[k] + 0.5*(-0.5 + d);  // Add noise in range [-0.25, +0.25]
	}

	// STEP 5: Set up least squares problem for ellipse fitting
	// We want to fit the general conic: A*x² + B*y² + C*x*y + D*x + E*y + F = 0
	// Setting F = -1, we solve: A*x² + B*y² + C*x*y + D*x + E*y = 1
	// This creates the matrix equation: [x² y² xy x y] * [A B C D E]' = [1 1 ... 1]'

	// Create coefficient matrix A (each row corresponds to one data point)
	double A[n+1][5];
	for (k=0; k<=n; k++)
	{
		A[k][0] = x[k]*x[k];  // x² term coefficient
		A[k][1] = y[k]*y[k];  // y² term coefficient
		A[k][2] = x[k]*y[k];  // xy term coefficient
		A[k][3] = x[k];       // x term coefficient
		A[k][4] = y[k];       // y term coefficient
	}

	// Create right-hand side vector B (all ones since F = -1)
	double B[n+1];
	for (k=0; k<=n; k++)
		B[k] = 1.0;

	// STEP 6: Solve overdetermined system using LAPACK's least squares solver
	lapack_int info;
	// LAPACKE_dgels parameters:
	// - LAPACK_ROW_MAJOR: row-major matrix storage
	// - 'N': no transpose of matrix A
	// - n+1: number of rows in A (number of equations)
	// - 5: number of columns in A (number of unknowns)
	// - 1: number of columns in B (single right-hand side)
	// - (double *) A: coefficient matrix (flattened)
	// - 5: leading dimension of A
	// - (double *) B: right-hand side vector (solution will be stored here)
	// - 1: leading dimension of B
	if (info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', n+1, 5, 1, (double *) A, 5, (double *) B, 1))
		return EXIT_FAILURE;  // Exit if LAPACK solver failed
	
	// Extract fitted ellipse parameters from solution vector
	double p[5] = {B[0], B[1], B[2], B[3], B[4]};  // [A, B, C, D, E]
	
	// STEP 7: Store data for visualization
	// Copy theoretical and noisy data to graph structure
	for (k=0; k<=n; k++)
	{
		Graph.x1[k] = xs[k];  // Theoretical ellipse X coordinates
		Graph.y1[k] = ys[k];  // Theoretical ellipse Y coordinates
		Graph.x2[k] = x[k];   // Noisy data X coordinates
		Graph.y2[k] = y[k];   // Noisy data Y coordinates
	}

	// STEP 8: Generate fitted ellipse contour by finding zero-level curve
	// Evaluate fitted equation: p[0]*X² + p[1]*Y² + p[2]*X*Y + p[3]*X + p[4]*Y - 1 = 0
	double D = 0.025;  // Grid spacing for contour search
	double X, Y;
	k = 0;  // Counter for fitted ellipse points
	
	// Search over rectangular grid for points where fitted equation ≈ 0
	for (X=-0.5; X<=4.5; X+=D)      // X range from -0.5 to 4.5
	{
    	for (Y=-2.5; Y<=2.5; Y+=D)  // Y range from -2.5 to 2.5
		{
			// Evaluate fitted ellipse equation at grid point (X,Y)
        	double Z = p[0]*X*X + p[1]*Y*Y + p[2]*X*Y + p[3]*X + p[4]*Y - 1.0;
        	
        	// If equation value is close to zero, point lies on fitted ellipse
        	if (Z*Z < 0.0001)  // Tolerance for zero detection
			{
				Graph.x3[k] = X;  // Store X coordinate of contour point
				Graph.y3[k] = Y;  // Store Y coordinate of contour point
				k++;              // Increment contour point counter
			}
		}
	}
	Graph.n = k;  // Set total number of points for all datasets

	// STEP 9: Start FLTK event loop for interactive display
	Fl::run();           // Handle GUI events until window is closed
	return EXIT_SUCCESS; // Successful program termination
}

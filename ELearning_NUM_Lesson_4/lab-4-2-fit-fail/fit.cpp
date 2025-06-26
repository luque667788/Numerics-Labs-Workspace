/*
 * ELLIPSE FITTING DEMONSTRATION PROGRAM
 * ===================================
 *
 * GENERAL OVERVIEW:
 * This program demonstrates curve fitting using the least squares method to fit an ellipse
 * to noisy data points. The program performs the following main tasks:
 *
 * 1. GENERATES SYNTHETIC DATA: Creates a parametric ellipse with known parameters
 * 2. ADDS NOISE: Simulates real-world measurement errors by adding random noise
 * 3. PERFORMS CURVE FITTING: Uses LAPACK's DGELS to solve the least squares problem
 * 4. VISUALIZES RESULTS: Uses FLTK to display:
 *    - Original ellipse (red line)
 *    - Noisy data points (blue circles)
 *    - Fitted ellipse (cyan line with circles)
 *
 * MATHEMATICAL BACKGROUND:
 * An ellipse can be represented by the implicit equation:
 * Ax² + By² + Cxy + Dx + Ey + F = 0
 *
 * For fitting, we normalize by setting F = -1, giving:
 * Ax² + By² + Cxy + Dx + Ey = 1
 *
 * This creates a linear system that can be solved using least squares.
 *
 * DEPENDENCIES:
 * - FLTK: For GUI and graphics rendering
 * - LAPACKE: For numerical linear algebra operations
 * - Standard C math library: For trigonometric functions
 */

// filepath: /home/luque/Documents/UNI/sem3/numeriks/labs workspaces/ELearning_NUM_Lesson_4/lab-4-2-fit-fail/fit.cpp

// FLTK GUI library headers
#include <FL/Fl.H>				 // Main FLTK header
#include <FL/Fl_Double_Window.H> // Double-buffered window for smooth graphics
#include <FL/Fl_Box.H>			 // Basic widget container
#include <FL/fl_draw.H>			 // Drawing functions (lines, circles, text)

// Standard C libraries
#include <math.h> // Mathematical functions (cos, sin, acos)
#include <time.h> // Time functions for random seed

// LAPACK linear algebra library
#include <lapacke.h> // C interface to LAPACK for solving linear systems

// Maximum number of data points that can be stored in graph arrays
#define GRAPH_MAX 1000

/*
 * GRAPH STRUCTURE
 * ==============
 * This structure holds three sets of 2D coordinate data for visualization:
 * - Set 1 (x1[], y1[]): Original theoretical ellipse points (red line)
 * - Set 2 (x2[], y2[]): Noisy measured data points (blue circles)
 * - Set 3 (x3[], y3[]): Fitted ellipse points (cyan line with circles)
 */
struct GRAPH
{
	double x1[GRAPH_MAX + 1]; // X coordinates for original ellipse
	double y1[GRAPH_MAX + 1]; // Y coordinates for original ellipse
	int n1;					  // Number of points in original ellipse
	double x2[GRAPH_MAX + 1]; // X coordinates for noisy data points
	double y2[GRAPH_MAX + 1]; // Y coordinates for noisy data points
	int n2;					  // Number of noisy data points
	double x3[GRAPH_MAX + 1]; // X coordinates for fitted ellipse
	double y3[GRAPH_MAX + 1]; // Y coordinates for fitted ellipse
	int n3;					  // Number of points in fitted ellipse
};

// Global instance of the graph data structure
static struct GRAPH Graph;

/*
 * CUSTOM GRAPHICS WIDGET CLASS
 * ===========================
 * GRAPHBOX extends FLTK's Fl_Box to create a custom drawing area for visualizing
 * the ellipse fitting results. It handles all the graphics rendering including
 * coordinate system setup, axis drawing, and data visualization.
 */
class GRAPHBOX : public Fl_Box
{
private:
	void draw(void)
	{
		// STEP 1: Clear background and prepare for drawing
		Fl_Box::draw(); // Call parent's draw method to clear background

		// STEP 2: Draw original ellipse (theoretical curve) in RED
		fl_color(fl_rgb_color(255, 0, 0)); // Set color to red

		// Calculate screen coordinates for first point
		// Transform from data coordinates to screen coordinates:
		// - x() + 100*Graph.x1[0]: Scale data by 100 and offset by widget's x position
		// - y() + h()/2 - 100*Graph.y1[0]: Flip Y axis (screen Y increases downward)
		int xk0 = x() + 100 * Graph.x1[0];
		int yk0 = y() + h() / 2 - 100 * Graph.y1[0];

		int n;
		// Draw line segments connecting consecutive points of original ellipse
		for (n = 1; n < Graph.n1; n++)
		{
			// Calculate screen coordinates for current point
			int xk1 = x() + 100 * Graph.x1[n];
			int yk1 = y() + h() / 2 - 100 * Graph.y1[n];

			// Draw line from previous point to current point
			fl_line(xk0, yk0, xk1, yk1);

			// Update previous point coordinates for next iteration
			xk0 = xk1;
			yk0 = yk1;
		}

		// STEP 3: Draw noisy data points as BLUE circles
		fl_color(fl_rgb_color(0, 0, 255)); // Set color to blue

		// Draw each noisy data point as a small circle
		for (n = 0; n < Graph.n2; n++)
		{
			// Transform data coordinates to screen coordinates
			int xk = x() + 100 * Graph.x2[n];
			int yk = y() + h() / 2 - 100 * Graph.y2[n];

			// Draw circle: begin_line/end_line pair needed for circle drawing
			fl_begin_line();
			fl_circle(xk, yk, 5); // Draw circle with radius 5 pixels
			fl_end_line();
		}

		// STEP 4: Draw fitted ellipse in CYAN with point markers
		fl_color(fl_rgb_color(0, 255, 255)); // Set color to cyan

		// Calculate screen coordinates for first point of fitted ellipse
		xk0 = x() + 100 * Graph.x3[0];
		yk0 = y() + h() / 2 - 100 * Graph.y3[0];

		// Draw fitted ellipse as connected line segments with point markers
		for (n = 1; n < Graph.n3; n++)
		{
			// Calculate screen coordinates for current point
			int xk1 = x() + 100 * Graph.x3[n];
			int yk1 = y() + h() / 2 - 100 * Graph.y3[n];

			// Draw line segment
			fl_line(xk0, yk0, xk1, yk1);

			// Draw circle marker at current point
			fl_begin_line();
			fl_circle(xk1, yk1, 5);
			fl_end_line();

			// Update coordinates for next iteration
			xk0 = xk1;
			yk0 = yk1;
		}

		// STEP 5: Draw coordinate axes and labels in BLACK
		fl_color(fl_rgb_color(0, 0, 0)); // Set color to black
		fl_font(FL_COURIER, 16);		 // Set font for axis labels
		char str[256];					 // Buffer for formatted text

		// Draw horizontal X-axis through middle of widget
		fl_line(x(), y() + h() / 2, x() + w(), y() + h() / 2);

		// Draw X-axis tick marks and labels
		for (int xpos = 0; xpos <= 10; xpos++)
		{
			// Draw tick mark (small vertical line)
			fl_line(x() + 50 * xpos, y() + h() / 2 - 5, x() + 50 * xpos, y() + h() / 2 + 5);

			// Create and draw numeric label for this tick
			sprintf(str, "%.1f", ((double)xpos) / 2.0);		   // Convert position to data units
			fl_draw(str, x() + 50 * xpos, y() + h() / 2 + 20); // Draw label below axis
		}

		// Draw vertical Y-axis on left side
		fl_line(x(), y(), x(), 10 + h());

		// Draw Y-axis labels at extremes (simplified labeling)
		fl_draw("+2.5", x(), y() + h() / 2 - 250 + 10); // Upper Y limit label
		fl_draw("-2.5", x(), y() + h() / 2 + 250);		// Lower Y limit label
	}

	// Constructor: Initialize the graphics widget
public:
	GRAPHBOX(int x, int y, int w, int h, const char *l = 0) : Fl_Box(x, y, w, h, l)
	{
		box(FL_FLAT_BOX);					// Set box style to flat (no border)
		color(fl_rgb_color(127, 127, 127)); // Set background to gray
	}
};

/*
 * PARAMETRIC ELLIPSE FUNCTIONS
 * ===========================
 * These functions generate points on an ellipse using parametric equations.
 *
 * Standard ellipse equation: (x-x0)²/a² + (y-y0)²/b² = 1
 * Parametric form with rotation:
 * x(t) = a*cos(θ)*cos(t) - b*sin(θ)*sin(t) + x0
 * y(t) = a*sin(θ)*cos(t) + b*cos(θ)*sin(t) + y0
 *
 * Where:
 * - t: parameter (0 to 2π)
 * - a, b: semi-major and semi-minor axes
 * - θ (th): rotation angle
 * - (x0, y0): center coordinates
 */

// Calculate X coordinate of ellipse point at parameter t
static double fx(double t, double a, double b, double th, double x0)
{
	// X = a*cos(θ)*cos(t) - b*sin(θ)*sin(t) + x0
	return a * cos(th) * cos(t) - b * sin(th) * sin(t) + x0;
}

// Calculate Y coordinate of ellipse point at parameter t
static double fy(double t, double a, double b, double th, double y0)
{
	// Y = a*sin(θ)*cos(t) + b*cos(θ)*sin(t) + y0
	return a * sin(th) * sin(t) + b * cos(th) * cos(t) + y0;
}

/*
 * MAIN PROGRAM
 * ===========
 * This is the main function that orchestrates the entire ellipse fitting demonstration.
 *
 * PROGRAM FLOW:
 * 1. Create GUI window and graphics widget
 * 2. Define true ellipse parameters
 * 3. Generate clean theoretical ellipse points
 * 4. Add random noise to simulate measurement errors
 * 5. Set up and solve least squares fitting problem using LAPACK
 * 6. Generate points on the fitted ellipse
 * 7. Display all results graphically
 * 8. Run the GUI event loop
 */
int main(void)
{
	// STEP 1: Initialize GUI components
	Fl_Double_Window window(530, 530, "Graph");					 // Create 530x530 window titled "Graph"
	GRAPHBOX graphbox(10, 10, window.w() - 20, window.h() - 20); // Create graphics area with 10px margins

	window.show(); // Display the window

	// STEP 2: Define true ellipse parameters (known ground truth)
	double a = 2.0;				  // Semi-major axis length
	double b = 1.5;				  // Semi-minor axis length
	double th = acos(-1.0) / 8.0; // Rotation angle = π/8 radians (22.5 degrees)
	double x0 = 2.0;			  // Center X coordinate
	double y0 = 0.0;			  // Center Y coordinate

	// STEP 3: Generate theoretical ellipse points (perfect, noise-free)
	int n = 100; // Number of points to generate
	int k;		 // Loop counter variable

	// Create array of parameter values from 0 to 2π
	double ts[n + 1];
	for (k = 0; k <= n; k++)
		ts[k] = 0.0 + 2.0 * acos(-1.0) * k / n; // ts[k] = 2π*k/n (acos(-1.0) = π)

	// Generate X coordinates of theoretical ellipse
	double xs[n + 1];
	for (k = 0; k <= n; k++)
		xs[k] = fx(ts[k], a, b, th, x0); // Call parametric X function

	// Generate Y coordinates of theoretical ellipse
	double ys[n + 1];
	for (k = 0; k <= n; k++)
		ys[k] = fy(ts[k], a, b, th, y0); // Call parametric Y function

	// STEP 4: Simulate noisy measurement data by adding random errors
	srand(clock()); // Seed random number generator with current time

	// Add noise to X coordinates
	double x[n + 1];
	for (k = 0; k <= n; k++)
	{
		double d = rand();				 // Generate random number [0, RAND_MAX]
		d /= RAND_MAX;					 // Normalize to [0, 1]
		x[k] = xs[k] + 0.5 * (-0.5 + d); // Add noise: ±0.25 uniform random error
	}

	// Add noise to Y coordinates (re-seed for different random sequence)
	srand(clock());
	double y[n + 1];
	for (k = 0; k <= n; k++)
	{
		double d = rand();				 // Generate random number [0, RAND_MAX]
		d /= RAND_MAX;					 // Normalize to [0, 1]
		y[k] = ys[k] + 0.5 * (-0.5 + d); // Add noise: ±0.25 uniform random error
	}

	// STEP 5: Set up least squares problem for ellipse fitting
	//
	// MATHEMATICAL BACKGROUND:
	// General conic equation: Ax² + By² + Cxy + Dx + Ey + F = 0
	// For ellipse fitting, normalize by setting F = -1:
	// Ax² + By² + Cxy + Dx + Ey = 1
	//
	// This creates a linear system: M * p = b
	// Where M is the design matrix and p = [A, B, C, D, E]ᵀ are the coefficients
	//
	// Design matrix A: each row contains [xi², yi², xi*yi, xi, yi]
	double A[n + 1][5];
	for (k = 0; k <= n; k++)
	{
		A[k][0] = x[k] * x[k]; // x² term coefficient
		A[k][1] = y[k] * y[k]; // y² term coefficient
		A[k][2] = x[k] * y[k]; // xy term coefficient
		A[k][3] = x[k];		   // x term coefficient
		A[k][4] = y[k];		   // y term coefficient
	}

	// Right-hand side vector B: all ones (since we normalized F = -1)
	double B[n + 1];
	for (k = 0; k <= n; k++)
		B[k] = 1.0; // Each equation equals 1

	// STEP 6: Solve least squares problem using LAPACK's DGELS routine
	//
	// LAPACKE_dgels solves overdetermined linear systems using QR or LQ factorization
	// Parameters:
	// - LAPACK_ROW_MAJOR: Matrix storage format (row-major for C)
	// - 'N': Normal operation (not transposed)
	// - n: Number of rows in A (number of data points)
	// - 5: Number of columns in A (number of parameters)
	// - 1: Number of columns in B (single right-hand side)
	// - (double *) A: Design matrix (cast to pointer for LAPACK)
	// - 5: Leading dimension of A
	// - (double *) B: Right-hand side vector (solution overwrites this)
	// - 1: Leading dimension of B
	lapack_int info;
	if (info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', n, 5, 1, (double *)A, 5, (double *)B, 1))
		return EXIT_FAILURE; // Exit if LAPACK fails

	// Extract fitted parameters from solution vector
	double p[5] = {B[0], B[1], B[2], B[3], B[4]}; // p = [A, B, C, D, E]

	// STEP 7: Generate points on the fitted ellipse curve
	//
	// The fitted ellipse is defined by: p[0]*X² + p[1]*Y² + p[2]*X*Y + p[3]*X + p[4]*Y = 1
	// We find points by scanning a grid and selecting points close to the curve

	double Sx[n + 1]; // X coordinates of fitted ellipse points
	double Sy[n + 1]; // Y coordinates of fitted ellipse points

	double D = 0.05; // Grid spacing for curve extraction
	double X, Y;	 // Grid scan variables
	k = 0;			 // Counter for fitted curve points

	// Scan 2D grid to find points on fitted ellipse
	for (X = -0.5; X <= 4.5; X += D)
	{
		for (Y = -2.5; Y <= 2.5; Y += D)
		{
			// Evaluate fitted ellipse equation at grid point (X,Y)
			double Z = p[0] * X * X + p[1] * Y * Y + p[2] * X * Y + p[3] * X + p[4] * Y - 1.0;

			// If Z ≈ 0, point is on the ellipse (within tolerance)
			if (Z * Z < 0.0001) // Tolerance: |Z| < 0.01
			{
				Sx[k] = X; // Store point on fitted ellipse
				Sy[k] = Y;
				k++;
			}
		}
	}
	int Sn = k; // Number of points found on fitted ellipse

	// STEP 8: Populate global graph structure for visualization

	// Store original theoretical ellipse (red line)
	for (k = 0; k <= n; k++)
	{
		Graph.x1[k] = xs[k]; // Perfect ellipse X coordinates
		Graph.y1[k] = ys[k]; // Perfect ellipse Y coordinates
	}
	Graph.n1 = k; // Number of original ellipse points

	// Store noisy measurement data (blue circles)
	for (k = 0; k <= n; k++)
	{
		Graph.x2[k] = x[k]; // Noisy X measurements
		Graph.y2[k] = y[k]; // Noisy Y measurements
	}
	Graph.n2 = k; // Number of noisy data points

	// Store fitted ellipse points (cyan line with circles)
	for (k = 0; k < Sn; k++)
	{
		Graph.x3[k] = Sx[k]; // Fitted ellipse X coordinates
		Graph.y3[k] = Sy[k]; // Fitted ellipse Y coordinates
	}
	Graph.n3 = k; // Number of fitted ellipse points

	// STEP 9: Start GUI event loop
	Fl::run();			 // Begin FLTK event processing (program runs until window closed)
	return EXIT_SUCCESS; // Program completed successfully
}

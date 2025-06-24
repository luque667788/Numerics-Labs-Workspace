/*
 * IMPROVED ELLIPSE FITTING DEMONSTRATION PROGRAM WITH CONVEX HULL
 * =============================================================
 * 
 * GENERAL OVERVIEW:
 * This program demonstrates IMPROVED curve fitting using the least squares method to fit an ellipse
 * to noisy data points. This version addresses some limitations of the basic "fit-fail" version
 * by implementing a convex hull algorithm for better curve visualization.
 * 
 * KEY IMPROVEMENTS OVER "fit-fail" VERSION:
 * 1. CONVEX HULL EXTRACTION: Uses Jarvis march (gift wrapping) algorithm to find the convex hull
 *    of the fitted curve points, providing a cleaner, more organized curve representation
 * 2. BETTER VISUALIZATION: The convex hull creates a proper closed curve instead of scattered points
 * 3. ROBUSTNESS: Less sensitive to grid artifacts and missing curve segments
 * 4. GEOMETRIC INSIGHT: Shows the outer boundary of the fitted conic section
 * 
 * PROGRAM TASKS:
 * 1. GENERATES SYNTHETIC DATA: Creates a parametric ellipse with known parameters
 * 2. ADDS NOISE: Simulates real-world measurement errors by adding random noise
 * 3. PERFORMS CURVE FITTING: Uses LAPACK's DGELS to solve the least squares problem
 * 4. EXTRACTS CURVE POINTS: Grid-based search for points on the fitted implicit curve
 * 5. COMPUTES CONVEX HULL: Applies Jarvis march algorithm to organize curve points
 * 6. VISUALIZES RESULTS: Uses FLTK to display:
 *    - Original ellipse (red line)
 *    - Noisy data points (blue circles) 
 *    - Fitted curve's convex hull (cyan line with circles)
 * 
 * MATHEMATICAL BACKGROUND:
 * An ellipse can be represented by the implicit equation:
 * Ax² + By² + Cxy + Dx + Ey + F = 0
 * 
 * For fitting, we normalize by setting F = -1, giving:
 * Ax² + By² + Cxy + Dx + Ey = 1
 * 
 * The convex hull provides the outer boundary of all points satisfying this equation.
 * 
 * DEPENDENCIES:
 * - FLTK: For GUI and graphics rendering
 * - LAPACKE: For numerical linear algebra operations
 * - Standard C math library: For trigonometric functions
 */

// filepath: /home/luque/Documents/UNI/sem3/numeriks/labs workspaces/ELearning_NUM_Lesson_4/lab-4-2-fit-fimplicit/fit.cpp

// FLTK GUI library headers
#include <FL/Fl.H>              // Main FLTK header
#include <FL/Fl_Double_Window.H> // Double-buffered window for smooth graphics
#include <FL/Fl_Box.H>          // Basic widget container
#include <FL/fl_draw.H>         // Drawing functions (lines, circles, text)

// Standard C libraries
#include <math.h>               // Mathematical functions (cos, sin, acos)
#include <time.h>               // Time functions for random seed

// LAPACK linear algebra library
#include <lapacke.h>            // C interface to LAPACK for solving linear systems

// Maximum number of data points that can be stored in graph arrays
#define	GRAPH_MAX		1000

/*
 * GRAPH STRUCTURE
 * ==============
 * This structure holds three sets of 2D coordinate data for visualization:
 * - Set 1 (x1[], y1[]): Original theoretical ellipse points (red line)
 * - Set 2 (x2[], y2[]): Noisy measured data points (blue circles)
 * - Set 3 (x3[], y3[]): Convex hull of fitted curve points (cyan line with circles)
 * 
 * NOTE: In this improved version, Set 3 represents the CONVEX HULL of the fitted
 * curve points, unlike the "fit-fail" version which shows raw scattered points.
 */
struct GRAPH
{
	double x1[GRAPH_MAX+1];    // X coordinates for original ellipse
	double y1[GRAPH_MAX+1];    // Y coordinates for original ellipse
	int n1;                    // Number of points in original ellipse
	double x2[GRAPH_MAX+1];    // X coordinates for noisy data points
	double y2[GRAPH_MAX+1];    // Y coordinates for noisy data points
	int n2;                    // Number of noisy data points
	double x3[GRAPH_MAX+1];    // X coordinates for convex hull of fitted curve
	double y3[GRAPH_MAX+1];    // Y coordinates for convex hull of fitted curve
	int n3;                    // Number of points in convex hull
};

// Global instance of the graph data structure
static struct GRAPH Graph;

/*
 * CUSTOM GRAPHICS WIDGET CLASS
 * ===========================
 * GRAPHBOX extends FLTK's Fl_Box to create a custom drawing area for visualizing
 * the ellipse fitting results. It handles all the graphics rendering including
 * coordinate system setup, axis drawing, and data visualization.
 * 
 * IMPROVEMENT OVER "fit-fail": This version displays a clean convex hull curve
 * instead of scattered, potentially disconnected curve points.
 */
class GRAPHBOX : public Fl_Box
{
    private : void draw(void)
	{
		// STEP 1: Clear background and prepare for drawing
		Fl_Box::draw();      // Call parent's draw method to clear background

		// STEP 2: Draw original ellipse (theoretical curve) in RED
		fl_color(fl_rgb_color(255, 0, 0));    // Set color to red

		// Calculate screen coordinates for first point
		// Transform from data coordinates to screen coordinates:
		// - x() + 100*Graph.x1[0]: Scale data by 100 and offset by widget's x position
		// - y() + h()/2 - 100*Graph.y1[0]: Flip Y axis (screen Y increases downward)
		int xk0 = x() + 100*Graph.x1[0];
		int yk0 = y() + h()/2 - 100*Graph.y1[0];

		int n;
		// Draw line segments connecting consecutive points of original ellipse
		for (n = 1; n < Graph.n1; n++)
		{
			// Calculate screen coordinates for current point
			int xk1 = x() + 100*Graph.x1[n];
			int yk1 = y() + h()/2 - 100*Graph.y1[n];

			// Draw line from previous point to current point
			fl_line(xk0, yk0, xk1, yk1);

			// Update previous point coordinates for next iteration
			xk0 = xk1;
			yk0 = yk1;
		}

		// STEP 3: Draw noisy data points as BLUE circles
		fl_color(fl_rgb_color(0, 0, 255));    // Set color to blue

		// Draw each noisy data point as a small circle
		for (n = 0; n < Graph.n2; n++)
		{
			// Transform data coordinates to screen coordinates
			int xk = x() + 100*Graph.x2[n];
			int yk = y() + h()/2 - 100*Graph.y2[n];

			// Draw circle: begin_line/end_line pair needed for circle drawing
			fl_begin_line();
			fl_circle(xk, yk, 5);    // Draw circle with radius 5 pixels
			fl_end_line();
		}

		// STEP 4: Draw convex hull of fitted curve in CYAN with point markers
		// IMPROVEMENT: Instead of scattered points, this shows a clean closed curve
		fl_color(fl_rgb_color(0, 255, 255));  // Set color to cyan

		// Calculate screen coordinates for first point of convex hull
		xk0 = x() + 100*Graph.x3[0];
		yk0 = y() + h()/2 - 100*Graph.y3[0];

		// Draw convex hull as connected line segments with point markers
		for (n = 1; n < Graph.n3; n++)
		{
			// Calculate screen coordinates for current point
			int xk1 = x() + 100*Graph.x3[n];
			int yk1 = y() + h()/2 - 100*Graph.y3[n];

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
		fl_color(fl_rgb_color(0, 0, 0));      // Set color to black
		fl_font(FL_COURIER, 16);              // Set font for axis labels
		char str[256];                        // Buffer for formatted text

		// Draw horizontal X-axis through middle of widget
		fl_line(x(), y()+h()/2, x()+w(), y()+h()/2);
		
		// Draw X-axis tick marks and labels
		for (int xpos=0; xpos<=10; xpos++)
		{
			// Draw tick mark (small vertical line)
			fl_line(x()+50*xpos, y()+h()/2-5, x()+50*xpos, y()+h()/2+5);
			
			// Create and draw numeric label for this tick
			sprintf(str, "%.1f", ((double) xpos)/2.0);  // Convert position to data units
			fl_draw(str, x()+50*xpos, y()+h()/2+20);    // Draw label below axis
		}
		
		// Draw vertical Y-axis on left side
		fl_line(x(), y(), x(), 10+h());
		
		// Draw Y-axis labels at extremes (simplified labeling)
		fl_draw("+2.5", x(), y()+h()/2-250+10);   // Upper Y limit label
		fl_draw("-2.5", x(), y()+h()/2+250);      // Lower Y limit label
    }

	// Constructor: Initialize the graphics widget
	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		box(FL_FLAT_BOX);                     // Set box style to flat (no border)
		color(fl_rgb_color(127, 127, 127));   // Set background to gray
    }
};

/*
 * CONVEX HULL COMPUTATIONAL GEOMETRY FUNCTIONS
 * ===========================================
 * These functions implement the Jarvis March (Gift Wrapping) algorithm to compute
 * the convex hull of a set of 2D points. This is a KEY IMPROVEMENT over the "fit-fail"
 * version, which simply displayed scattered curve points without organization.
 * 
 * CONVEX HULL BENEFITS:
 * 1. Creates a clean, connected curve representation
 * 2. Eliminates interior points and grid artifacts
 * 3. Provides the outer boundary of the fitted conic section
 * 4. More robust to noise and missing curve segments
 * 
 * ALGORITHM: Jarvis March (Gift Wrapping)
 * - Time Complexity: O(nh) where n = points, h = hull vertices
 * - Space Complexity: O(1) additional space
 * - Advantage: Simple implementation, good for small hull sizes
 */

// Helper function: Calculate cross product to determine point orientation
// Returns: > 0 if counterclockwise, < 0 if clockwise, = 0 if collinear
static double rotation(double ax, double ay, double bx, double by, double cx, double cy)
{
	// Cross product: (B-A) × (C-B) = (by-ay)*(cx-bx) - (bx-ax)*(cy-by)
	return (by - ay) * (cx - bx) - (bx - ax) * (cy - by);
}

// Main convex hull function using Jarvis March algorithm
// Input: Sx[], Sy[] - arrays of input points, n - number of input points
// Output: Hx[], Hy[] - arrays of hull vertices
// Returns: number of vertices in convex hull
static int convex(double Sx[], double Sy[], double Hx[], double Hy[], int n)
{
	// STEP 1: Validate input - need at least 3 points for a meaningful hull
	if (n < 3)		// set >= three points ?
		return 0;
	
	// STEP 2: Find leftmost point (guaranteed to be on convex hull)
	int l = 0;      // Index of leftmost point
	int i;
	for (i=1; i<n; i++)
		if (Sx[i] < Sx[l])    // Compare X coordinates
			l = i;
	
	// STEP 3: Jarvis March algorithm - "wrap" around the point set
	int k = 0;      // Hull vertex counter
	int p = l;      // Current hull point (start with leftmost)
	
	while (true)
	{
		// STEP 3a: Find next hull vertex by testing all points
		int q = (p + 1) % n;    // Initial candidate for next hull point
		
		// Test all points to find the one that makes the most counterclockwise turn
		for (i=0; i<n; i++)
			// If point i is more counterclockwise than current candidate q
			if (rotation(Sx[p], Sy[p], Sx[i], Sy[i], Sx[q], Sy[q]) < 0)
				q = i;	// Update candidate - point i is more "left" than q
		
		// STEP 3b: Add found hull vertex to result
		Hx[k] = Sx[q];    // Store hull vertex coordinates
		Hy[k] = Sy[q];
		k++;              // Increment hull vertex count

		// STEP 3c: Safety check to prevent infinite loops
		if (k > n-1)
			break;

		// STEP 3d: Check if we've completed the hull (returned to start)
		if (q == l)       // Back to leftmost point?
			break;
		
		// STEP 3e: Move to next hull vertex for next iteration
		p = q;
	}

	// Return number of vertices in convex hull
	return k;
}

/*
 * PARAMETRIC ELLIPSE FUNCTIONS
 * ===========================
 * These functions generate points on an ellipse using parametric equations.
 * (Same as in the "fit-fail" version - no changes needed here)
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
	return a*cos(th)*cos(t) - b*sin(th)*sin(t) + x0;
}

// Calculate Y coordinate of ellipse point at parameter t  
static double fy(double t, double a, double b, double th, double y0)
{
	// Y = a*sin(θ)*cos(t) + b*cos(θ)*sin(t) + y0
	return a*sin(th)*sin(t) + b*cos(th)*cos(t) + y0;
}

/*
 * MAIN PROGRAM WITH CONVEX HULL ENHANCEMENT
 * =======================================
 * This is the main function that orchestrates the IMPROVED ellipse fitting demonstration.
 * 
 * KEY DIFFERENCE FROM "fit-fail" VERSION:
 * After extracting curve points from the fitted implicit equation, this version computes
 * the convex hull of those points to create a clean, organized curve visualization.
 * This addresses the main limitation of the "fit-fail" version which showed scattered,
 * potentially disconnected curve points.
 * 
 * PROGRAM FLOW:
 * 1. Create GUI window and graphics widget
 * 2. Define true ellipse parameters  
 * 3. Generate clean theoretical ellipse points
 * 4. Add random noise to simulate measurement errors
 * 5. Set up and solve least squares fitting problem using LAPACK
 * 6. Generate points on the fitted ellipse using grid search
 * 7. **NEW STEP**: Compute convex hull of fitted curve points
 * 8. Display all results graphically (including convex hull)
 * 9. Run the GUI event loop
 */
int main(void)
{
	// STEP 1: Initialize GUI components (same as "fit-fail" version)
	Fl_Double_Window window(530, 530, "Graph");                    // Create 530x530 window titled "Graph"
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);      // Create graphics area with 10px margins

	window.show();    // Display the window

	// STEP 2: Define true ellipse parameters (same as "fit-fail" version)
	double a  = 2.0;               // Semi-major axis length
	double b  = 1.5;               // Semi-minor axis length  
	double th = acos(-1.0)/8.0;    // Rotation angle = π/8 radians (22.5 degrees)
	double x0 = 2.0;               // Center X coordinate
	double y0 = 0.0;               // Center Y coordinate

	// STEP 3: Generate theoretical ellipse points (same as "fit-fail" version)
	int n = 100;                   // Number of points to generate
	int k;                         // Loop counter variable
	
	// Create array of parameter values from 0 to 2π
	double ts[n+1];
	for (k=0; k<=n; k++)
		ts[k] = 0.0 + 2.0*acos(-1.0)*k/n;    // ts[k] = 2π*k/n (acos(-1.0) = π)
	
	// Generate X coordinates of theoretical ellipse
	double xs[n+1];
	for (k=0; k<=n; k++)
    	xs[k] = fx(ts[k], a, b, th, x0);      // Call parametric X function
    	
	// Generate Y coordinates of theoretical ellipse
	double ys[n+1];
	for (k=0; k<=n; k++)
    	ys[k] = fy(ts[k], a, b, th, y0);      // Call parametric Y function
	
	// STEP 4: Simulate noisy measurement data (same as "fit-fail" version)
	srand(clock());               // Seed random number generator with current time
	
	// Add noise to X coordinates
	double x[n+1];
	for (k=0; k<=n; k++)
	{
		double d = rand();        // Generate random number [0, RAND_MAX]
		d /= RAND_MAX;           // Normalize to [0, 1]
    	x[k] = xs[k] + 0.5*(-0.5 + d);  // Add noise: ±0.25 uniform random error
	}
	
	// Add noise to Y coordinates (re-seed for different random sequence)
	srand(clock());
	double y[n+1];
	for (k=0; k<=n; k++)
	{
		double d = rand();        // Generate random number [0, RAND_MAX] 
		d /= RAND_MAX;           // Normalize to [0, 1]
    	y[k] = ys[k] + 0.5*(-0.5 + d);  // Add noise: ±0.25 uniform random error
	}

	// STEP 5: Set up least squares problem (same as "fit-fail" version)
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
	double A[n+1][5];
	for (k=0; k<=n; k++)
	{
		A[k][0] = x[k]*x[k];     // x² term coefficient
		A[k][1] = y[k]*y[k];     // y² term coefficient  
		A[k][2] = x[k]*y[k];     // xy term coefficient
		A[k][3] = x[k];          // x term coefficient
		A[k][4] = y[k];          // y term coefficient
	}

	// Right-hand side vector B: all ones (since we normalized F = -1)
	double B[n+1];
	for (k=0; k<=n; k++)
		B[k] = 1.0;              // Each equation equals 1

	// STEP 6: Solve least squares problem using LAPACK (same as "fit-fail" version)
	lapack_int info;
	if (info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', n, 5, 1, (double *) A, 5, (double *) B, 1))
		return EXIT_FAILURE;     // Exit if LAPACK fails
	
	// Extract fitted parameters from solution vector
	double p[5] = {B[0], B[1], B[2], B[3], B[4]};   // p = [A, B, C, D, E]

	// STEP 7: Generate points on fitted curve (same grid method as "fit-fail")
	//
	// The fitted ellipse is defined by: p[0]*X² + p[1]*Y² + p[2]*X*Y + p[3]*X + p[4]*Y = 1
	// We find points by scanning a grid and selecting points close to the curve
	
	double Sx[n+1];              // X coordinates of fitted curve points (before hull)
	double Sy[n+1];              // Y coordinates of fitted curve points (before hull)

	double D = 0.05;             // Grid spacing for curve extraction
	double X, Y;                 // Grid scan variables
	k = 0;                       // Counter for fitted curve points
	
	// Scan 2D grid to find points on fitted ellipse
	for (X=-0.5; X<=4.5; X+=D)
	{
    	for (Y=-2.5; Y<=2.5; Y+=D)
		{
        	// Evaluate fitted ellipse equation at grid point (X,Y)
        	double Z = p[0]*X*X + p[1]*Y*Y + p[2]*X*Y + p[3]*X + p[4]*Y - 1.0;
        	
        	// If Z ≈ 0, point is on the ellipse (within tolerance)
        	if (Z*Z < 0.0001)        // Tolerance: |Z| < 0.01
			{
				Sx[k] = X;           // Store point on fitted ellipse
				Sy[k] = Y;
				k++;
			}
		}
	}
	int Sn = k;                  // Number of points found on fitted ellipse

	// STEP 8: **NEW IMPROVEMENT** - Compute convex hull of fitted curve points
	//
	// This is the KEY DIFFERENCE from "fit-fail" version:
	// Instead of displaying scattered curve points, we compute their convex hull
	// to create a clean, organized curve representation
	
	double Hx[n+1];              // X coordinates of convex hull vertices
	double Hy[n+1];              // Y coordinates of convex hull vertices

	// Apply Jarvis March algorithm to find convex hull
	int Hn = convex(Sx, Sy, Hx, Hy, Sn);

	// Close the convex hull by connecting back to first vertex
	Hx[Hn] = Hx[0];             // Add first vertex as last to close the curve
	Hy[Hn] = Hy[0];

	// STEP 9: Populate global graph structure for visualization
	
	// Store original theoretical ellipse (red line) - same as "fit-fail"
	for (k=0; k<=n; k++)
	{
		Graph.x1[k] = xs[k];     // Perfect ellipse X coordinates
		Graph.y1[k] = ys[k];     // Perfect ellipse Y coordinates
	}
	Graph.n1 = k;               // Number of original ellipse points

	// Store noisy measurement data (blue circles) - same as "fit-fail"
	for (k=0; k<=n; k++)
	{
		Graph.x2[k] = x[k];      // Noisy X measurements
		Graph.y2[k] = y[k];      // Noisy Y measurements  
	}
	Graph.n2 = k;               // Number of noisy data points

	// Store convex hull vertices (cyan line with circles) - **IMPROVEMENT**
	// This replaces the scattered curve points from "fit-fail" version
	for (k=0; k<=Hn; k++)
	{
		Graph.x3[k] = Hx[k];     // Convex hull X coordinates
		Graph.y3[k] = Hy[k];     // Convex hull Y coordinates
	}
	Graph.n3 = k;               // Number of convex hull vertices	

	// STEP 10: Start GUI event loop (same as "fit-fail" version)
	Fl::run();                  // Begin FLTK event processing (program runs until window closed)
	return EXIT_SUCCESS;        // Program completed successfully
}

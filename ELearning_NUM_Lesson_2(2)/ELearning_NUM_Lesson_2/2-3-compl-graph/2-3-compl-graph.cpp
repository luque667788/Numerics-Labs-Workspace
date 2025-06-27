
/*
===============================================================================
                    MANDELBROT SET VISUALIZATION PROGRAM
===============================================================================

GENERAL MATHEMATICAL OVERVIEW:
------------------------------
This program visualizes the famous MANDELBROT SET, one of the most beautiful 
and complex fractals in mathematics. The Mandelbrot set is defined as:

M = {c ∈ ℂ : the sequence z₀=0, z_{n+1} = z_n² + c does not diverge}

Where:
- c is a complex number c = a + bi (a,b are real numbers)
- z_n is the nth term in the iterative sequence
- "does not diverge" means |z_n| remains bounded as n → ∞

MATHEMATICAL THEORY:
-------------------
1. For each complex number c, we iterate: z_{n+1} = z_n² + c starting with z₀ = 0
2. If |z_n| > 2 at any point, the sequence diverges to infinity
3. If after many iterations |z_n| ≤ 2, c is likely in the Mandelbrot set
4. The boundary of this set creates the famous fractal pattern

COMPUTATIONAL APPROACH:
----------------------
1. Map each pixel (x,y) to a complex number c = a + bi
2. For each c, iterate z_{n+1} = z_n² + c until:
   - |z_n| > 2 (divergence detected) OR
   - Maximum iterations reached (assume convergence)
3. Color pixel based on how quickly the sequence diverges
4. Black pixels = points in the Mandelbrot set
5. Colored pixels = points outside the set (color indicates divergence speed)

NUMERICAL METHODS DEMONSTRATED:
------------------------------
- Complex number arithmetic (multiplication, addition)
- Iterative algorithms and convergence testing
- Discrete approximation of continuous mathematical objects
- Visualization of mathematical functions through computational graphics
- Escape-time algorithms for fractal generation

===============================================================================
*/

// FLTK (Fast Light Toolkit) headers for creating the graphical user interface
// These provide the window system and drawing capabilities for visualization
#include <FL/Fl.H>          // Main FLTK header - core functionality
#include <FL/Fl_Window.H>   // Window creation and management
#include <FL/Fl_Box.H>      // Basic rectangular widget for custom drawing
#include <FL/fl_draw.H>     // Low-level drawing functions (pixels, colors)

// SIZE: Resolution of our computational grid (600x600 pixels)
// Mathematical significance: Each pixel represents a point in the complex plane
// Higher SIZE = better resolution but exponentially more computation
// SIZE×SIZE = 360,000 complex numbers to test!
#define SIZE	600

// GRAPH structure: Data container for our computed fractal
// Mathematical purpose: Stores the "escape time" for each point in complex plane
struct GRAPH
{
	// plane[x][y]: 2D array representing the complex plane
	// Each element stores the number of iterations before divergence
	// Mathematical mapping: plane[x][y] ↔ complex number c = a + bi
	// where: a = real part, b = imaginary part
	// Array indices (x,y) map to complex coordinates through linear transformation
	int plane[SIZE+1][SIZE+1];
};

// Global instance of our computational grid
// Static = private to this file, maintains state between function calls
// This will hold the computed iteration counts for the entire Mandelbrot set
static struct GRAPH Graph;

// GRAPHBOX: Custom widget class for displaying our computed Mandelbrot set
// Inherits from Fl_Box to get basic rectangular widget functionality
// Mathematical purpose: Converts our numerical iteration data into visual pixels
class GRAPHBOX : public Fl_Box
{
    // draw(): Private method called automatically by FLTK to render the widget
    // This is where we convert mathematical data into visual representation
    private : void draw(void)
	{
		int xk, yk;  // Loop counters for traversing our computational grid
		
		// Double nested loop: traverse every point in our SIZE×SIZE grid
		// Mathematical significance: Each iteration processes one complex number
		for (xk=0; xk<SIZE; xk++)           // Traverse real axis (horizontal)
			for (yk=0; yk<SIZE; yk++)       // Traverse imaginary axis (vertical)
			{
				// Extract the computed iteration count for this complex number
				// n = "escape time" = how many iterations before divergence
				// Mathematical meaning: 
				//   - Low n = point diverges quickly (outside Mandelbrot set)
				//   - High n = point converges slowly (near/in Mandelbrot set)
				int n = Graph.plane[xk][yk];

				// Convert iteration count to grayscale color
				// fl_rgb_color(n,n,n): RGB color where R=G=B=n (grayscale)
				// Mathematical visualization:
				//   - n=0 (black): Points that diverge immediately
				//   - n=NMAX (white): Points likely in the Mandelbrot set
				//   - Intermediate values: Shades of gray showing divergence speed
				fl_color(fl_rgb_color(n, n, n));

				// Draw one pixel at the corresponding screen location
				// Coordinate transformation: mathematical grid → screen pixels
				// x() + xk: Horizontal position (real axis)
				// y() + h() - 1 - yk: Vertical position (imaginary axis, flipped)
				// Mathematical note: We flip Y because screen coordinates go down,
				// but mathematical imaginary axis goes up
				fl_point(x() + xk, y() + h() - 1 - yk);
			}
    }

	// Constructor: Initialize the graphical widget
	// Parameters: x,y (position), w,h (width,height), l (optional label)
	// Mathematical purpose: Creates the canvas for our fractal visualization
	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		/* nothing to be done */
		// The base class constructor handles all the setup we need
    }
};

// NMAX: Maximum number of iterations to test for convergence/divergence
// Mathematical significance: Computational limit for infinite mathematical process
// Trade-off: Higher NMAX = more accurate but slower computation
// Why 250? Good balance between accuracy and speed for visualization
#define NMAX	250		// maximum iterations

/*
===============================================================================
                        MANDELBROT ITERATION CORE ALGORITHM
===============================================================================
This function implements the heart of Mandelbrot set computation:
Testing whether a complex number c = a + bi belongs to the set.

MATHEMATICAL PROCESS:
1. Start with z₀ = 0 + 0i
2. Iterate: z_{n+1} = z_n² + c
3. Check if |z_n| > 2 (divergence criterion)
4. Count iterations until divergence or NMAX reached

WHY |z| > 2 MEANS DIVERGENCE:
If |z_n| > 2 at any point, mathematical theory proves the sequence will 
diverge to infinity. This is the "escape radius" for the Mandelbrot set.
===============================================================================
*/
static int Mandelbrot_findDepth(double a, double b)
{
	int n;                    // Iteration counter
	double re, im;           // Real and imaginary parts of z_{n+1}
	double x = 0, y = 0;     // Current z_n = x + yi, starting with z₀ = 0
	double d = 0;            // |z_n|² = distance squared from origin
	
	// Main iteration loop: implement z_{n+1} = z_n² + c
	// Mathematical process: Test convergence for complex number c = a + bi
	for (n=0; n<NMAX; n++)		// Continue until max iterations or divergence
	{
		// Compute z_{n+1} = z_n² + c where z_n = x + yi, c = a + bi
		// Complex multiplication: (x + yi)² = x² - y² + 2xyi
		// Mathematical expansion: z_n² + c = (x² - y² + a) + (2xy + b)i
		re = x*x - y*y + a;		// Real part: x² - y² + a
		im = 2*x*y + b;         // Imaginary part: 2xy + b
		
		// Compute |z_{n+1}|² = re² + im² (magnitude squared)
		// Mathematical purpose: Test escape criterion without expensive sqrt()
		// We use |z|² > 4 instead of |z| > 2 to avoid square root computation
		d = re*re + im*im;		// distance squared from origin
		
		// Divergence test: If |z_n|² > 4, then |z_n| > 2
		// Mathematical theorem: If |z_n| > 2, sequence diverges to infinity
		// This point c = a + bi is NOT in the Mandelbrot set
		if (d > 4.0)			// if > 4 : diverging
			break;              // Exit loop early - divergence detected
			
		// Update for next iteration: z_n = z_{n+1}
		// Prepare for next iteration of z_{n+1} = z_n² + c
		x = re;					// Real part of new z_n
		y = im;                 // Imaginary part of new z_n
	}
	
	// Return iteration count (escape time)
	// Mathematical interpretation:
	//   - n = 0-3: Fast divergence (definitely outside Mandelbrot set)
	//   - n = NMAX: No divergence detected (likely in Mandelbrot set)
	//   - n = intermediate: On boundary (creates fractal detail)
	return n;	// iterations completed before divergence
}

/*
===============================================================================
                     MANDELBROT SET COMPUTATION FUNCTION
===============================================================================
This function computes the Mandelbrot set over a specified region of the 
complex plane and stores results in our global Graph structure.

MATHEMATICAL PROCESS:
1. Divide complex plane region [rmin,rmax] × [imin,imax] into SIZE×SIZE grid
2. For each grid point, compute corresponding complex number c = re + im*i
3. Test each c using Mandelbrot_findDepth() to get escape time
4. Store results for visualization

COORDINATE TRANSFORMATION:
- Mathematical domain: [rmin,rmax] × [imin,imax] in complex plane
- Computational domain: [0,SIZE-1] × [0,SIZE-1] pixel grid
- Linear mapping: pixel(x,y) ↔ complex(re,im)
===============================================================================
*/
static void Mandelbrot(double rmin, double rmax, double imin, double imax)
{
	double dr, di;           // Step sizes for discretizing the complex plane
	double re, im;           // Current complex number c = re + im*i being tested
	
	// Calculate step sizes for uniform sampling of the complex plane
	// Mathematical purpose: Divide continuous complex plane into discrete grid
	dr = (rmax-rmin)/SIZE;		// Real axis step size (horizontal spacing)
	di = (imax-imin)/SIZE;      // Imaginary axis step size (vertical spacing)
	
	// Initialize real part: start slightly before rmin to align with loop increment
	// Mathematical trick: re starts at (rmin - dr), then we add dr in first iteration
	re = rmin - dr;
	
	int xk, yk;  // Grid indices for our SIZE×SIZE computational array
	
	// Outer loop: traverse the real axis (horizontal direction)
	// Mathematical interpretation: Sample real parts from rmin to rmax
	for (xk=0; xk<SIZE; xk++)
	{
		re += dr;				// Increment real part: next sample on real axis
		                        // Mathematical: re = rmin + xk * dr
		
		// Initialize imaginary part for this column
		// Same trick as real part: start before imin, increment in inner loop
		im = imin - di;

		// Inner loop: traverse the imaginary axis (vertical direction)
		// Mathematical interpretation: Sample imaginary parts from imin to imax
		for (yk=0; yk<SIZE; yk++)
		{
			im += di;			// Increment imaginary part: next sample on imaginary axis
			                    // Mathematical: im = imin + yk * di
			
			// Now we have: c = re + im*i, a specific complex number to test
			// Call core algorithm to determine if c is in Mandelbrot set
			// Mandelbrot_findDepth returns iteration count before divergence
			
			// Store inverted result: NMAX - iterations
			// Mathematical visualization logic:
			//   - If iterations = 0 (fast divergence): store NMAX (white)
			//   - If iterations = NMAX (slow/no divergence): store 0 (black)
			//   - This creates the classic Mandelbrot visualization where:
			//     * Black = points in the set
			//     * White/gray = points outside the set
			Graph.plane[xk][yk] = NMAX - Mandelbrot_findDepth(re, im);
		}
	}
	
	/*
	MATHEMATICAL SUMMARY OF WHAT WE'VE COMPUTED:
	--------------------------------------------
	After this function completes, Graph.plane[x][y] contains the "inverse escape time"
	for the complex number corresponding to pixel (x,y). This creates the data needed
	to visualize the Mandelbrot set:
	
	- Black regions: Points likely in the Mandelbrot set (bounded sequences)
	- Colored regions: Points outside the set (divergent sequences)
	- Fractal boundary: The infinitely complex edge between order and chaos
	
	The resulting image reveals one of mathematics' most beautiful structures!
	*/
}

/*
===============================================================================
                              MAIN PROGRAM
===============================================================================
This is the entry point that orchestrates the entire Mandelbrot visualization:
1. Create graphical user interface
2. Compute the Mandelbrot set
3. Display the results
4. Handle user interaction

MATHEMATICAL PARAMETERS:
The region [-2,2] × [-2,2] in the complex plane contains the entire 
Mandelbrot set and provides good context around it.
===============================================================================
*/
int main(void)
{
	// Create the main application window
	// Parameters: width=620, height=620, title="Graph"
	// Mathematical purpose: Provides canvas for visualizing complex plane
	// Size 620×620: Slightly larger than SIZE=600 to allow for borders
	Fl_Window window(620, 620, "Graph");
	
	// Create our custom graphical widget for displaying the Mandelbrot set
	// Parameters: x=10, y=10 (position), width=600, height=600 (size)
	// Mathematical purpose: This widget will convert our computed data into pixels
	// Positioning: 10-pixel border around the 600×600 computational grid
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	// Make the window visible on screen
	// At this point, the window exists but shows nothing (Graph.plane is uninitialized)
	window.show();

	// MATHEMATICAL COMPUTATION PHASE:
	// Compute the Mandelbrot set over the region [-2,2] × [-2,2] in complex plane
	// This region was chosen because:
	//   - It contains the entire Mandelbrot set
	//   - Provides good context showing both the set and surrounding area
	//   - Symmetric about both real and imaginary axes
	//   - Standard viewing window used in most Mandelbrot visualizations
	
	// Mathematical interpretation of parameters:
	// rmin=-2.0, rmax=2.0: Real axis from -2 to +2
	// imin=-2.0, imax=2.0: Imaginary axis from -2i to +2i
	// This covers complex numbers c where -2 ≤ Re(c) ≤ 2 and -2 ≤ Im(c) ≤ 2
	//Mandelbrot(-2.0, 2.0, -2.0, 2.0);
	Mandelbrot(0.0 + 0.5, 0.0 - 0.0, 0.0 - 0.5, 0.0 + 0.5);
	
	// After Mandelbrot() completes:
	//   - Graph.plane[][] contains computed escape times for 360,000 complex numbers
	//   - GRAPHBOX will automatically redraw, converting data to visual pixels
	//   - The famous Mandelbrot fractal pattern appears on screen!

	// Start the FLTK event loop
	// This handles:
	//   - Window redraws (calls our GRAPHBOX::draw() method)
	//   - User interactions (mouse clicks, window resizing, etc.)
	//   - Keeps the program running until user closes window
	// Mathematical significance: Makes our computed fractal interactive
	Fl::run();
	
	// Program termination
	// Mathematical achievement: We've successfully computed and visualized
	// one of the most complex and beautiful objects in mathematics!
	return EXIT_SUCCESS;
}

/*
===============================================================================
                           FINAL MATHEMATICAL SUMMARY
===============================================================================

WHAT THIS PROGRAM ACHIEVES:
---------------------------
This program demonstrates several fundamental concepts in computational mathematics:

1. COMPLEX NUMBER THEORY: Implementation of complex arithmetic and iteration
2. CONVERGENCE ANALYSIS: Testing sequences for boundedness vs. divergence  
3. FRACTAL GEOMETRY: Visualization of infinitely complex mathematical structures
4. NUMERICAL METHODS: Discrete approximation of continuous mathematical objects
5. COMPUTATIONAL GRAPHICS: Translation of numerical data into visual form

THE MANDELBROT SET'S SIGNIFICANCE:
---------------------------------
- Discovered by Benoit Mandelbrot in 1980
- Demonstrates how simple mathematical rules can create infinite complexity
- Contains self-similar structures at all scales (fractal property)
- Boundary has fractional dimension (not quite 1D curve, not quite 2D area)
- Connects to deep areas of mathematics: complex analysis, dynamical systems, chaos theory

COMPUTATIONAL INSIGHTS:
----------------------
- Simple algorithm: z_{n+1} = z_n² + c
- Massive computation: 360,000 complex numbers × up to 250 iterations each
- Parallel-friendly: Each pixel computed independently
- Optimization challenges: How to compute faster while maintaining accuracy
- Visualization challenges: How to represent infinite mathematical complexity in finite pixels

This program is a beautiful example of mathematics, computation, and art converging!
===============================================================================
*/

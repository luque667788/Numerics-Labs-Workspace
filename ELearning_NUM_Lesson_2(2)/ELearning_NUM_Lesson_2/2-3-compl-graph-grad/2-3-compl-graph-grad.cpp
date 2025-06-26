
/*
===============================================================================
        ENHANCED MANDELBROT SET VISUALIZATION WITH GRADIENT COLORING
===============================================================================

MAJOR DIFFERENCE FROM PREVIOUS VERSION:
--------------------------------------
This program is IDENTICAL to the previous Mandelbrot visualization in mathematical
computation, but uses ADVANCED COLOR MAPPING instead of simple grayscale.

KEY ENHANCEMENT: GRADIENT COLOR SCHEME
-------------------------------------
Instead of simple grayscale (black to white), this version uses a sophisticated
color gradient that creates much more visually appealing and informative fractal
visualization. The mathematical computation remains exactly the same.

MATHEMATICAL BACKGROUND (IDENTICAL TO PREVIOUS):
------------------------------------------------
This program visualizes the famous MANDELBROT SET using the same mathematical
principles: M = {c ∈ ℂ : the sequence z₀=0, z_{n+1} = z_n² + c does not diverge}

NEW FEATURE: ADVANCED COLOR MAPPING
----------------------------------
The escape time (iteration count) is now mapped to a rainbow-like color gradient:
- 0-49 iterations: Deep blue → Light blue (fast divergence)
- 50-99 iterations: Blue → Cyan → Light green (medium-fast divergence)  
- 100-149 iterations: Green → Yellow (medium divergence)
- 150-199 iterations: Yellow → Orange → Red (slow divergence)
- 200+ iterations: Grayscale (very slow divergence, near Mandelbrot set)

VISUAL IMPROVEMENT BENEFITS:
---------------------------
1. Better distinction between different escape times
2. More aesthetically pleasing fractal visualization
3. Easier to identify different "layers" of the fractal boundary
4. Enhanced perception of the fractal's infinite detail
5. Standard coloring scheme used in most modern Mandelbrot visualizations

MATHEMATICAL SIGNIFICANCE OF COLORS:
-----------------------------------
- Cool colors (blue/cyan): Points that diverge quickly (far from set)
- Warm colors (yellow/red): Points that diverge slowly (near set boundary)
- Black/gray: Points likely in the Mandelbrot set (no divergence detected)
- Color transitions reveal the fractal's self-similar structure at different scales

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
// NOTE: This structure is IDENTICAL to the previous version
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
// NOTE: Data storage is IDENTICAL to previous version - only visualization changes!
static struct GRAPH Graph;

// GRAPHBOX: Custom widget class for displaying our computed Mandelbrot set
// Inherits from Fl_Box to get basic rectangular widget functionality
// MAJOR DIFFERENCE: Enhanced color mapping instead of simple grayscale!
class GRAPHBOX : public Fl_Box
{
    // draw(): Private method called automatically by FLTK to render the widget
    // THIS IS WHERE THE MAIN DIFFERENCE FROM PREVIOUS VERSION OCCURS!
    // Mathematical data → Enhanced visual representation with gradient colors
    private : void draw(void)
	{
		int xk, yk;  // Loop counters for traversing our computational grid
		
		// Double nested loop: traverse every point in our SIZE×SIZE grid
		// Mathematical significance: Each iteration processes one complex number
		// NOTE: Loop structure IDENTICAL to previous version
		for (xk=0; xk<SIZE; xk++)           // Traverse real axis (horizontal)
			for (yk=0; yk<SIZE; yk++)       // Traverse imaginary axis (vertical)
			{
				// Extract the computed iteration count for this complex number
				// n = "escape time" = how many iterations before divergence
				// Mathematical meaning: 
				//   - Low n = point diverges quickly (outside Mandelbrot set)
				//   - High n = point converges slowly (near/in Mandelbrot set)
				// NOTE: Data extraction IDENTICAL to previous version
				int n = Graph.plane[xk][yk];

				/*
				=================================================================
				        ADVANCED COLOR MAPPING ALGORITHM
				=================================================================
				This is the MAIN DIFFERENCE from the previous version!
				Instead of simple grayscale: fl_color(fl_rgb_color(n, n, n))
				We now use a sophisticated multi-stage color gradient.
				
				MATHEMATICAL COLOR THEORY:
				- Each range represents different "escape velocities"
				- Color transitions reveal fractal boundary structure
				- Gradient creates smooth visual flow between regions
				=================================================================
				*/
				
				// STAGE 1: Fast divergence (0-49 iterations)
				// Mathematical interpretation: Points far from Mandelbrot set
				// Color scheme: Deep blue → Light blue
				// Visual effect: Cool colors for "cold" (quickly diverging) regions
				if (n < 50)
					fl_color(fl_rgb_color(0, 0, n+200));  // RGB: (0, 0, 200-249)
					// Red=0, Green=0, Blue increases from 200 to 249
					// Creates deep blue to light blue gradient
				else
				{
					// STAGE 2: Medium-fast divergence (50-99 iterations) 
					// Mathematical interpretation: Points moderately close to set boundary
					// Color scheme: Blue → Cyan → Light green
					// Visual effect: Transition from cool to warm colors
					if (n < 100)
						fl_color(fl_rgb_color(0, n+150, n+150));  // RGB: (0, 200-249, 200-249)
						// Red=0, Green and Blue increase together from 200 to 249
						// Creates cyan (blue+green) gradient
					else
					{
						// STAGE 3: Medium divergence (100-149 iterations)
						// Mathematical interpretation: Points near set boundary
						// Color scheme: Green → Yellow
						// Visual effect: Warm colors indicating proximity to set
						if (n < 150)
							fl_color(fl_rgb_color(100, n+100, 0));  // RGB: (100, 200-249, 0)
							// Red=100 (constant), Green increases, Blue=0
							// Creates green to yellow-green gradient
						else
						{
							// STAGE 4: Slow divergence (150-199 iterations)
							// Mathematical interpretation: Points very close to set boundary
							// Color scheme: Yellow → Orange → Red
							// Visual effect: Hot colors for "hot" (slowly diverging) regions
							if (n < 200)
								fl_color(fl_rgb_color(n+50, 50, 0));  // RGB: (200-249, 50, 0)
								// Red increases from 200 to 249, Green=50 (constant), Blue=0
								// Creates orange to red gradient
							else
								// STAGE 5: Very slow/no divergence (200+ iterations)
								// Mathematical interpretation: Points likely IN Mandelbrot set
								// Color scheme: Grayscale (same as previous version)
								// Visual effect: Neutral colors for set interior
								fl_color(fl_rgb_color(n, n, n));  // RGB: (n, n, n)
								// Same as previous version for points in/near the set
						}
					}
				}

				// Draw one pixel at the corresponding screen location
				// Coordinate transformation: mathematical grid → screen pixels
				// NOTE: Pixel placement IDENTICAL to previous version
				// x() + xk: Horizontal position (real axis)
				// y() + h() - 1 - yk: Vertical position (imaginary axis, flipped)
				// Mathematical note: We flip Y because screen coordinates go down,
				// but mathematical imaginary axis goes up
				fl_point(x() + xk, y() + h() - 1 - yk);
			}
    }
	// Constructor: Initialize the graphical widget
	// Parameters: x,y (position), w,h (width,height), l (optional label)
	// Mathematical purpose: Creates the canvas for our ENHANCED fractal visualization
	// NOTE: Constructor IDENTICAL to previous version
	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		/* nothing to be done */
		// The base class constructor handles all the setup we need
		// The magic happens in the enhanced draw() method above!
    }
};

// NMAX: Maximum number of iterations to test for convergence/divergence
// Mathematical significance: Computational limit for infinite mathematical process
// NOTE: Value and purpose IDENTICAL to previous version
// Why 250? Good balance between accuracy and speed, plus works well with color stages
#define NMAX	250		// maximum iterations

/*
===============================================================================
                        MANDELBROT ITERATION CORE ALGORITHM
===============================================================================
*** IMPORTANT: This function is IDENTICAL to the previous version! ***

The mathematical computation of the Mandelbrot set is exactly the same.
Only the visualization (color mapping) has been enhanced.

MATHEMATICAL PROCESS (UNCHANGED):
1. Start with z₀ = 0 + 0i
2. Iterate: z_{n+1} = z_n² + c
3. Check if |z_n| > 2 (divergence criterion)
4. Count iterations until divergence or NMAX reached

This demonstrates an important principle: separating mathematical computation
from visualization allows us to enhance the visual output without changing
the underlying mathematical algorithms.
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
	// *** ALGORITHM IDENTICAL TO PREVIOUS VERSION ***
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
	//   - n = 0-49: Fast divergence → Blue colors in enhanced version
	//   - n = 50-99: Medium-fast divergence → Cyan colors
	//   - n = 100-149: Medium divergence → Green-yellow colors  
	//   - n = 150-199: Slow divergence → Orange-red colors
	//   - n = 200+: Very slow/no divergence → Grayscale colors
	// *** RETURN VALUE IDENTICAL - only color interpretation enhanced! ***
	return n;	// iterations completed before divergence
}

/*
===============================================================================
                     MANDELBROT SET COMPUTATION FUNCTION
===============================================================================
*** IMPORTANT: This function is IDENTICAL to the previous version! ***

The mathematical computation and data storage are exactly the same.
The enhancement is purely in the visualization stage (GRAPHBOX::draw()).

This demonstrates the principle of SEPARATION OF CONCERNS:
- Mathematical computation (this function): Unchanged
- Data storage (Graph.plane): Unchanged  
- Visualization (GRAPHBOX::draw): Enhanced with gradient colors

MATHEMATICAL PROCESS (UNCHANGED):
1. Divide complex plane region into SIZE×SIZE grid
2. For each grid point, compute corresponding complex number c = re + im*i
3. Test each c using Mandelbrot_findDepth() to get escape time
4. Store results for visualization
===============================================================================
*/
static void Mandelbrot(double rmin, double rmax, double imin, double imax)
{
	double dr, di;           // Step sizes for discretizing the complex plane
	double re, im;           // Current complex number c = re + im*i being tested
	
	// Calculate step sizes for uniform sampling of the complex plane
	// Mathematical purpose: Divide continuous complex plane into discrete grid
	// *** CALCULATION IDENTICAL TO PREVIOUS VERSION ***
	dr = (rmax-rmin)/SIZE;		// Real axis step size (horizontal spacing)
	di = (imax-imin)/SIZE;      // Imaginary axis step size (vertical spacing)
	
	// Initialize real part: start slightly before rmin to align with loop increment
	// Mathematical trick: re starts at (rmin - dr), then we add dr in first iteration
	re = rmin - dr;
	
	int xk, yk;  // Grid indices for our SIZE×SIZE computational array
	
	// Outer loop: traverse the real axis (horizontal direction)
	// Mathematical interpretation: Sample real parts from rmin to rmax
	// *** LOOP STRUCTURE IDENTICAL TO PREVIOUS VERSION ***
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
			//   - If iterations = 0 (fast divergence): store NMAX (255) → Blue in new version
			//   - If iterations = NMAX (slow/no divergence): store 0 → Black in new version
			//   - Intermediate values create the enhanced color gradient
			// *** DATA STORAGE IDENTICAL - enhancement is in color interpretation! ***
			Graph.plane[xk][yk] = NMAX - Mandelbrot_findDepth(re, im);
		}
	}
	
	/*
	MATHEMATICAL SUMMARY (UNCHANGED FROM PREVIOUS):
	----------------------------------------------
	After this function completes, Graph.plane[x][y] contains the "inverse escape time"
	for the complex number corresponding to pixel (x,y). 
	
	*** ENHANCEMENT: The SAME numerical data now produces beautiful gradient colors! ***
	- Blue regions: Points that diverge quickly (far from set)
	- Cyan/Green regions: Points with medium divergence (moderate distance from set)
	- Yellow/Red regions: Points that diverge slowly (near set boundary)  
	- Black/Gray regions: Points likely in the Mandelbrot set (bounded sequences)
	
	The mathematical beauty remains the same - we've just enhanced how we see it!
	*/
}

/*
===============================================================================
                              MAIN PROGRAM
===============================================================================
*** IMPORTANT: Main program is IDENTICAL to the previous version! ***

The same mathematical computation, the same data generation, the same program flow.
The only difference is that our GRAPHBOX now renders with enhanced gradient colors
instead of simple grayscale, creating a much more beautiful visualization of the
same mathematical object.

This demonstrates excellent software design: the mathematical engine is completely
separate from the visualization layer, allowing visual enhancements without
changing the core computational algorithms.
===============================================================================
*/
int main(void)
{
	// Create the main application window
	// Parameters: width=620, height=620, title="Graph"
	// Mathematical purpose: Provides canvas for visualizing complex plane
	// *** WINDOW SETUP IDENTICAL TO PREVIOUS VERSION ***
	Fl_Window window(620, 620, "Graph");
	
	// Create our custom graphical widget for displaying the Mandelbrot set
	// Parameters: x=10, y=10 (position), width=600, height=600 (size)
	// Mathematical purpose: This widget will convert our computed data into pixels
	// *** ENHANCEMENT: Same widget, but with beautiful gradient color rendering! ***
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	// Make the window visible on screen
	// At this point, the window exists but shows nothing (Graph.plane is uninitialized)
	// *** DISPLAY SETUP IDENTICAL TO PREVIOUS VERSION ***
	window.show();

	// MATHEMATICAL COMPUTATION PHASE:
	// Compute the Mandelbrot set over the region [-2,2] × [-2,2] in complex plane
	// *** COMPUTATION IDENTICAL TO PREVIOUS VERSION ***
	// Same mathematical region: contains entire Mandelbrot set with good context
	// Same 360,000 complex number calculations
	// Same convergence testing and data storage
	Mandelbrot(-2.0, 2.0, -2.0, 2.0);
	
	// After Mandelbrot() completes:
	//   - Graph.plane[][] contains the SAME computed escape times as before
	//   - GRAPHBOX will automatically redraw with ENHANCED gradient colors
	//   - The result: same mathematical accuracy, dramatically improved beauty!

	// Start the FLTK event loop
	// This handles window redraws, user interactions, program lifecycle
	// *** EVENT LOOP IDENTICAL TO PREVIOUS VERSION ***
	Fl::run();
	
	// Program termination
	// Mathematical achievement: We've computed the same fractal with enhanced visualization!
	// *** RETURN IDENTICAL TO PREVIOUS VERSION ***
	return EXIT_SUCCESS;
}

/*
===============================================================================
                    COMPREHENSIVE COMPARISON WITH PREVIOUS VERSION
===============================================================================

MATHEMATICAL ASPECTS - IDENTICAL:
---------------------------------
✓ Same Mandelbrot set definition: z_{n+1} = z_n² + c
✓ Same convergence testing: |z| > 2 escape criterion  
✓ Same iteration limit: NMAX = 250
✓ Same complex plane region: [-2,2] × [-2,2]
✓ Same resolution: 600×600 = 360,000 calculations
✓ Same data storage: Graph.plane[x][y] array
✓ Same computational algorithms throughout

VISUAL ENHANCEMENT - NEW FEATURES:
----------------------------------
✗ Simple grayscale: fl_color(fl_rgb_color(n, n, n))
✓ Multi-stage gradient: 5 different color ranges
✓ Cool colors (blue/cyan) for fast divergence
✓ Warm colors (yellow/red) for slow divergence  
✓ Smooth color transitions reveal fractal structure
✓ Industry-standard coloring scheme
✓ Enhanced aesthetic appeal and mathematical insight

PROGRAMMING INSIGHTS:
--------------------
1. SEPARATION OF CONCERNS: Mathematical computation separate from visualization
2. CODE REUSE: Same core algorithms with enhanced presentation layer
3. MAINTAINABILITY: Can modify colors without touching mathematical code
4. EXTENSIBILITY: Easy to add more color schemes or interactive features

EDUCATIONAL VALUE:
-----------------
This comparison demonstrates that the same mathematical object can be
presented in dramatically different ways, each revealing different aspects
of its structure. The Mandelbrot set's infinite complexity becomes more
visually accessible through thoughtful color design, while the underlying
mathematics remains pure and unchanged.

Both versions are mathematically correct - this version is simply more
beautiful and informative in its visual presentation!
===============================================================================
*/

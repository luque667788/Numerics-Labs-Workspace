/*
 * MANDELBROT SET VISUALIZATION - COMPLEX SEQUENCE STABILITY ANALYSIS
 * 
 * GENERAL OVERVIEW:
 * ================
 * This program visualizes the famous Mandelbrot set, which is a fractal defined in the complex plane.
 * The Mandelbrot set consists of all complex numbers c for which the iterative sequence:
 * 
 *   z_{n+1} = z_n² + c
 * 
 * starting with z_0 = 0, remains bounded (doesn't diverge to infinity).
 * 
 * MATHEMATICAL BACKGROUND:
 * =======================
 * - For each point c = a + bi in the complex plane, we iterate the formula z_{n+1} = z_n² + c
 * - If |z_n| > 2 for any n, the sequence will diverge to infinity
 * - Points where the sequence remains bounded belong to the Mandelbrot set (colored black)
 * - Points where it diverges are colored based on how quickly they diverge
 * 
 * VISUALIZATION APPROACH:
 * ======================
 * 1. Create a grid representing the complex plane (real axis = x, imaginary axis = y)
 * 2. For each point in the grid, test if it belongs to the Mandelbrot set
 * 3. Color each pixel based on the number of iterations before divergence
 * 4. Use FLTK library to display the resulting fractal image
 * 
 * The result is a beautiful fractal with intricate boundary patterns and self-similarity.
 */

#include <FL/Fl.H>          // Main FLTK header for GUI framework
#include <FL/Fl_Window.H>   // Window class for creating GUI windows
#include <FL/Fl_Box.H>      // Box widget class for custom drawing areas
#include <FL/fl_draw.H>     // Drawing functions for graphics operations
// complex case of sequence stability mandel brot
#define SIZE	600         // Size of the visualization grid (600x600 pixels)

/*
 * GRAPH STRUCTURE:
 * ===============
 * This structure holds the 2D array representing our visualization grid.
 * Each element plane[i][j] stores the iteration count for the corresponding
 * point in the complex plane, which determines the pixel color.
 */
struct GRAPH
{
	int plane[SIZE+1][SIZE+1];  // 2D array: plane[x][y] = iteration count at complex point (x,y)
};

static struct GRAPH Graph;      // Global instance of our graph data structure

/*
 * CUSTOM GRAPHICS WIDGET CLASS:
 * =============================
 * This class extends FLTK's Fl_Box to create a custom widget that can draw
 * our Mandelbrot set visualization. It overrides the draw() method to render
 * the fractal based on the data stored in our Graph structure.
 */
class GRAPHBOX : public Fl_Box
{
    private : void draw(void)  // Override the draw method for custom rendering
	{
		int xk, yk;                    // Loop variables for pixel coordinates
		
		// Iterate through every pixel in our SIZE x SIZE grid
		for (xk=0; xk<SIZE; xk++)      // Loop through x-coordinates (real axis)
			for (yk=0; yk<SIZE; yk++)  // Loop through y-coordinates (imaginary axis)
			{
				int n = Graph.plane[xk][yk];  // Get iteration count for this pixel
				
				// Create grayscale color based on iteration count
				// Higher iteration counts = lighter colors (closer to Mandelbrot set)
				// Lower iteration counts = darker colors (diverges quickly)
				fl_color(fl_rgb_color(n, n, n));  // Set color (n,n,n) creates grayscale
				
				// Draw a single pixel point
				// x() + xk: widget's x position + pixel x offset
				// y() + h() - 1 - yk: flip y-axis (screen coords vs. math coords)
				fl_point(x() + xk, y() + h() - 1 - yk);
			}
    }

	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		/* Constructor - inherits from Fl_Box, no additional initialization needed */
    }
};

#define NMAX	250		// Maximum iterations before considering a point as "bounded"
                        // Higher values = more detail but slower computation

/*
 * MANDELBROT ITERATION FUNCTION:
 * =============================
 * This function determines if a complex number c = a + bi belongs to the Mandelbrot set.
 * 
 * MATHEMATICAL PROCESS:
 * ====================
 * For complex number c = a + bi, we iterate: z_{n+1} = z_n² + c, starting with z_0 = 0
 * 
 * Complex multiplication: (x + yi)² = x² - y² + 2xyi
 * So if z_n = x + yi, then z_n² = (x² - y²) + (2xy)i
 * Therefore: z_{n+1} = (x² - y² + a) + (2xy + b)i
 * 
 * ESCAPE CRITERION:
 * ================
 * If |z_n| > 2, the sequence will diverge to infinity.
 * We use |z|² = x² + y² > 4 (since |z| > 2 means |z|² > 4)
 * This avoids expensive square root calculations.
 */
static int Mandelbrot_findDepth(double a, double b)  // a = real part, b = imaginary part of c
{
	int n;                              // Iteration counter
	double re, im;                      // Real and imaginary parts of z_n
	double x = 0, y = 0;               // Current z_n: x + yi (start with z_0 = 0)
	double d = 0;                      // Distance squared: |z_n|²
	
	for (n=0; n<NMAX; n++)             // Iterate up to NMAX times
	{
		// Calculate z_{n+1} = z_n² + c
		// If z_n = x + yi, then z_n² = (x² - y²) + (2xy)i
		re = x*x - y*y + a;            // Real part: x² - y² + a
		im = 2*x*y + b;                // Imaginary part: 2xy + b
		
		// Check escape criterion: |z_n|² > 4 means the sequence diverges
		d = re*re + im*im;             // Calculate |z_{n+1}|² = re² + im²
		if (d > 4.0)                   // If magnitude squared > 4, sequence diverges
			break;                     // Exit loop - this point is not in Mandelbrot set
		
		// Prepare for next iteration: z_n becomes z_{n+1}
		x = re;                        // Update real part for next iteration
		y = im;                        // Update imaginary part for next iteration
	}
	
	return n;	// Return number of iterations before divergence (or NMAX if bounded)
}

/*
 * MANDELBROT SET COMPUTATION FUNCTION:
 * ===================================
 * This function fills our Graph.plane array with iteration counts for each point
 * in a specified rectangular region of the complex plane.
 * 
 * PARAMETERS:
 * rmin, rmax: Range of real axis (horizontal) to examine
 * imin, imax: Range of imaginary axis (vertical) to examine
 * 
 * COORDINATE MAPPING:
 * ==================
 * We map from complex plane coordinates to pixel grid coordinates:
 * - Real axis (rmin to rmax) → x-coordinates (0 to SIZE-1)
 * - Imaginary axis (imin to imax) → y-coordinates (0 to SIZE-1)
 * 
 * COLOR ENCODING:
 * ==============
 * We store (NMAX - iterations) so that:
 * - Points in Mandelbrot set (iterations = NMAX) → color = 0 (black)
 * - Points that diverge quickly (low iterations) → higher color values (lighter)
 */
static void Mandelbrot(double rmin, double rmax, double imin, double imax)
{
	double dr, di, re, im;             // Step sizes and current complex coordinates
	
	// Calculate step sizes for mapping pixel coordinates to complex plane
	dr = (rmax-rmin)/SIZE;             // Real axis step size: total range / grid size
	di = (imax-imin)/SIZE;             // Imaginary axis step size: total range / grid size
	re = rmin - dr;                    // Initialize real part (subtract dr, will be added first)
	
	int xk, yk;                        // Pixel coordinate variables
	
	// Iterate through each pixel in our SIZE x SIZE grid
	for (xk=0; xk<SIZE; xk++)          // Loop through x-coordinates (real axis)
	{
		re += dr;                      // Move to next real coordinate: rmin + xk*dr
		im = imin - di;                // Reset imaginary part (subtract di, will be added first)

		for (yk=0; yk<SIZE; yk++)      // Loop through y-coordinates (imaginary axis)
		{
			im += di;                  // Move to next imaginary coordinate: imin + yk*di
			
			// Calculate iteration count for complex number (re + im*i)
			// Store inverted value: high iterations → low color (dark)
			//                      low iterations → high color (light)
			Graph.plane[xk][yk] = NMAX - Mandelbrot_findDepth(re, im);
		}
	}
}

/*
 * MAIN FUNCTION - PROGRAM ENTRY POINT:
 * ====================================
 * Sets up the GUI window, computes the Mandelbrot set for a specific region,
 * and starts the interactive display loop.
 */
int main(void)
{
	// Create the main application window
	Fl_Window window(620, 620, "Graph");         // 620x620 pixel window titled "Graph"
	
	// Create custom graphics widget inside the window
	// Position: (10,10), Size: window width-20 x window height-20 (10px border)
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	window.show();                               // Make the window visible

	/*
	 * MANDELBROT SET REGION SELECTION:
	 * ===============================
	 * Different regions of the complex plane reveal different aspects of the fractal.
	 * The comments below show various interesting regions to explore:
	 */

	// Full Mandelbrot set overview (classic view):
	// Mandelbrot(-2.0, 2.0, -2.0, 2.0);

	// Zoomed regions showing intricate fractal details:
	// Mandelbrot(-1.8, -1.6, -0.1, 0.1);        // Zoom into left side
	// Mandelbrot(-1.79, -1.73, -0.04, 0.04);    // Even deeper zoom

	// Other interesting regions:
	// Mandelbrot(-1.0, 0.0, 0.0, 1.0);          // Upper left quadrant
	
	// Currently active region: Close-up of intricate fractal boundary
	// Real axis: -0.75 to -0.73, Imaginary axis: 0.2 to 0.22
	// This shows fine detail and self-similar structures
	Mandelbrot(-0.75, -0.73, 0.2, 0.22);

	// Start the FLTK event loop - handles window events, redraws, user input
	Fl::run();                                   // Keep program running until window closes
	
	return EXIT_SUCCESS;                         // Program completed successfully
}

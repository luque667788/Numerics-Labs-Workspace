/*
 * MANDELBROT SET VISUALIZATION WITH GRADIENT COLORING - COMPLEX SEQUENCE STABILITY ANALYSIS
 * 
 * GENERAL OVERVIEW:
 * ================
 * This program visualizes the famous Mandelbrot set using a beautiful gradient color scheme.
 * Like the grayscale version, it computes the iterative sequence z_{n+1} = z_n² + c
 * starting with z_0 = 0 for each point c in the complex plane.
 * 
 * KEY DIFFERENCE FROM GRAYSCALE VERSION:
 * =====================================
 * Instead of using simple grayscale coloring (where all RGB components are equal),
 * this version uses a sophisticated gradient color mapping that creates a more
 * visually appealing and informative representation:
 * 
 * - Points that diverge quickly (low iterations) → Blue tones
 * - Medium divergence speed → Cyan/Green tones  
 * - Slower divergence → Yellow/Orange tones
 * - Very slow divergence → Red tones
 * - Points in Mandelbrot set (no divergence) → Grayscale
 * 
 * This gradient approach reveals more detail about the "escape velocity" of points
 * near the Mandelbrot set boundary, making the fractal structure more visible.
 * 
 * MATHEMATICAL BACKGROUND:
 * =======================
 * - For each point c = a + bi in the complex plane, we iterate z_{n+1} = z_n² + c
 * - If |z_n| > 2 for any n, the sequence will diverge to infinity
 * - Points where the sequence remains bounded belong to the Mandelbrot set
 * - The iteration count before divergence determines the color intensity and hue
 * 
 * ENHANCED VISUALIZATION APPROACH:
 * ===============================
 * 1. Create a grid representing the complex plane (real axis = x, imaginary axis = y)
 * 2. For each point, test convergence and count iterations before divergence
 * 3. Map iteration counts to different color ranges for enhanced visual appeal
 * 4. Use FLTK library to display the resulting colorful fractal image
 */

#include <FL/Fl.H>          // Main FLTK header for GUI framework
#include <FL/Fl_Window.H>   // Window class for creating GUI windows
#include <FL/Fl_Box.H>      // Box widget class for custom drawing areas
#include <FL/fl_draw.H>     // Drawing functions for graphics operations

#define SIZE	600         // Size of the visualization grid (600x600 pixels)

/*
 * GRAPH STRUCTURE:
 * ===============
 * This structure holds the 2D array representing our visualization grid.
 * Each element plane[i][j] stores the iteration count for the corresponding
 * point in the complex plane, which determines the pixel color and hue.
 */
struct GRAPH
{
	int plane[SIZE+1][SIZE+1];  // 2D array: plane[x][y] = iteration count at complex point (x,y)
};

static struct GRAPH Graph;      // Global instance of our graph data structure

/*
 * CUSTOM GRAPHICS WIDGET CLASS WITH GRADIENT COLORING:
 * ====================================================
 * This class extends FLTK's Fl_Box to create a custom widget that renders
 * the Mandelbrot set with a beautiful gradient color scheme instead of grayscale.
 * 
 * MAJOR DIFFERENCE FROM GRAYSCALE VERSION:
 * =======================================
 * The draw() method now implements a sophisticated color mapping system that
 * assigns different colors based on iteration counts, creating a rainbow-like
 * gradient that enhances the visual appeal and reveals more fractal detail.
 */
class GRAPHBOX : public Fl_Box
{
    private : void draw(void)  // Override the draw method for custom gradient rendering
	{
		int xk, yk;                    // Loop variables for pixel coordinates
		
		// Iterate through every pixel in our SIZE x SIZE grid
		for (xk=0; xk<SIZE; xk++)      // Loop through x-coordinates (real axis)
			for (yk=0; yk<SIZE; yk++)  // Loop through y-coordinates (imaginary axis)
			{
				int n = Graph.plane[xk][yk];  // Get iteration count for this pixel
				
				/*
				 * GRADIENT COLOR MAPPING SYSTEM:
				 * ==============================
				 * Instead of simple grayscale, we use a multi-stage color gradient
				 * that maps iteration counts to different color ranges:
				 * 
				 * Range 0-49:   Blue gradient (fast divergence)
				 * Range 50-99:  Cyan gradient (medium-fast divergence)  
				 * Range 100-149: Yellow gradient (medium divergence)
				 * Range 150-199: Red gradient (slow divergence)
				 * Range 200+:   Grayscale (very slow/no divergence)
				 * 
				 * This creates a beautiful "heat map" effect where colors indicate
				 * how quickly points escape from the Mandelbrot set.
				 */
				
				if (n < 50)                    // Fast divergence: Blue tones
					fl_color(fl_rgb_color(0, 0, n+200));  // RGB(0, 0, 200-249): Pure blue gradient
				else
				{
					if (n < 100)               // Medium-fast divergence: Cyan tones
						fl_color(fl_rgb_color(0, n+150, n+150)); // RGB(0, 150-199, 150-199): Cyan gradient
					else
					{
						if (n < 150)           // Medium divergence: Yellow tones
							fl_color(fl_rgb_color(100, n+100, 0)); // RGB(100, 100-199, 0): Yellow gradient
						else
						{
							if (n < 200)       // Slow divergence: Red tones
								fl_color(fl_rgb_color(n+50, 50, 0)); // RGB(150-199, 50, 0): Red gradient
							else               // Very slow/no divergence: Grayscale
								fl_color(fl_rgb_color(n, n, n)); // RGB(n, n, n): Traditional grayscale
						}
					}
				}

				// Draw the pixel with the calculated gradient color
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
                        // Same as grayscale version - mathematical logic unchanged

/*
 * MANDELBROT ITERATION FUNCTION:
 * =============================
 * This function is IDENTICAL to the grayscale version - the mathematical
 * computation of the Mandelbrot set remains the same. Only the coloring
 * differs between the two versions.
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
 * This function is IDENTICAL to the grayscale version - it fills our Graph.plane 
 * array with iteration counts for each point in a specified rectangular region 
 * of the complex plane. The gradient coloring happens only in the draw() method.
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
 * DATA ENCODING:
 * =============
 * We store (NMAX - iterations) so that:
 * - Points in Mandelbrot set (iterations = NMAX) → value = 0
 * - Points that diverge quickly (low iterations) → higher values
 * The gradient coloring system then maps these values to beautiful colors!
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
			// Store inverted value for proper gradient color mapping:
			// High iterations (Mandelbrot set) → low values → darker/special colors
			// Low iterations (fast divergence) → high values → brighter colors
			Graph.plane[xk][yk] = NMAX - Mandelbrot_findDepth(re, im);
		}
	}
}

/*
 * MAIN FUNCTION - PROGRAM ENTRY POINT:
 * ====================================
 * Sets up the GUI window, computes the Mandelbrot set for a specific region,
 * and starts the interactive display loop. Identical to grayscale version
 * except the visual output uses gradient coloring.
 * 
 * VISUAL ENHANCEMENT COMPARISON:
 * =============================
 * Grayscale version: Shows fractal structure but with limited visual contrast
 * Gradient version:  Reveals much more detail through color-coded "escape velocity"
 *                   - Makes boundary details more visible
 *                   - Creates artistic appeal while maintaining mathematical accuracy
 *                   - Different colors help distinguish convergence rates
 */
int main(void)
{
	// Create the main application window
	Fl_Window window(620, 620, "Graph");         // 620x620 pixel window titled "Graph"
	
	// Create custom graphics widget with gradient coloring capability
	// Position: (10,10), Size: window width-20 x window height-20 (10px border)
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	window.show();                               // Make the window visible

	/*
	 * MANDELBROT SET REGION SELECTION:
	 * ===============================
	 * Same regions as grayscale version - mathematical computation identical.
	 * The gradient coloring will make these regions much more visually striking:
	 */

	// Full Mandelbrot set overview (classic view with beautiful gradient):
	// Mandelbrot(-2.0, 2.0, -2.0, 2.0);

	// Zoomed regions showing intricate fractal details in full color:
	// Mandelbrot(-1.8, -1.6, -0.1, 0.1);        // Zoom with blue-to-red gradient
	// Mandelbrot(-1.79, -1.73, -0.04, 0.04);    // Deep zoom with color detail

	// Other interesting regions enhanced by gradient coloring:
	// Mandelbrot(-1.0, 0.0, 0.0, 1.0);          // Upper left with color mapping
	
	// Currently active region with gradient enhancement:
	// Real axis: -0.75 to -0.73, Imaginary axis: 0.2 to 0.22
	// The gradient colors will reveal subtle boundary details invisible in grayscale
	Mandelbrot(-0.75, -0.73, 0.2, 0.22);

	// Start the FLTK event loop - handles window events, redraws, user input
	Fl::run();                                   // Keep program running until window closes
	
	return EXIT_SUCCESS;                         // Program completed successfully
}

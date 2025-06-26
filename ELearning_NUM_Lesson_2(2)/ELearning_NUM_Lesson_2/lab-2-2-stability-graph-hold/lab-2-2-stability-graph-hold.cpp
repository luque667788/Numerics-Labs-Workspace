
/*
===============================================================================
   ANIMATED STEP-FUNCTION VISUALIZATION OF NUMERICAL STABILITY ("HOLD" VERSION)
===============================================================================

MAJOR DIFFERENCE FROM PREVIOUS ANIMATED VERSION:
-----------------------------------------------
This program is nearly IDENTICAL to the previous animated graph program in
mathematical computation and overall structure, but uses a DIFFERENT DRAWING
STYLE that creates a "step function" or "sample-and-hold" visualization.

KEY VISUAL ENHANCEMENT: STEP-FUNCTION PLOTTING
----------------------------------------------
Instead of diagonal line segments connecting points directly:
- Previous version: Smooth diagonal lines from point to point
- This version: Horizontal + vertical "step" pattern

MATHEMATICAL INTERPRETATION OF "HOLD" VISUALIZATION:
---------------------------------------------------
The step-function style is particularly appropriate for discrete mathematical
sequences because:
1. Emphasizes the DISCRETE nature of iterative sequences
2. Shows "holding" each value until the next iteration
3. Creates a more accurate representation of digital/computational processes
4. Mimics sample-and-hold circuits in signal processing
5. Better represents how computers actually store and process discrete data

VISUAL PATTERN ADVANTAGES:
-------------------------
- DIGITAL APPEARANCE: Looks like oscilloscope traces or digital signals
- CLEAR VALUE LEVELS: Each horizontal segment clearly shows the held value
- SHARP TRANSITIONS: Vertical segments emphasize iteration boundaries
- COMPUTATIONAL ACCURACY: Better represents discrete computational reality
- EASY COMPARISON: Horizontal segments make value comparison easier

EDUCATIONAL BENEFITS:
--------------------
- Reinforces discrete nature of numerical sequences
- Visualizes computer data processing concepts
- Shows connection between mathematics and digital signal processing
- Provides alternative visualization for different learning styles
- Demonstrates how presentation style affects mathematical understanding

===============================================================================
*/

// FLTK (Fast Light Toolkit) headers for advanced graphical user interface
#include <FL/Fl.H>          // Main FLTK header - core functionality
#include <FL/Fl_Window.H>   // Window creation and management
#include <FL/Fl_Box.H>      // Basic rectangular widget for custom drawing
#include <FL/fl_draw.H>     // Low-level drawing functions (lines, text, colors)

// Mathematical functions library (for potential future extensions)
#include <math.h>

// GRAPH_MAX: Maximum number of sequence terms to compute and display
// Mathematical significance: Limits our observation window to 37 iterations
// NOTE: Value and purpose IDENTICAL to previous animated version
#define	GRAPH_MAX	36+1

/*
===============================================================================
                     DATA STRUCTURE (IDENTICAL TO PREVIOUS)
===============================================================================
The mathematical data storage is exactly the same as the previous animated
version. Only the visualization method changes - demonstrating the principle
of separating data from presentation.
===============================================================================
*/
struct GRAPH
{
	// x[]: Array to store the complete sequence of iteration values
	// Mathematical purpose: Preserves entire iteration history for plotting
	// NOTE: Data structure IDENTICAL to previous version
	double x[GRAPH_MAX+1];
	
	// n: Current number of computed terms (sequence length)
	// Mathematical interpretation: We have computed x₀, x₁, ..., x_{n-1}
	// NOTE: Usage IDENTICAL to previous version
	int n;

	// window: Pointer to the main window for triggering redraws
	// Technical purpose: Allows timer callback to refresh the display
	// NOTE: Window management IDENTICAL to previous version
	Fl_Window *window;
};

// Global instance of our graphical data structure
// Static scope: Private to this file, maintains state across timer callbacks
// NOTE: Global data management IDENTICAL to previous version
static struct GRAPH Graph;

/*
===============================================================================
                    ENHANCED STEP-FUNCTION GRAPHICAL WIDGET
===============================================================================
This class creates the SAME sophisticated real-time plotting system as before,
but with a CRUCIAL DIFFERENCE in the drawing method that creates step-function
visualization instead of smooth diagonal lines.

VISUAL DIFFERENCE: STEP-FUNCTION vs SMOOTH LINES
The mathematical data is identical, but the presentation emphasizes the
discrete, "sample-and-hold" nature of computational sequences.
===============================================================================
*/
class GRAPHBOX : public Fl_Box
{
    // draw(): Enhanced method for step-function visualization
    // MAJOR DIFFERENCE: Uses horizontal + vertical line segments instead of diagonals
    // Mathematical visualization: Emphasizes discrete nature of sequences
    private : void draw(void)
	{		/*
		=================================================================
		           BACKGROUND AND AXES SETUP (IDENTICAL TO PREVIOUS)
		=================================================================
		Professional graph appearance with proper coordinate system.
		This section is exactly the same as the previous version.
		=================================================================
		*/
		
		// Redraw the widget's background using default styling
		// NOTE: Background rendering IDENTICAL to previous version
		Fl_Box::draw();

		// COORDINATE AXES RENDERING (IDENTICAL):
		// Set drawing color to black for mathematical axis lines
		fl_color(fl_rgb_color(0, 0, 0));
		
		// Draw horizontal axis (x-axis) through middle of widget
		// Mathematical significance: Represents y = 0 line (zero level)
		fl_line(x(), y()+h()/2, x()+w(), y()+h()/2);
		
		// Draw vertical axis (y-axis) at left edge of widget  
		// Mathematical significance: Represents iteration axis (n = 0)
		fl_line(x(), y(), x(), y()+h());

		/*
		=================================================================
		              STEP-FUNCTION SEQUENCE VISUALIZATION
		=================================================================
		*** THIS IS THE MAJOR DIFFERENCE FROM PREVIOUS VERSION! ***
		
		Instead of smooth diagonal lines connecting consecutive points,
		we now draw "step function" segments that:
		1. Hold each value horizontally (sample-and-hold behavior)
		2. Use vertical transitions between levels
		
		MATHEMATICAL INTERPRETATION:
		This better represents the discrete nature of computational
		sequences, where each value x_n is "held" until the next
		iteration computes x_{n+1}.
		=================================================================
		*/
		
		// Set drawing color to dark red for the mathematical sequence plot
		// NOTE: Color choice IDENTICAL to previous version
		fl_color(fl_rgb_color(127, 0, 0));

		// Initialize step-function drawing coordinates
		// Mathematical mapping: iteration n → horizontal position
		// NOTE: Starting coordinates IDENTICAL to previous version
		int xk0 = 10;                    // Starting x-position (10 pixels from left)
		int yk0 = 10 + h()/2;           // Starting y-position (center + offset)

		// MAIN STEP-FUNCTION PLOTTING LOOP:
		// *** MAJOR DIFFERENCE: Draw horizontal + vertical segments ***
		// Creates "sample-and-hold" visualization of discrete sequence
		int n;
		for (n=1; n<Graph.n; n++)         // Plot from x₁ to current computed term
		{
			// Calculate end coordinates for current step-function segment
			// Mathematical transformation: (iteration, value) → (screen_x, screen_y)
			// NOTE: Coordinate calculation IDENTICAL to previous version
			
			// Horizontal position: 10 pixels per iteration step
			int xk1 = x() + 10*(n + 1);
			
			// Vertical position: scaled and centered mathematical value
			// Scaling factor: 100 pixels per unit, centered at h()/2
			int yk1 = y() + h()/2 - 100*Graph.x[n];
			
			/*
			=============================================================
			        STEP-FUNCTION DRAWING (KEY DIFFERENCE!)
			=============================================================
			Instead of: fl_line(xk0, yk0, xk1, yk1);  // Diagonal line
			We now draw TWO lines to create step-function appearance:
			=============================================================
			*/
			
			// VERTICAL LINE: Shows instant transition between values
			// Mathematical meaning: Represents the computational "jump" 
			// from x_{n-1} to x_n at iteration boundary
			// Digital signal analogy: Sharp edge of digital transition
			fl_line(xk0, yk0, xk0, yk1);    // Vertical: (xk0,yk0) → (xk0,yk1)
			
			// HORIZONTAL LINE: Shows "holding" the new value
			// Mathematical meaning: Value x_n remains constant until next iteration
			// Sample-and-hold analogy: Maintains value until next sample
			// Computational reality: How computers actually store discrete values
			fl_line(xk0, yk1, xk1, yk1);    // Horizontal: (xk0,yk1) → (xk1,yk1)

			// Update starting coordinates for next step-function segment
			// Mathematical progression: Current endpoint becomes next startpoint
			// NOTE: Coordinate updating IDENTICAL to previous version
			xk0 = xk1;                    // Next segment starts where current ends
			yk0 = yk1;                    // Maintains visual continuity
		}

		/*
		=================================================================
		              REAL-TIME VALUE DISPLAY (IDENTICAL)
		=================================================================
		The numerical display system is exactly the same as before.
		Only the graph visualization method has changed.
		=================================================================
		*/
		
		// Set text color to black for readability
		// NOTE: Text rendering IDENTICAL to previous version
		fl_color(fl_rgb_color(0, 0, 0));
		fl_font(FL_COURIER, 16);
		
		// Display current iteration and value
		char str[256];
		n = Graph.n-1;
		sprintf(str, "x(%2d)=% 1.4lf", n, Graph.x[n]);
		fl_draw(str, x()+w()/2, y()+h()-20);
    }
	// Constructor: Initialize the step-function graphical widget
	// NOTE: Constructor IDENTICAL to previous version - only drawing method differs
	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		// Set widget appearance to flat style (no 3D borders)
		box(FL_FLAT_BOX);
		
		// Set background color to medium gray
		// NOTE: Styling IDENTICAL to previous version
		color(fl_rgb_color(127, 127, 127));
    }
};

/*
===============================================================================
                   ANIMATION TIMER CALLBACK (IDENTICAL TO PREVIOUS)
===============================================================================
*** IMPORTANT: This function is COMPLETELY IDENTICAL to the previous version! ***

The mathematical computation, data storage, and animation timing are exactly
the same. The only difference is in the visualization method (drawing style).

This demonstrates excellent software design: the mathematical engine is
completely separate from the presentation layer, allowing visual style
changes without affecting the core computational algorithms.
===============================================================================
*/
static void Timer_CB(void *)
{
	// ANIMATION CONTINUATION CHECK (IDENTICAL):
	if (Graph.n < GRAPH_MAX)
	{
		// CORE MATHEMATICAL ITERATION (IDENTICAL):
		// Same formula: x_{n+1} = x_n² - 1
		double x = Graph.x[Graph.n-1];
		x = x*x - 1.0;
		
		// SEQUENCE STORAGE (IDENTICAL):
		Graph.x[Graph.n] = x;
		Graph.n++;

		// VISUAL UPDATE TRIGGER (IDENTICAL):
		// Triggers redraw which now uses step-function visualization
		if (Graph.window)
			Graph.window->redraw();
		
		// ANIMATION CONTINUATION (IDENTICAL):
		Fl::repeat_timeout(0.5, Timer_CB);
	}
	
	/*
	MATHEMATICAL OBSERVATION (ENHANCED BY STEP-FUNCTION STYLE):
	The step-function visualization makes certain patterns more obvious:
	- Discrete "jumps" between values are emphasized
	- Horizontal segments make value levels easy to compare
	- Digital appearance reinforces computational nature
	- Better represents how computers actually process sequences
	*/
}

/*
===============================================================================
                         MAIN PROGRAM (IDENTICAL TO PREVIOUS)
===============================================================================
*** IMPORTANT: Main program is COMPLETELY IDENTICAL to the previous version! ***

The same mathematical setup, the same experimental parameters, the same
animation system. The only enhancement is the step-function visualization
style that better represents the discrete nature of computational sequences.

This demonstrates the power of modular design: we can completely change
the visual appearance without touching the mathematical core or program flow.
===============================================================================
*/
int main(void)
{
	// CREATE GRAPHICAL USER INTERFACE (IDENTICAL):
	// Same window size, same layout, same professional appearance
	Fl_Window window(400, 400, "Graph");
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);
	window.show();

	// INITIALIZE ANIMATION SYSTEM (IDENTICAL):
	Graph.window = &window;

	/*
	=================================================================
	             EXPERIMENTAL STARTING VALUES (IDENTICAL)
	=================================================================
	Same experimental options as previous version, but now with
	step-function visualization that may reveal different patterns
	or make existing patterns more obvious.
	=================================================================
	*/
	
	// PRIMARY EXPERIMENTAL SETUP (IDENTICAL):
	Graph.x[0] = -1.5;
	
	// ALTERNATIVE EXPERIMENTS (IDENTICAL):
	// Graph.x[0] = -1.618;               // Golden ratio fixed point
	// Graph.x[0] = -1.618-0.000033;      // Slightly perturbed
	// Graph.x[0] = -1.618-0.000034;      // Critical sensitivity
	
	// SEQUENCE INITIALIZATION (IDENTICAL):
	Graph.n = 1;

	// START MATHEMATICAL ANIMATION (IDENTICAL):
	Fl::add_timeout(0, Timer_CB);

	// ENTER EVENT PROCESSING LOOP (IDENTICAL):
	Fl::run();
	
	// PROGRAM COMPLETION:
	// Same mathematical achievement with enhanced step-function visualization
	return EXIT_SUCCESS;
}

/*
===============================================================================
                   COMPREHENSIVE COMPARISON WITH PREVIOUS VERSION
===============================================================================

MATHEMATICAL ASPECTS - COMPLETELY IDENTICAL:
--------------------------------------------
✓ Same Mandelbrot-related iteration: x_{n+1} = x_n² - 1
✓ Same starting value options and experimental setup
✓ Same convergence testing and sequence generation
✓ Same data storage and animation timing  
✓ Same numerical precision and computational methods
✓ Same educational mathematical objectives

VISUAL ENHANCEMENT - STEP-FUNCTION STYLE:
-----------------------------------------
✗ Previous: Smooth diagonal lines connecting points
✓ Current: Step-function with horizontal + vertical segments

STEP-FUNCTION ADVANTAGES:
------------------------
1. DISCRETE EMPHASIS: Better represents discrete computational sequences
2. DIGITAL APPEARANCE: Looks like oscilloscope or logic analyzer traces
3. VALUE CLARITY: Horizontal segments make value levels obvious
4. COMPUTATIONAL ACCURACY: Shows how computers actually store data
5. SIGNAL PROCESSING CONNECTION: Resembles sample-and-hold circuits

DRAWING METHOD COMPARISON:
-------------------------
Previous version:  fl_line(xk0, yk0, xk1, yk1);     // One diagonal line
Current version:   fl_line(xk0, yk0, xk0, yk1);     // Vertical line
                   fl_line(xk0, yk1, xk1, yk1);     // Horizontal line

EDUCATIONAL INSIGHTS ENHANCED:
-----------------------------
- Reinforces discrete nature of numerical computation
- Shows connection between mathematics and digital signal processing  
- Provides alternative visualization for different learning preferences
- Demonstrates how presentation style affects understanding
- Better represents computational reality vs mathematical abstraction

TECHNICAL ACHIEVEMENTS:
----------------------
- MODULAR DESIGN: Changed visualization without touching mathematical core
- SEPARATION OF CONCERNS: Data, computation, and presentation independent
- ALTERNATIVE REPRESENTATIONS: Same data, different insights
- PROFESSIONAL FLEXIBILITY: Easy to switch between visualization styles

WHEN TO USE EACH VERSION:
------------------------
- Smooth lines: When emphasizing mathematical continuity and trends
- Step functions: When emphasizing discrete computation and digital signals
- Both versions: For comparing how visualization affects understanding

This "hold" version demonstrates that the same mathematical content can be
presented in different ways to reveal different aspects of the underlying
mathematical and computational reality!
===============================================================================
*/


/*
===============================================================================
        ANIMATED GRAPHICAL VISUALIZATION OF NUMERICAL STABILITY
===============================================================================

MAJOR ENHANCEMENT FROM PREVIOUS VERSION:
----------------------------------------
This program is an ADVANCED EVOLUTION of the simple math.c program that showed
numerical stability through console output. Now we have:

1. REAL-TIME GRAPHICAL VISUALIZATION: Watch the sequence unfold dynamically
2. ANIMATED LINE GRAPH: Each new point appears every 0.5 seconds  
3. INTERACTIVE DISPLAY: Visual representation of mathematical convergence
4. ENHANCED USER EXPERIENCE: See patterns emerge visually over time

MATHEMATICAL FOUNDATION (SAME AS BEFORE):
----------------------------------------
We still compute the iterative sequence: x_{n+1} = x_n² - 1
But now instead of printing values, we:
- Store sequence values in an array
- Plot them as connected line segments
- Animate the plotting process in real-time
- Display current value and iteration number

EDUCATIONAL ADVANTAGES OF VISUALIZATION:
---------------------------------------
1. PATTERN RECOGNITION: Visual patterns easier to spot than numbers
2. CONVERGENCE BEHAVIOR: See oscillations, spirals, or convergence graphically
3. REAL-TIME DYNAMICS: Watch mathematical processes unfold over time
4. INTUITIVE UNDERSTANDING: Graphs provide immediate mathematical insight
5. COMPARISON CAPABILITY: Easy to compare different starting values

TECHNICAL ENHANCEMENTS:
----------------------
- Timer-based animation (0.5 second intervals)
- Dynamic graph plotting with connected line segments
- Coordinate system with axes and scaling
- Real-time value display
- Automatic window refresh for smooth animation

NUMERICAL STABILITY INSIGHTS (ENHANCED):
---------------------------------------
The graphical representation reveals stability patterns that might be
difficult to see in numerical output alone:
- Convergent sequences show flattening curves
- Oscillatory behavior appears as zigzag patterns  
- Divergent sequences show exponential growth curves
- Fixed points appear as horizontal asymptotes

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
// Why 36+1? Provides good balance between detail and screen space
// This allows us to observe long-term convergence behavior
#define	GRAPH_MAX	36+1

/*
===============================================================================
                           ENHANCED DATA STRUCTURE
===============================================================================
This structure represents a significant upgrade from the simple console version.
Instead of just computing and printing, we now store all values for visualization.
===============================================================================
*/
struct GRAPH
{
	// x[]: Array to store the complete sequence of iteration values
	// Mathematical purpose: Preserves entire iteration history for plotting
	// x[0] = x₀ (initial value), x[1] = x₁, x[2] = x₂, etc.
	// Index corresponds to iteration number: x[n] represents x_n
	// Data type: double for higher precision than simple float version
	double x[GRAPH_MAX+1];
	
	// n: Current number of computed terms (sequence length)
	// Mathematical interpretation: We have computed x₀, x₁, ..., x_{n-1}
	// Also serves as the index for the next term to be computed
	// Animation control: Determines when to stop the timer-based computation
	int n;

	// window: Pointer to the main window for triggering redraws
	// Technical purpose: Allows timer callback to refresh the display
	// Graphics synchronization: Ensures smooth animated visualization
	// Memory management: Does not own the window, just references it
	Fl_Window *window;
};

// Global instance of our graphical data structure
// Static scope: Private to this file, maintains state across timer callbacks
// Persistence: Stores the evolving sequence for animated visualization
// Shared access: Both timer callback and drawing functions use this data
static struct GRAPH Graph;

/*
===============================================================================
                        ADVANCED GRAPHICAL WIDGET CLASS
===============================================================================
This class creates a sophisticated real-time plotting system, far beyond
the simple console output of the basic version. It renders:
- Coordinate axes for mathematical reference
- Connected line segments showing sequence evolution  
- Real-time value display with current iteration
- Professional mathematical visualization aesthetics
===============================================================================
*/
class GRAPHBOX : public Fl_Box
{
    // draw(): Complex method for rendering the animated mathematical graph
    // Called automatically by FLTK whenever the window needs refreshing
    // Mathematical visualization: Converts numerical sequence into visual plot
    private : void draw(void)
	{		/*
		=================================================================
		                    BACKGROUND AND AXES SETUP
		=================================================================
		Professional graph appearance requires proper coordinate system
		and visual reference framework for mathematical interpretation.
		=================================================================
		*/
		
		// Redraw the widget's background using default styling
		// Technical: Calls parent class method to handle base rendering
		// Visual: Provides clean slate for our mathematical graph overlay
		Fl_Box::draw();

		// COORDINATE AXES RENDERING:
		// Set drawing color to black for mathematical axis lines
		// Professional standard: Black axes for clear mathematical reference
		fl_color(fl_rgb_color(0, 0, 0));
		
		// Draw horizontal axis (x-axis) through middle of widget
		// Mathematical significance: Represents y = 0 line (zero level)
		// Coordinates: from left edge to right edge at vertical center
		// Visual reference: Shows positive/negative regions of sequence values
		fl_line(x(), y()+h()/2, x()+w(), y()+h()/2);
		
		// Draw vertical axis (y-axis) at left edge of widget  
		// Mathematical significance: Represents iteration axis (n = 0)
		// Coordinates: from top to bottom at left edge
		// Visual reference: Shows sequence progression over iterations
		fl_line(x(), y(), x(), y()+h());

		/*
		=================================================================
		                    SEQUENCE VISUALIZATION
		=================================================================
		This section renders the iterative sequence as connected line
		segments, creating a dynamic graph that reveals mathematical
		patterns, convergence behavior, and stability characteristics.
		=================================================================
		*/
		
		// Set drawing color to dark red for the mathematical sequence plot
		// Color choice: Dark red stands out against gray background and black axes
		// Visual psychology: Red suggests dynamic, evolving mathematical process
		fl_color(fl_rgb_color(127, 0, 0));

		// Initialize line segment starting coordinates for graph plotting
		// Mathematical mapping: iteration n → horizontal position
		// x-coordinate: 10 pixels offset + 10 pixels per iteration step
		// y-coordinate: vertical center, adjusted by sequence value scaling
		int xk0 = 10;                    // Starting x-position (10 pixels from left)
		int yk0 = 10 + h()/2;           // Starting y-position (center + offset)

		// MAIN GRAPH PLOTTING LOOP:
		// Connect consecutive sequence values with line segments
		// Mathematical visualization: x_n connected to x_{n+1} for visual continuity
		// Animation effect: More line segments appear as n increases over time
		int n;
		for (n=1; n<Graph.n; n++)         // Plot from x₁ to current computed term
		{
			// Calculate end coordinates for current line segment
			// Mathematical transformation: (iteration, value) → (screen_x, screen_y)
			
			// Horizontal position: 10 pixels per iteration step
			// Mathematical mapping: iteration n → x-pixel position
			// Linear scaling: allows clear separation between iteration points
			int xk1 = x() + 10*(n + 1);
			
			// Vertical position: scaled and centered mathematical value
			// Mathematical transformation: sequence value → y-pixel position
			// Scaling factor: 100 pixels per unit (amplifies small values)
			// Sign convention: subtract because screen y increases downward
			// Centering: h()/2 places zero at middle of widget
			int yk1 = y() + h()/2 - 100*Graph.x[n];
			
			// Draw line segment connecting previous point to current point
			// Mathematical visualization: Shows sequence evolution step-by-step
			// Visual continuity: Connected segments reveal convergence patterns
			fl_line(xk0, yk0, xk1, yk1);

			// Update starting coordinates for next line segment
			// Mathematical progression: Current endpoint becomes next startpoint
			// Animation preparation: Ready for next iteration visualization
			xk0 = xk1;                    // Next segment starts where current ends
			yk0 = yk1;                    // Maintains visual continuity
		}

		/*
		=================================================================
		                    REAL-TIME VALUE DISPLAY
		=================================================================
		Professional mathematical software displays current numerical values
		alongside graphical representation for complete information.
		=================================================================
		*/
		
		// Set text color to black for readability
		// Typography: High contrast against gray background
		fl_color(fl_rgb_color(0, 0, 0));
		
		// Set font to monospace (Courier) for aligned numerical display
		// Font choice: Monospace ensures consistent spacing for numbers
		// Size 16: Large enough for easy reading during animation
		fl_font(FL_COURIER, 16);
		
		// Prepare formatted string showing current iteration and value
		// Mathematical information: Both index n and value x_n displayed
		char str[256];                    // Buffer for formatted text
		n = Graph.n-1;                    // Get index of most recent computed term
		
		// Format string: "x(nn)= v.vvvv" showing iteration and value
		// Mathematical notation: Matches standard sequence notation x_n
		// Precision: 4 decimal places for detailed numerical observation
		// Spacing: "% 1.4lf" provides space for positive values, alignment
		sprintf(str, "x(%2d)=% 1.4lf", n, Graph.x[n]);
		
		// Display formatted text at bottom center of widget
		// Position: Centered horizontally, 20 pixels from bottom
		// Real-time update: Shows changing values as animation progresses
		fl_draw(str, x()+w()/2, y()+h()-20);
    }
	// Constructor: Initialize the advanced graphical widget for mathematical plotting
	// Enhanced from simple console version with professional graph styling
	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		// Set widget appearance to flat style (no 3D borders)
		// Professional look: Clean, uncluttered mathematical visualization
		box(FL_FLAT_BOX);
		
		// Set background color to medium gray
		// Visual design: Neutral background that doesn't compete with graph data
		// Color psychology: Gray provides calm, scientific appearance
		color(fl_rgb_color(127, 127, 127));
    }
};

/*
===============================================================================
                           ANIMATION TIMER CALLBACK
===============================================================================
This is the heart of the real-time mathematical visualization system.
Unlike the simple console version that computed all values at once,
this timer-driven approach creates dramatic animated revelation of
mathematical convergence, making stability patterns visually obvious.

ANIMATION STRATEGY:
- Compute one new sequence term every 0.5 seconds
- Update the global Graph data structure  
- Trigger window refresh to show new line segment
- Schedule next timer callback for continued animation
- Stop when maximum iterations reached

MATHEMATICAL BENEFIT:
Students can watch convergence happen in real-time, building intuition
about dynamical systems and numerical stability that static numbers alone
cannot provide.
===============================================================================
*/
static void Timer_CB(void *)
{
	// ANIMATION CONTINUATION CHECK:
	// Only continue if we haven't reached maximum iteration limit
	// Mathematical bound: Prevents infinite computation and display overflow
	// User experience: Animation has clear endpoint for pedagogical sessions
	if (Graph.n < GRAPH_MAX)
	{
		// CORE MATHEMATICAL ITERATION:
		// Extract previous term from stored sequence history
		// Same mathematical formula as console version: x_{n+1} = x_n² - 1
		double x = Graph.x[Graph.n-1];    // Get x_{n-1} (previous computed term)
		
		// Apply the iterative formula: x_{n+1} = x_n² - 1
		// Mathematical significance: Identical to console version computation
		// This is the fundamental iteration that demonstrates stability
		x = x*x - 1.0;                    // Compute next sequence term
		
		// SEQUENCE STORAGE:
		// Store newly computed term in sequence array for visualization
		// Data persistence: Unlike console version, we keep all values
		// Graph memory: Enables connected line plotting and historical reference
		Graph.x[Graph.n] = x;             // Store x_n at index n
		
		// SEQUENCE COUNTER UPDATE:
		// Advance to next position for future computation
		// Mathematical progression: n represents current sequence length
		// Animation control: Determines when animation should stop
		Graph.n++;                        // Increment sequence length

		// VISUAL UPDATE TRIGGER:
		// Force window redraw to show newly added graph point
		// Real-time visualization: New line segment appears immediately
		// User feedback: Provides immediate visual response to computation
		if (Graph.window)                 // Safety check: ensure window exists
			Graph.window->redraw();       // Trigger complete window refresh
		
		// ANIMATION CONTINUATION:
		// Schedule next timer callback to continue the animated sequence
		// Timing: 0.5 second delay provides comfortable observation pace
		// Recursion: Timer reschedules itself until sequence completion
		// Mathematical pacing: Allows time to observe each convergence step
		Fl::repeat_timeout(0.5, Timer_CB);
	}
	
	/*
	ANIMATION COMPLETION:
	When Graph.n >= GRAPH_MAX, the timer stops scheduling itself.
	The final graph shows the complete sequence evolution.
	Students can observe the final convergence state and overall pattern.
	
	MATHEMATICAL OBSERVATION OPPORTUNITY:
	At this point, students should examine:
	- Did the sequence converge to a fixed point?
	- Was there oscillatory behavior?
	- How quickly did convergence occur?
	- What was the final numerical value?
	*/
}

/*
===============================================================================
                              MAIN PROGRAM
===============================================================================
This main function demonstrates the power of visualization in mathematical
education. The same mathematical iteration that appeared as simple numbers
in the console version now becomes a dramatic, real-time graphical experience
that reveals stability patterns with immediate visual impact.

EDUCATIONAL ENHANCEMENT:
Instead of scrolling numbers, students now see:
- Smooth curves revealing convergence
- Zigzag patterns showing oscillation  
- Flat lines indicating fixed points
- Dramatic curves showing divergence

EXPERIMENTAL CAPABILITY:
Multiple starting values are provided (commented out) to demonstrate
how small changes in initial conditions can lead to dramatically
different stability behaviors - a key insight in numerical analysis.
===============================================================================
*/
int main(void)
{
	// CREATE GRAPHICAL USER INTERFACE:
	// Professional mathematical visualization window
	// Size 400×400: Square format ideal for mathematical graphs
	// Title "Graph": Clear identification of program purpose
	Fl_Window window(400, 400, "Graph");
	
	// Create advanced plotting widget with professional styling
	// Positioning: 10-pixel border creates clean, framed appearance
	// Size: Fills most of window while maintaining visual margins
	// Mathematical canvas: Where our sequence visualization will appear
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	// Display window on screen
	// At this point: Empty graph with axes, ready for animated sequence
	window.show();

	// INITIALIZE ANIMATION SYSTEM:
	// Connect global graph data to window for refresh triggering
	// Technical requirement: Timer callback needs window reference
	Graph.window = &window;

	/*
	=================================================================
	                    EXPERIMENTAL STARTING VALUES
	=================================================================
	This section demonstrates the power of numerical experimentation.
	Different starting values reveal different stability behaviors:
	
	AVAILABLE EXPERIMENTS (uncomment one to try):
	- Graph.x[0] = -1.5        → Standard stable case
	- Graph.x[0] = -1.618      → Near golden ratio fixed point  
	- Graph.x[0] = -1.618-0.000033 → Slightly perturbed stable case
	- Graph.x[0] = -1.618-0.000034 → Critical sensitivity demonstration
	
	MATHEMATICAL SIGNIFICANCE:
	These values explore the boundary between stable and unstable
	behavior, demonstrating how numerical analysis must consider
	both mathematical theory and computational reality.
	=================================================================
	*/
	
	// PRIMARY EXPERIMENTAL SETUP:
	// Start with x₀ = -1.5 (same as console version for comparison)
	// Mathematical expectation: Bounded, potentially convergent sequence
	// Visual prediction: Should see curve that flattens toward fixed point
	Graph.x[0] = -1.5;
	
	// ALTERNATIVE EXPERIMENTS (uncomment to explore):
	// Golden ratio fixed point: Theoretically should remain constant
	// Graph.x[0] = -1.618;
	
	// Slightly perturbed from fixed point: Tests numerical sensitivity
	// Graph.x[0] = -1.618-0.000033;
	
	// Critical perturbation: May show dramatically different behavior
	// Graph.x[0] = -1.618-0.000034;
	
	// SEQUENCE INITIALIZATION:
	// Start with one computed term (x₀), ready to compute x₁, x₂, ...
	// Mathematical state: Sequence length = 1, next computation will be x₁
	// Animation ready: Timer will begin computing and displaying sequence
	Graph.n = 1;

	/*
	=================================================================
	                       ANIMATION LAUNCH
	=================================================================
	This is where the magic happens: instead of a simple loop printing
	numbers, we launch a sophisticated timer-based animation system
	that reveals mathematical behavior through real-time visualization.
	=================================================================
	*/
	
	// START MATHEMATICAL ANIMATION:
	// Schedule immediate first timer callback (0 delay)
	// Animation begins: First sequence term will be computed and displayed
	// Real-time revelation: Students watch stability patterns emerge
	Fl::add_timeout(0, Timer_CB);

	// ENTER EVENT PROCESSING LOOP:
	// Handle window events, timer callbacks, user interactions
	// Mathematical computation: Continues until sequence completion
	// User experience: Smooth, responsive graphical application
	Fl::run();
	
	// PROGRAM COMPLETION:
	// Mathematical achievement: Successfully demonstrated stability through
	// dramatic real-time visualization that reveals patterns impossible
	// to see in simple numerical output
	return EXIT_SUCCESS;
}

/*
===============================================================================
                         COMPREHENSIVE EDUCATIONAL IMPACT
===============================================================================

COMPARISON WITH CONSOLE VERSION:
-------------------------------
BEFORE (math.c):                    NOW (this program):
- Static numerical output           → Dynamic animated visualization
- Print 9 values and stop         → Real-time graphical revelation  
- Hard to see patterns            → Patterns immediately obvious
- Abstract mathematical concept    → Concrete visual experience
- Single run, single insight      → Interactive experimentation

MATHEMATICAL INSIGHTS ENHANCED:
------------------------------
1. CONVERGENCE VISUALIZATION: Watch curves flatten toward fixed points
2. OSCILLATION PATTERNS: See zigzag behavior in unstable regions
3. SENSITIVITY ANALYSIS: Tiny changes create dramatically different graphs
4. STABILITY BOUNDARIES: Visual exploration of mathematical phase space
5. REAL-TIME DYNAMICS: Experience mathematics as living, evolving process

PEDAGOGICAL ADVANTAGES:
----------------------
- IMMEDIATE FEEDBACK: See results as computation progresses
- PATTERN RECOGNITION: Visual patterns easier than numerical analysis
- EXPERIMENTAL MINDSET: Easy to try different starting values
- INTUITIVE UNDERSTANDING: Graph behavior matches mathematical theory
- ENGAGEMENT: Animation maintains student attention and interest

TECHNICAL ACHIEVEMENTS:
----------------------
- SEPARATION OF CONCERNS: Mathematical computation separate from visualization
- REAL-TIME SYSTEMS: Timer-based animation with smooth updates
- PROFESSIONAL UI: Industry-standard graph appearance and functionality
- EXTENSIBLE DESIGN: Easy to modify timing, colors, scaling, or experiments

MATHEMATICAL CONNECTIONS:
------------------------
This program serves as a bridge to advanced topics:
- Dynamical systems theory
- Chaos and bifurcation theory  
- Fixed point analysis
- Numerical methods stability
- Computer graphics and scientific visualization

The transformation from simple console output to sophisticated real-time
visualization demonstrates how technology can revolutionize mathematical
education, making abstract concepts tangible and immediately comprehensible.
===============================================================================
*/

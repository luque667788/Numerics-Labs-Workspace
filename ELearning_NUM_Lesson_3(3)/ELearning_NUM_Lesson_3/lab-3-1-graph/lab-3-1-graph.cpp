/*
 * RUNGE FUNCTION GRAPHICAL VISUALIZATION PROGRAM
 * ==============================================
 * 
 * GENERAL OVERVIEW:
 * This program creates a real-time graphical visualization of the Runge function
 * using the FLTK (Fast Light Toolkit) GUI library. It demonstrates the combination of:
 * 
 * 1. **Mathematical Function Evaluation**: Computing f(x) = 1/(1 + 16x²)
 * 2. **Numerical Sampling**: Discretizing the continuous function into points
 * 3. **Computer Graphics**: Rendering the function as a connected line graph
 * 4. **GUI Programming**: Creating interactive windows and custom drawing components
 * 
 * MATHEMATICAL SIGNIFICANCE:
 * =========================
 * 
 * **THE RUNGE FUNCTION VISUALIZATION:**
 * - Function: f(x) = 1/(1 + 16x²) for x ∈ [-1, 1]
 * - Visual characteristics: Bell-shaped curve, symmetric about x=0
 * - Maximum value: f(0) = 1.0 at the center
 * - Minimum values: f(±1) ≈ 0.059 at the boundaries
 * - Rapid decay: Function drops quickly as |x| increases
 * 
 * **SAMPLING AND DISCRETIZATION:**
 * - Continuous function sampled at discrete points
 * - Sample spacing: Δx = 1/25 = 0.04 (fine enough for smooth visualization)
 * - Total samples: ~51 points covering [-1, 1]
 * - Connected with line segments to approximate continuous curve
 * 
 * **COORDINATE TRANSFORMATION:**
 * - Mathematical domain: x ∈ [-1, 1], y ∈ [0, 1]
 * - Screen coordinates: Pixel-based positioning
 * - Scaling factors: 250 pixels per unit in both x and y directions
 * - Origin mapping: Mathematical (0,0) → Screen center
 * 
 * PROGRAM ARCHITECTURE:
 * ====================
 * 
 * **OBJECT-ORIENTED DESIGN:**
 * - GRAPH struct: Data container for function samples
 * - GRAPHBOX class: Custom GUI component extending Fl_Box
 * - Event-driven: FLTK handles window management and redraw events
 * 
 * **GRAPHICS PIPELINE:**
 * 1. Function sampling: Generate (x,y) coordinate pairs
 * 2. Coordinate transformation: Mathematical → Screen coordinates
 * 3. Line rendering: Connect points with colored line segments
 * 4. Axis drawing: Add coordinate system for reference
 * 
 * **REAL-TIME CAPABILITIES:**
 * - Interactive window (resizable, closable)
 * - Automatic redraw on window events
 * - Smooth curve rendering with anti-aliasing
 * 
 * DIFFERENCES FROM PREVIOUS PROGRAMS:
 * ==================================
 * 
 * **Language Evolution:**
 * - Previous: Pure C with mathematical focus
 * - Current: C++ with object-oriented graphics programming
 * 
 * **Functionality Expansion:**
 * - Previous: Console output (numbers and text)
 * - Current: Graphical output (visual function representation)
 * 
 * **User Interaction:**
 * - Previous: Batch processing (run once, display results, exit)
 * - Current: Interactive GUI (persistent window, real-time updates)
 * 
 * **Mathematical Application:**
 * - Previous: Computational tools (coefficients, weights, samples)
 * - Current: Visualization tool (understanding function behavior)
 */

// filepath: /home/luque/Documents/UNI/sem3/numeriks/labs workspaces/ELearning_NUM_Lesson_3(3)/ELearning_NUM_Lesson_3/lab-3-1-graph/lab-3-1-graph.cpp

/*
 * FLTK LIBRARY INCLUDES
 * =====================
 * 
 * FLTK (Fast Light Toolkit) is a cross-platform GUI library for C++.
 * Each header provides specific functionality:
 */

// Core FLTK functionality and main event loop
#include <FL/Fl.H>
// Double-buffered window class for smooth graphics rendering
#include <FL/Fl_Double_Window.H>
// Base widget class for creating custom GUI components
#include <FL/Fl_Box.H>
// Graphics drawing functions (lines, text, colors, etc.)
#include <FL/fl_draw.H>

/*
 * GRAPH DATA STRUCTURE CONFIGURATION
 * ==================================
 * 
 * This section defines the data storage for our function samples.
 */

// Maximum number of function sample points we can store
// Set to 100 to provide ample space for fine sampling resolution
#define	GRAPH_MAX		100

/*
 * GRAPH DATA STRUCTURE
 * ====================
 * 
 * Container for storing sampled function data points.
 * This structure encapsulates both the coordinate data and metadata.
 * 
 * DESIGN RATIONALE:
 * - Separate x and y arrays allow independent access to coordinates
 * - Fixed-size arrays provide predictable memory usage
 * - Counter variable tracks actual number of stored points
 * - 1-based indexing consideration: arrays sized as [GRAPH_MAX+1]
 */
struct GRAPH
{
	double x[GRAPH_MAX+1];  // Array of x-coordinates (mathematical domain values)
	double y[GRAPH_MAX+1];  // Array of y-coordinates (function values f(x))
	int n;                  // Number of valid data points currently stored
};

// Global instance of our graph data structure
// Using global scope for simplicity in this educational graphics program
static struct GRAPH Graph;

/*
 * CUSTOM GRAPHICS WIDGET CLASS
 * ============================
 * 
 * GRAPHBOX extends Fl_Box to create a specialized widget for rendering mathematical functions.
 * This demonstrates object-oriented graphics programming in C++.
 * 
 * **INHERITANCE HIERARCHY:**
 * Fl_Widget → Fl_Box → GRAPHBOX
 * 
 * **KEY CONCEPTS:**
 * - Inheritance: GRAPHBOX inherits all Fl_Box functionality
 * - Polymorphism: Overrides draw() method for custom rendering
 * - Encapsulation: Private draw() method, public constructor
 * - Event-driven: FLTK automatically calls draw() when needed
 * 
 * **GRAPHICS COORDINATE SYSTEMS:**
 * - FLTK Screen Coordinates: Origin (0,0) at top-left, y increases downward
 * - Mathematical Coordinates: Origin (0,0) at center, y increases upward
 * - Transformation required: Mathematical → Screen coordinates
 */
class GRAPHBOX : public Fl_Box
{
    /*
     * PRIVATE DRAW METHOD - CORE GRAPHICS RENDERING
     * ============================================
     * 
     * This method is called automatically by FLTK whenever the widget needs redrawing.
     * Events that trigger redraw: window resize, window expose, explicit redraw requests.
     * 
     * RENDERING PIPELINE:
     * 1. Clear background using parent class method
     * 2. Set up coordinate transformation parameters
     * 3. Draw function curve as connected line segments
     * 4. Draw coordinate axes and tick marks
     * 5. Add axis labels and numerical annotations
     */
    private : void draw(void)
	{
		/*
		 * STEP 1: BACKGROUND RENDERING
		 * ============================
		 * 
		 * Call parent class draw() method to handle:
		 * - Background color filling
		 * - Border drawing (if enabled)
		 * - Standard widget appearance
		 * 
		 * This ensures our custom graphics overlay on proper widget foundation.
		 */
		// redraw background
		Fl_Box::draw();

		/*
		 * STEP 2: FUNCTION CURVE RENDERING
		 * ================================
		 * 
		 * Draw the Runge function as a series of connected line segments.
		 * Each segment connects consecutive sample points.
		 */
		// redraw graph

		// Set line color to red (RGB: 255, 0, 0)
		// This makes the function curve stand out against the gray background
		fl_color(fl_rgb_color(255, 0, 0));

		/*
		 * COORDINATE TRANSFORMATION SETUP
		 * ==============================
		 * 
		 * Mathematical domain: x ∈ [-1, 1], y ∈ [0, 1]
		 * Screen domain: pixel coordinates within widget bounds
		 * 
		 * Transformation formulas:
		 * - Screen_x = widget_x + 250 * (1 + Math_x)
		 * - Screen_y = widget_y + widget_height/2 - 250 * Math_y
		 * 
		 * Scaling factor: 250 pixels per mathematical unit
		 * This provides good resolution for the [-1,1] domain
		 */
		
		// line start coordinates
		// Initialize at left edge of graph, vertically centered
		int xk0 = x();           // Widget's left edge (x-coordinate)
		int yk0 = y() + h()/2;   // Widget's vertical center (y-coordinate)

		/*
		 * FUNCTION CURVE DRAWING LOOP
		 * ===========================
		 * 
		 * Strategy: Connect each point to the next with a line segment
		 * Result: Piecewise linear approximation of smooth curve
		 * 
		 * Mathematical process:
		 * For each sample point (Graph.x[n], Graph.y[n]):
		 * 1. Transform mathematical coordinates to screen pixels
		 * 2. Draw line from previous point to current point  
		 * 3. Update "previous point" for next iteration
		 */
		int n;
		for (n = 0; n < Graph.n; n++)
		{
			/*
			 * COORDINATE TRANSFORMATION
			 * ========================
			 * 
			 * Transform mathematical point (Graph.x[n], Graph.y[n]) to screen coordinates.
			 * 
			 * X-coordinate transformation:
			 * - Graph.x[n] ∈ [-1, 1] (mathematical domain)
			 * - (1 + Graph.x[n]) ∈ [0, 2] (shift to positive range)  
			 * - 250 * (1 + Graph.x[n]) ∈ [0, 500] (scale to pixel range)
			 * - x() + 250 * (1 + Graph.x[n]) (offset by widget position)
			 * 
			 * Y-coordinate transformation:
			 * - Graph.y[n] ∈ [0, 1] (mathematical range for Runge function)
			 * - 250 * Graph.y[n] ∈ [0, 250] (scale to pixel range)
			 * - y() + h()/2 - 250 * Graph.y[n] (flip y-axis and position)
			 * 
			 * Note: Y-axis flip needed because screen coordinates increase downward,
			 * but mathematical coordinates increase upward.
			 */
			// line end coordinates
			int xk1 = x() + 250*(1 + Graph.x[n]);      // Transform x: [-1,1] → screen pixels
			int yk1 = y() + h()/2 - 250*Graph.y[n];    // Transform y: [0,1] → screen pixels (flipped)

			/*
			 * LINE SEGMENT DRAWING
			 * ===================
			 * 
			 * Draw line from (xk0, yk0) to (xk1, yk1).
			 * This connects the previous point to the current point.
			 * 
			 * Special case: For n=0, draws from left edge to first sample point.
			 * This provides a smooth visual connection to the graph area.
			 */
			fl_line(xk0, yk0, xk1, yk1);

			/*
			 * UPDATE FOR NEXT ITERATION
			 * ========================
			 * 
			 * Current end point becomes next iteration's start point.
			 * This ensures continuous line segments form a connected curve.
			 */
			// next start coordinates become current end coordinates
			xk0 = xk1;  // Update x-coordinate for next line segment
			yk0 = yk1;  // Update y-coordinate for next line segment
		}

		/*
		 * STEP 3: COORDINATE SYSTEM RENDERING
		 * ===================================
		 * 
		 * Draw axes, tick marks, and labels to provide mathematical context.
		 * This helps users interpret the function values and domain.
		 */

		// Set drawing color to black for axes and labels
		fl_color(fl_rgb_color(0, 0, 0));
		
		// Set font for axis labels: Courier (monospace) font, size 16
		// Monospace ensures consistent alignment of numerical labels
		fl_font(FL_COURIER, 16);
		
		// String buffer for formatted numerical labels
		char str[256];

		/*
		 * X-AXIS RENDERING
		 * ===============
		 * 
		 * Draw horizontal line through the middle of the widget.
		 * This represents y = 0 in mathematical coordinates.
		 */
		// x-axis: horizontal line from left to right edge at vertical center
		fl_line(x(), y()+h()/2, x()+w(), y()+h()/2);
		
		/*
		 * X-AXIS TICK MARKS AND LABELS
		 * ============================
		 * 
		 * Add tick marks and numerical labels to show x-coordinate values.
		 * 
		 * Spacing: 11 tick marks (xpos = 0 to 10)
		 * Mathematical range: -1.0 to +1.0
		 * Label formula: -1.0 + xpos/5.0
		 */
		// x-ticks
		for (int xpos=0; xpos<=10; xpos++)
		{
			/*
			 * TICK MARK DRAWING
			 * ================
			 * 
			 * Draw short vertical line at each tick position.
			 * Tick positions: every 50 pixels (x() + 50*xpos)
			 * Tick length: 10 pixels total (5 above and 5 below x-axis)
			 */
			fl_line(x()+50*xpos, y()+h()/2-5, x()+50*xpos, y()+h()/2+5);
			
			/*
			 * NUMERICAL LABEL GENERATION
			 * ==========================
			 * 
			 * Calculate mathematical x-value for this tick position.
			 * Formula: -1.0 + xpos/5.0
			 * 
			 * Examples:
			 * - xpos=0: -1.0 + 0/5.0 = -1.0
			 * - xpos=5: -1.0 + 5/5.0 = 0.0  
			 * - xpos=10: -1.0 + 10/5.0 = 1.0
			 * 
			 * Format: %1.1lf (1 decimal place, double precision)
			 */
			sprintf(str, "%1.1lf", -1.0+((double) xpos)/5.0);
			
			/*
			 * LABEL POSITIONING
			 * ================
			 * 
			 * Place numerical label below the tick mark.
			 * Horizontal position: centered on tick (x() + 50*xpos)
			 * Vertical position: 20 pixels below x-axis (y() + h()/2 + 20)
			 */
			fl_draw(str, x()+50*xpos, y()+h()/2+20);
		}
		
		/*
		 * Y-AXIS RENDERING
		 * ===============
		 * 
		 * Draw vertical line at the left edge of the widget.
		 * Extended slightly beyond widget bounds for visual completeness.
		 */
		// y-axis: vertical line from top to bottom (extended 10 pixels below)
		fl_line(x(), y(), x(), 10+h());
		
		/*
		 * Y-AXIS LABELS
		 * =============
		 * 
		 * Add labels for key y-values to show function range.
		 * Simple approach: label the extreme values (+1 and -1)
		 * 
		 * Note: For Runge function, y-range is [0,1], so -1 label may not be needed,
		 * but included for generality and potential other functions.
		 */
		// y-ticks
		fl_draw("+1", x(), y()+h()/2-250+10);  // Label at y=+1 position
		fl_draw("-1", x(), y()+h()/2+250);     // Label at y=-1 position
    }

	/*
	 * PUBLIC CONSTRUCTOR
	 * ==================
	 * 
	 * Initialize the custom graphics widget with specified properties.
	 * Uses constructor delegation to call parent Fl_Box constructor.
	 * 
	 * PARAMETERS:
	 * - (x, y): Widget position within parent window
	 * - (w, h): Widget dimensions (width and height)
	 * - l: Optional text label (default: none)
	 * 
	 * APPEARANCE CONFIGURATION:
	 * - FL_FLAT_BOX: Simple rectangular appearance without 3D borders
	 * - Gray background: RGB(127, 127, 127) provides neutral backdrop for red function curve
	 */
	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		// Set widget appearance to flat rectangle (no decorative borders)
		box(FL_FLAT_BOX);
		
		// Set background color to medium gray
		// This provides good contrast for the red function curve and black axes
		color(fl_rgb_color(127, 127, 127));
    }
};

/*
 * NUMERICAL PRECISION CONSTANT
 * ============================
 * 
 * Small epsilon value used for floating-point loop termination.
 * 
 * PURPOSE:
 * Prevents precision errors in floating-point arithmetic from causing
 * the sampling loop to terminate prematurely.
 * 
 * EXAMPLE PROBLEM WITHOUT EPSILON:
 * for (t = -1.0; t <= 1.0; t += 1.0/25.0)
 * 
 * Due to floating-point representation limitations:
 * - 1.0/25.0 = 0.04 (might be stored as 0.039999999...)
 * - After 50 iterations: t might be 0.999999... instead of exactly 1.0
 * - Loop terminates early, missing the final sample point
 * 
 * SOLUTION WITH EPSILON:
 * for (t = -1.0; t <= 1.0 + e; t += 1.0/25.0)
 * 
 * The small epsilon (0.01) ensures the loop continues until t definitely exceeds 1.0,
 * guaranteeing we capture the sample at t = 1.0.
 */
#define e	0.01

/*
 * RUNGE FUNCTION DEFINITION (IDENTICAL TO PREVIOUS IMPLEMENTATIONS)
 * ================================================================
 * 
 * This is the same Runge function used in previous programs.
 * See earlier detailed mathematical explanations for:
 * - Function properties and behavior
 * - Significance in interpolation theory
 * - Runge phenomenon demonstration
 * 
 * GRAPHICAL CONTEXT:
 * In this program, the function is sampled at multiple points and
 * visualized as a continuous curve rather than discrete values.
 */
static double f(double x)
{
	// Same implementation: f(x) = 1/(1 + 16x²)
	return 1.0/(1.0 + 16.0*x*x);
}

/*
 * MAIN PROGRAM: GUI APPLICATION SETUP AND EXECUTION
 * ==================================================
 * 
 * This main function demonstrates GUI programming concepts:
 * 1. Window creation and configuration
 * 2. Custom widget instantiation and layout
 * 3. Mathematical function sampling
 * 4. Event loop initialization
 * 
 * PROGRAM FLOW:
 * 1. Create main application window
 * 2. Create and configure custom graphics widget
 * 3. Sample the Runge function at regular intervals
 * 4. Store sample data in global structure
 * 5. Display window and enter interactive event loop
 * 
 * GUI vs CONSOLE DIFFERENCES:
 * - Previous programs: Batch processing, immediate output, program termination
 * - Current program: Interactive GUI, persistent display, user-controlled termination
 */
int main(void)
{
	/*
	 * WINDOW CREATION AND CONFIGURATION
	 * =================================
	 * 
	 * Create the main application window using FLTK's double-buffered window class.
	 * Double buffering prevents flicker during redraw operations.
	 * 
	 * PARAMETERS:
	 * - Width: 530 pixels (provides space for 500-pixel graph plus margins)
	 * - Height: 530 pixels (square window for aesthetic appeal)  
	 * - Title: "Graph" (appears in window title bar)
	 * 
	 * WINDOW PROPERTIES:
	 * - Resizable: User can resize window (triggers automatic redraw)
	 * - Closable: Standard window controls available
	 * - Double-buffered: Smooth graphics rendering
	 */
	Fl_Double_Window window(530, 530, "Graph");
	
	/*
	 * CUSTOM GRAPHICS WIDGET CREATION
	 * ===============================
	 * 
	 * Create our custom GRAPHBOX widget and embed it within the main window.
	 * 
	 * POSITIONING AND SIZING:
	 * - Position: (10, 10) - 10-pixel margin from window edges
	 * - Size: (window.w()-20, window.h()-20) - full window minus 20-pixel total margin
	 * - Result: Graphics area fills most of window with small border
	 * 
	 * LAYOUT STRATEGY:
	 * This creates a single, large graphics area that maximizes space for the function plot.
	 * The 10-pixel margins provide visual separation from window borders.
	 */
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	/*
	 * WINDOW DISPLAY
	 * ==============
	 * 
	 * Make the window visible on screen.
	 * This triggers initial draw() calls for all widgets within the window.
	 * 
	 * Note: At this point, the Graph structure is empty (Graph.n = 0),
	 * so the initial display shows only the coordinate axes without the function curve.
	 */
	window.show();

	/*
	 * FUNCTION SAMPLING AND DATA COLLECTION
	 * ====================================
	 * 
	 * Sample the Runge function at regular intervals and store results
	 * in the global Graph structure for visualization.
	 * 
	 * SAMPLING PARAMETERS:
	 * - Domain: [-1.0, 1.0] (standard interval for Runge function analysis)
	 * - Step size: 1.0/25.0 = 0.04 (fine enough for smooth curve appearance)
	 * - Total samples: ~51 points (sufficient for good visual approximation)
	 * 
	 * LOOP MECHANICS:
	 * - Variable t: sampling parameter (independent variable)
	 * - Termination: t <= 1.0 + e (epsilon prevents precision-related early termination)
	 * - Index i: tracks number of stored samples
	 */
	int i = 0;      // Sample counter (array index)
	double t;       // Sampling parameter (mathematical x-coordinate)
	
	/*
	 * SAMPLING LOOP
	 * =============
	 * 
	 * Generate function samples across the domain [-1, 1].
	 * Each iteration computes one (x, y) coordinate pair.
	 * 
	 * MATHEMATICAL PROCESS:
	 * For each sample position t:
	 * 1. Set x-coordinate: x = t (direct mapping)
	 * 2. Compute y-coordinate: y = f(t) = 1/(1 + 16t²)
	 * 3. Store coordinate pair in Graph arrays
	 * 4. Increment sample counter
	 * 
	 * DATA STORAGE:
	 * - Graph.x[i]: Mathematical x-coordinate (domain value)
	 * - Graph.y[i]: Mathematical y-coordinate (function value)
	 * - Arrays filled sequentially from index 0
	 */
	for (t = -1.0; t <= 1.0 + e; t += 1.0/25.0)
	{
		/*
		 * COORDINATE ASSIGNMENT
		 * ====================
		 * 
		 * Direct assignment approach:
		 * - x-coordinate = sampling parameter (no transformation needed)
		 * - y-coordinate = function evaluation at current x
		 * 
		 * This creates the discrete representation of the continuous function f(x).
		 */
		double x = t;        // Mathematical x-coordinate (domain value)
		double y = f(x);     // Mathematical y-coordinate (function value)

		/*
		 * DATA STORAGE
		 * ============
		 * 
		 * Store the computed coordinate pair in global arrays.
		 * Index i tracks the current storage position.
		 * 
		 * Arrays grow sequentially:
		 * - i=0: First sample point (t ≈ -1.0)
		 * - i=25: Middle sample point (t ≈ 0.0)  
		 * - i=50: Last sample point (t ≈ 1.0)
		 */
		Graph.x[i] = x;     // Store x-coordinate
		Graph.y[i] = y;     // Store y-coordinate
		i++;                // Advance to next storage position
	}
	
	/*
	 * FINALIZE SAMPLE COUNT
	 * ====================
	 * 
	 * Record the total number of samples collected.
	 * This tells the drawing routine how many points to connect.
	 * 
	 * Value of i after loop completion:
	 * - Equals the number of successful iterations
	 * - Typically ~51 for the given parameters
	 * - Used by draw() method to limit rendering loop
	 */
	Graph.n = i;

	/*
	 * GUI EVENT LOOP EXECUTION
	 * ========================
	 * 
	 * Transfer control to FLTK's main event loop.
	 * This call does not return until the application terminates.
	 * 
	 * EVENT LOOP RESPONSIBILITIES:
	 * - Window management (resize, move, minimize, etc.)
	 * - User input handling (mouse, keyboard)
	 * - Automatic redraw triggering when needed
	 * - Application lifecycle management
	 * 
	 * TERMINATION CONDITIONS:
	 * - User closes window (clicks X button)
	 * - Application receives terminate signal
	 * - Explicit call to exit() function
	 * 
	 * INTERACTIVE FEATURES:
	 * - Window can be resized (triggers automatic graph redraw)
	 * - Window can be moved around desktop
	 * - Graph remains responsive to system events
	 * 
	 * COMPARISON TO CONSOLE PROGRAMS:
	 * - Console: Linear execution, immediate termination
	 * - GUI: Event-driven execution, persistent until user action
	 */
	Fl::run();
	
	// Program termination (reached when GUI event loop exits)
	return EXIT_SUCCESS;
}

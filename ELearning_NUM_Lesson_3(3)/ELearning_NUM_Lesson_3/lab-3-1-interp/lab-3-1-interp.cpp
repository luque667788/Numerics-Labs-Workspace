/*
 * POLYNOMIAL INTERPOLATION VISUALIZATION AND COMPARISON PROGRAM
 * =============================================================
 * 
 * GENERAL OVERVIEW:
 * This program demonstrates polynomial interpolation by creating a visual comparison between:
 * 1. **Original Function**: The smooth Runge function f(x) = 1/(1 + 16x²)
 * 2. **Interpolating Polynomial**: A degree-15 polynomial that passes through 16 sample points
 * 3. **Sample Points**: The discrete points used to construct the interpolating polynomial
 * 
 * MATHEMATICAL SIGNIFICANCE:
 * =========================
 * 
 * **POLYNOMIAL INTERPOLATION THEORY:**
 * Given n+1 distinct points (x₀,y₀), (x₁,y₁), ..., (xₙ,yₙ), there exists a unique
 * polynomial P(x) of degree at most n such that P(xᵢ) = yᵢ for all i.
 * 
 * **BARYCENTRIC LAGRANGE INTERPOLATION:**
 * This program uses the barycentric form of Lagrange interpolation:
 * 
 *     P(x) = Σᵢ₌₀ⁿ wᵢ·yᵢ/(x-xᵢ) / Σⱼ₌₀ⁿ wⱼ/(x-xⱼ)
 * 
 * Where wᵢ are the barycentric weights: wᵢ = (-1)ⁱ × C(n,i) for equispaced nodes.
 * 
 * **THE RUNGE PHENOMENON DEMONSTRATION:**
 * This program visually demonstrates the famous Runge phenomenon:
 * - The original function is smooth and well-behaved
 * - High-degree polynomial interpolation with equispaced nodes creates oscillations
 * - Oscillations are most severe near the domain boundaries (x ≈ ±1)
 * - The interpolating polynomial exactly matches the function at sample points
 * - Between sample points, significant deviations can occur
 * 
 * **EDUCATIONAL VALUE:**
 * - Shows why equispaced nodes can be problematic for high-degree interpolation
 * - Motivates the use of Chebyshev nodes or spline interpolation
 * - Demonstrates the trade-off between polynomial degree and interpolation quality
 * - Illustrates the difference between interpolation and approximation
 * 
 * VISUAL REPRESENTATION:
 * ======================
 * 
 * **COLOR CODING:**
 * - **Blue Line**: Original Runge function (smooth, theoretical curve)
 * - **Red Line**: Interpolating polynomial (computed from sample points)
 * - **Red Circles**: Sample points where polynomial exactly matches function
 * 
 * **EXPECTED VISUAL BEHAVIOR:**
 * - Both curves coincide at sample points (red circles)
 * - Near x = 0: Good agreement between curves
 * - Near x = ±1: Significant oscillations in red curve (Runge phenomenon)
 * - Blue curve remains smooth throughout domain
 * - Red curve may exceed display range near boundaries
 * 
 * PROGRAM ARCHITECTURE EVOLUTION:
 * ===============================
 * 
 * **From Previous Programs:**
 * - Builds on mathematical foundations from binomial coefficient calculator
 * - Extends graphics capabilities from function visualization program
 * - Combines interpolation setup from barycentric weights calculator
 * 
 * **New Capabilities:**
 * - Dual function rendering (original vs interpolated)
 * - Interactive interpolation evaluation at arbitrary points
 * - Visual comparison of mathematical concepts
 * - Real-time demonstration of numerical analysis principles
 * 
 * **Advanced Features:**
 * - Robust interpolation algorithm with singularity handling
 * - Enhanced graphics with multiple curve rendering
 * - Sample point visualization for educational clarity
 * - Comprehensive coordinate system display
 */

// filepath: /home/luque/Documents/UNI/sem3/numeriks/labs workspaces/ELearning_NUM_Lesson_3(3)/ELearning_NUM_Lesson_3/lab-3-1-interp/lab-3-1-interp.cpp

/*
 * FLTK GRAPHICS LIBRARY INCLUDES
 * ==============================
 * Same as previous graphics program - see detailed explanations there.
 */
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

// Mathematical functions library for pow(), fabs(), etc.
#include <math.h>

// Maximum number of points for dual-curve visualization
#define	GRAPH_MAX		100

/*
 * ENHANCED GRAPH DATA STRUCTURE
 * =============================
 * 
 * EVOLUTION FROM PREVIOUS PROGRAMS:
 * - Previous: Single y-array for one function
 * - Current: Dual y-arrays for function comparison
 * 
 * STRUCTURE DESIGN:
 * This structure stores data for visualizing two related functions:
 * 1. Original function values (y1)
 * 2. Interpolated function values (y2)
 * 
 * Both functions share the same x-coordinates but have different y-values,
 * enabling direct visual comparison of interpolation accuracy.
 */
struct GRAPH
{
	double  x[GRAPH_MAX+1];   // Shared x-coordinates for both functions
	double y1[GRAPH_MAX+1];   // Original function values f(x)
	double y2[GRAPH_MAX+1];   // Interpolated function values P(x)
	int n;                    // Number of evaluation points (same for both functions)
};

// Global graph data structure for dual-function visualization
static struct GRAPH Graph;

/*a
 * ENHANCED GRAPHICS WIDGET FOR DUAL-FUNCTION VISUALIZATION
 * ========================================================
 * 
 * EVOLUTION FROM PREVIOUS GRAPHICS PROGRAM:
 * - Previous: Single function curve rendering
 * - Current: Dual function curves + sample point markers
 * 
 * NEW CAPABILITIES:
 * 1. **Dual Curve Rendering**: Original function vs interpolating polynomial
 * 2. **Color-Coded Visualization**: Blue (original) vs Red (interpolated)
 * 3. **Sample Point Markers**: Red circles showing interpolation nodes
 * 4. **Enhanced Visual Comparison**: Side-by-side mathematical concepts
 * 
 * RENDERING PIPELINE ENHANCEMENT:
 * 1. Background and coordinate system (same as before)
 * 2. **NEW**: Blue curve for original function
 * 3. **NEW**: Red curve for interpolated function  
 * 4. **NEW**: Red circle markers for sample points
 * 5. Axes and labels (same as before)
 */
class GRAPHBOX : public Fl_Box
{
    /*
     * ENHANCED DRAW METHOD - DUAL FUNCTION RENDERING
     * ==============================================
     * 
     * This method now renders multiple mathematical objects:
     * - Two function curves with different colors
     * - Sample point markers for educational visualization
     * - Same coordinate system as previous program
     * 
     * VISUAL HIERARCHY:
     * 1. Background (gray)
     * 2. Function curves (blue and red lines)
     * 3. Sample points (red circles)
     * 4. Coordinate axes (black lines and text)
     */
    private : void draw(void)
	{
		/*
		 * STEP 1: BACKGROUND RENDERING (UNCHANGED)
		 * ========================================
		 * Same as previous program - see detailed explanation there.
		 */
		// redraw background
		Fl_Box::draw();

		/*
		 * STEP 2: ORIGINAL FUNCTION CURVE (BLUE)
		 * ======================================
		 * 
		 * Render the original Runge function as a blue curve.
		 * This represents the "ground truth" - the function we're trying to interpolate.
		 * 
		 * VISUAL PURPOSE:
		 * - Shows the smooth, ideal behavior of the mathematical function
		 * - Provides baseline for comparing interpolation accuracy
		 * - Blue color chosen for good contrast against red interpolation curve
		 */
		// redraw graph

		// Set color to blue for original function curve
		fl_color(fl_rgb_color(0, 0, 255));

		/*
		 * BLUE CURVE RENDERING LOOP
		 * =========================
		 * 
		 * Same line-drawing algorithm as previous program, but using y1 array.
		 * y1 contains the original function values f(x) at evaluation points.
		 * 
		 * Mathematical significance:
		 * This curve represents the "perfect" function that we're trying to
		 * approximate with polynomial interpolation.
		 */
		// line start coordinates
		int xk0 = x();
		int yk0 = y() + h()/2;

		int n;
		for (n = 0; n < Graph.n; n++)
		{
			// Transform mathematical coordinates to screen pixels (same as before)
			// line end coordinates
			int xk1 = x() + 250*(1 + Graph.x[n]);
			int yk1 = y() + h()/2 - 250*Graph.y1[n];  // Use y1[] for original function

			// Draw line segment connecting previous point to current point
			fl_line(xk0, yk0, xk1, yk1);

			// Update for next iteration
			// next start coordinates become current end coordinates
			xk0 = xk1;
			yk0 = yk1;
		}

		/*
		 * STEP 3: INTERPOLATED FUNCTION CURVE (RED)
		 * =========================================
		 * 
		 * Render the interpolating polynomial as a red curve.
		 * This represents the polynomial that passes through the sample points.
		 * 
		 * VISUAL PURPOSE:
		 * - Shows the polynomial approximation behavior
		 * - Demonstrates interpolation accuracy (or lack thereof)
		 * - Red color creates strong visual contrast with blue original
		 * - Reveals Runge phenomenon oscillations near boundaries
		 */
		// Set color to red for interpolated function curve
		fl_color(fl_rgb_color(255, 0, 0));

		/*
		 * RED CURVE RENDERING LOOP
		 * ========================
		 * 
		 * Same line-drawing algorithm, but using y2 array.
		 * y2 contains the interpolated function values P(x) at evaluation points.
		 * 
		 * Mathematical significance:
		 * This curve shows how well (or poorly) polynomial interpolation
		 * approximates the original function between sample points.
		 */
		// line start coordinates (reset for second curve)
		xk0 = x();
		yk0 = y() + h()/2;

		for (n = 0; n < Graph.n; n++)
		{
			// Transform mathematical coordinates to screen pixels
			// line end coordinates
			int xk1 = x() + 250*(1 + Graph.x[n]);
			int yk1 = y() + h()/2 - 250*Graph.y2[n];  // Use y2[] for interpolated function

			// Draw line segment for interpolated curve
			fl_line(xk0, yk0, xk1, yk1);

			// Update for next iteration
			// next start coordinates become current end coordinates
			xk0 = xk1;
			yk0 = yk1;
		}

		/*
		 * STEP 4: SAMPLE POINT MARKERS (RED CIRCLES)
		 * ==========================================
		 * 
		 * NEW FEATURE: Visualize the interpolation nodes as red circles.
		 * 
		 * EDUCATIONAL PURPOSE:
		 * - Shows exactly where the interpolating polynomial equals the original function
		 * - Helps understand that interpolation is exact at sample points
		 * - Reveals spacing of equispaced nodes
		 * - Visual aid for understanding interpolation vs approximation
		 * 
		 * IMPLEMENTATION:
		 * - Use y2 coordinates (interpolated values) since they equal y1 at sample nodes
		 * - Circle radius: 5 pixels (large enough to see, small enough not to clutter)
		 * - Same red color as interpolated curve for visual consistency
		 */
		for (n = 0; n < Graph.n; n++)
		{
			/*
			 * SAMPLE POINT COORDINATE CALCULATION
			 * ==================================
			 * 
			 * Calculate screen coordinates for each sample point.
			 * Note: We could use either y1[n] or y2[n] since they should be
			 * identical at interpolation nodes. Using y2[n] for consistency
			 * with the red interpolation curve.
			 */
			int xk = x() + 250*(1 + Graph.x[n]);      // X-coordinate in screen pixels
			int yk = y() + h()/2 - 250*Graph.y2[n];   // Y-coordinate in screen pixels

			/*
			 * CIRCLE DRAWING
			 * ==============
			 * 
			 * FLTK circle drawing sequence:
			 * 1. fl_begin_line(): Start line drawing mode
			 * 2. fl_circle(): Draw circle outline at specified center and radius
			 * 3. fl_end_line(): Complete line drawing
			 * 
			 * Circle parameters:
			 * - Center: (xk, yk) - transformed sample point coordinates
			 * - Radius: 5 pixels - visible but not overwhelming
			 * - Color: Red (already set) - matches interpolation curve
			 */
			fl_begin_line();
			fl_circle(xk, yk, 5);
			fl_end_line();
		}

		/*
		 * STEP 5: COORDINATE SYSTEM RENDERING (UNCHANGED)
		 * ===============================================
		 * 
		 * Same axis and labeling code as previous program.
		 * See previous detailed explanations for:
		 * - Axis line drawing
		 * - Tick mark placement
		 * - Numerical label formatting
		 * - Font and color settings
		 */
		fl_color(fl_rgb_color(0, 0, 0));
		fl_font(FL_COURIER, 16);
		char str[256];

		// x-axis
		fl_line(x(), y()+h()/2, x()+w(), y()+h()/2);
		// x-ticks
		for (int xpos=0; xpos<=10; xpos++)
		{
			fl_line(x()+50*xpos, y()+h()/2-5, x()+50*xpos, y()+h()/2+5);
			sprintf(str, "%1.1lf", -1.0+((double) xpos)/5.0);
			fl_draw(str, x()+50*xpos, y()+h()/2+20);
		}
		// y-axis
		fl_line(x(), y(), x(), 10+h());
		// y-ticks
		fl_draw("+1", x(), y()+h()/2-250+10);
		fl_draw("-1", x(), y()+h()/2+250);
    }

	/*
	 * PUBLIC CONSTRUCTOR (UNCHANGED)
	 * =============================
	 * Same as previous program - see detailed explanation there.
	 */
	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		box(FL_FLAT_BOX);
		color(fl_rgb_color(127, 127, 127));
    }
};

/*
 * NUMERICAL PRECISION CONSTANTS
 * =============================
 * 
 * ULTRA-HIGH PRECISION EPSILON:
 * This epsilon value is much smaller than the previous program's 0.01.
 * 
 * PURPOSE IN INTERPOLATION CONTEXT:
 * - Prevents numerical singularities in barycentric interpolation formula
 * - When evaluation point t equals a node xⱼ exactly, division by (t - xⱼ) would cause infinity
 * - This epsilon detects "near-exact" matches and handles them specially
 * - Ensures robust interpolation evaluation across the entire domain
 * 
 * VALUE SIGNIFICANCE:
 * - 1e-15: Close to machine epsilon for double precision
 * - Smaller than typical floating-point rounding errors
 * - Ensures detection of exact node matches within numerical precision
 */
#define e	0.000000000000001

/*
 * BARYCENTRIC LAGRANGE INTERPOLATION FUNCTION
 * ===========================================
 * 
 * MATHEMATICAL FOUNDATION:
 * This function implements the barycentric form of Lagrange interpolation,
 * which is numerically stable and computationally efficient.
 * 
 * BARYCENTRIC INTERPOLATION FORMULA:
 * P(t) = Σⱼ₌₀ⁿ wⱼ·fⱼ/(t-xⱼ) / Σⱼ₌₀ⁿ wⱼ/(t-xⱼ)
 * 
 * Where:
 * - P(t): Interpolating polynomial evaluated at point t
 * - wⱼ: Barycentric weights (precomputed)
 * - fⱼ: Function values at interpolation nodes
 * - xⱼ: Interpolation node positions
 * - n: Degree of interpolating polynomial
 * 
 * ADVANTAGES OF BARYCENTRIC FORM:
 * 1. **Numerical Stability**: Avoids catastrophic cancellation
 * 2. **Computational Efficiency**: O(n) evaluation vs O(n²) for standard Lagrange
 * 3. **Singularity Handling**: Graceful handling of evaluation at nodes
 * 4. **Weight Reusability**: Same weights work for different function data
 * 
 * PARAMETERS:
 * - fvals[]: Function values at interpolation nodes (n+1 elements)
 * - xnodes[]: Interpolation node positions (n+1 elements)  
 * - n: Degree of polynomial (number of nodes = n+1)
 * - barw[]: Precomputed barycentric weights (n+1 elements)
 * - t: Evaluation point where we want P(t)
 * 
 * RETURNS:
 * - Interpolated function value P(t)
 * 
 * ALGORITHM ROBUSTNESS:
 * - Handles exact node matches without division by zero
 * - Numerically stable for all practical evaluation points
 * - Suitable for real-time evaluation in graphics applications
 */
static double LagrangeInterp1D(double fvals[], double xnodes[], int n, double barw[], double t)
{
    /*
     * BARYCENTRIC FORMULA IMPLEMENTATION
     * =================================
     * 
     * The algorithm computes numerator and denominator separately:
     * 
     * Numerator:   Σⱼ₌₀ⁿ wⱼ·fⱼ/(t-xⱼ)
     * Denominator: Σⱼ₌₀ⁿ wⱼ/(t-xⱼ)
     * 
     * Final result: numerator / denominator
     */
    
    // Initialize accumulators for barycentric formula components
    double numt = 0.0;      // Numerator accumulator
    double denomt = 0.0;    // Denominator accumulator
    
	// Loop over all interpolation nodes
	int j;
    for (j=0; j<=n; j++)
	{
        /*
         * DISTANCE CALCULATION AND SINGULARITY CHECK
         * ==========================================
         * 
         * Calculate distance from evaluation point to current node.
         * This is the key quantity that appears in denominators of barycentric formula.
         */
        double tdiff = t - xnodes[j];  // Distance: t - xⱼ
        
        /*
         * BARYCENTRIC TERMS ACCUMULATION
         * =============================
         * 
         * Add contributions from current node to both numerator and denominator.
         * 
         * Numerator term:   wⱼ·fⱼ/(t-xⱼ)
         * Denominator term: wⱼ/(t-xⱼ)
         * 
         * Mathematical significance:
         * - Each node contributes to the interpolation with weight inversely
         *   proportional to its distance from the evaluation point
         * - Closer nodes have larger influence on the result
         * - Barycentric weights wⱼ encode the geometric properties of node distribution
         */
        numt += ((barw[j] / tdiff ) * fvals[j]);    // Weighted function value contribution
        denomt += (barw[j] / tdiff);           // Weight contribution (note: extra '+' is redundant)

        /*
         * SINGULARITY HANDLING
         * ====================
         * 
         * PROBLEM: When t ≈ xⱼ, we have tdiff ≈ 0, causing division by zero.
         * MATHEMATICAL SOLUTION: When t = xⱼ exactly, P(t) = fⱼ (interpolation property).
         * NUMERICAL IMPLEMENTATION: Detect near-zero tdiff and return exact value.
         * 
         * EPSILON COMPARISON:
         * - fabs(tdiff) < e: Absolute value comparison handles both positive and negative differences
         * - Ultra-small epsilon ensures we only trigger this for genuine node matches
         * - Prevents false positives from normal floating-point evaluation
         * 
         * EXACT INTERPOLATION PROPERTY:
         * When evaluation point coincides with a node, the interpolating polynomial
         * must equal the function value at that node by definition.
         */
        if (fabs(tdiff) < e)
		{
            // Exact match detected: return function value at this node
            numt = fvals[j];    // Set numerator to exact function value
            denomt = 1.0;       // Set denominator to 1 (no division needed)
            break;              // Exit loop early (no need to process remaining nodes)
		}
	}
    
    /*
     * FINAL RESULT COMPUTATION
     * =======================
     * 
     * Complete the barycentric interpolation formula by dividing
     * accumulated numerator by accumulated denominator.
     * 
     * NORMAL CASE: numt and denomt contain weighted sums, division gives interpolated value
     * EXACT NODE CASE: numt = fⱼ, denomt = 1.0, division gives exact function value
     * 
     * MATHEMATICAL GUARANTEE:
     * The barycentric formula ensures that the result is always the unique
     * polynomial of degree ≤ n that passes through all given points.
     */
    return numt / denomt;
}

/*
 * MATHEMATICAL UTILITY FUNCTIONS (SAME AS PREVIOUS PROGRAMS)
 * ==========================================================
 * 
 * These functions are identical to previous implementations.
 * See detailed explanations in earlier programs for:
 * - factorial(): Computes n! with overflow protection
 * - binomial(): Computes C(n,k) using factorials
 * - f(): Defines the Runge function for testing interpolation
 */

// Enhanced factorial with long long precision (same as previous programs)
static long long factorial(int n)
{
    long long res = 1;
    int k;
    for (k=2; k<=n; k++)
        res *= k;
    return res;
}

// Enhanced binomial coefficient calculation (same as previous programs)
static long long binomial(int n, int k)
{
    long long a = factorial(n);
    long long b = factorial(k);
    long long c = factorial(n-k);
    long long res = a / b / c;
    return res;
}

// Runge function definition (same as previous programs)
static double f(double x)
{
	return 1.0/(1.0 + 16.0*x*x);
}

/*
 * MAIN PROGRAM: COMPLETE INTERPOLATION DEMONSTRATION
 * ==================================================
 * 
 * PROGRAM EVOLUTION SUMMARY:
 * This represents the culmination of our interpolation program series:
 * 
 * 1. **binomial.c**: Basic combinatorial mathematics
 * 2. **calc.c**: Interpolation setup and weight calculation  
 * 3. **graph.cpp**: Function visualization
 * 4. **interp.cpp**: Complete interpolation with visual comparison
 * 
 * COMPREHENSIVE WORKFLOW:
 * ======================
 * 
 * **PHASE 1: INTERPOLATION SETUP**
 * 1. Generate equispaced interpolation nodes
 * 2. Compute barycentric weights using binomial coefficients
 * 3. Sample original function at interpolation nodes
 * 
 * **PHASE 2: FUNCTION EVALUATION AND COMPARISON**
 * 4. Evaluate both original and interpolated functions at dense grid
 * 5. Store results for dual-curve visualization
 * 
 * **PHASE 3: INTERACTIVE VISUALIZATION**
 * 6. Create GUI window with custom graphics widget
 * 7. Render both functions with different colors
 * 8. Mark interpolation nodes with visual indicators
 * 9. Provide interactive display with coordinate system
 * 
 * EDUCATIONAL OBJECTIVES:
 * ======================
 * 
 * **MATHEMATICAL UNDERSTANDING:**
 * - Demonstrate polynomial interpolation theory in practice
 * - Visualize the Runge phenomenon with equispaced nodes
 * - Show exact interpolation property at sample points
 * - Illustrate limitations of high-degree polynomial interpolation
 * 
 * **COMPUTATIONAL SKILLS:**
 * - Implement robust numerical algorithms
 * - Handle floating-point precision issues
 * - Create educational visualizations
 * - Combine multiple mathematical concepts in one program
 * 
 * **PRACTICAL INSIGHTS:**
 * - Understand when interpolation works well vs poorly
 * - Appreciate the importance of node distribution
 * - Recognize trade-offs in numerical approximation
 * - Motivate advanced techniques (Chebyshev nodes, splines)
 */
int main(void)
{
	/*
	 * GUI INITIALIZATION (SAME AS PREVIOUS GRAPHICS PROGRAM)
	 * =====================================================
	 * See detailed explanations in previous program for:
	 * - Window creation and sizing
	 * - Custom widget instantiation
	 * - Display activation
	 */
	Fl_Double_Window window(530, 530, "Graph");
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);
	window.show();

	/*
	 * INTERPOLATION SETUP PHASE
	 * =========================
	 * 
	 * This section is identical to the calc.c program - it sets up
	 * the interpolation problem by generating nodes, weights, and function samples.
	 */

	// Set interpolation degree (15 means 16 interpolation nodes)
	int n = 15;
	int k;  // Loop counter variable
	
	/*
	 * STEP 1: EQUISPACED NODE GENERATION (SAME AS CALC.C)
	 * ===================================================
	 * Generate 16 equally spaced points in [-1, 1] for interpolation.
	 * See detailed mathematical explanation in calc.c program.
	 */
	// EquispacedNodes
	double xnodes[n+1];
	for (k=0; k<=n; k++) xnodes[k] = k;                          // Initialize with indices
	for (k=0; k<=n; k++) xnodes[k] = -1.0+xnodes[k]*2.0/n;      // Transform to [-1,1]
	
	/*
	 * STEP 2: BARYCENTRIC WEIGHT CALCULATION (SAME AS CALC.C)
	 * =======================================================
	 * Compute weights for barycentric interpolation formula.
	 * Formula: wₖ = (-1)ᵏ × C(n,k)
	 * See detailed mathematical explanation in calc.c program.
	 */
	// EquispacedBarWeights
	double w[n+1];
	for (k=0; k<=n; k++)
    	w[k] = pow(-1.0, k) * binomial(n, k);
	
	/*
	 * STEP 3: FUNCTION SAMPLING AT INTERPOLATION NODES (SAME AS CALC.C)
	 * =================================================================
	 * Evaluate the Runge function at each interpolation node.
	 * These values will be used to construct the interpolating polynomial.
	 * See detailed mathematical explanation in calc.c program.
	 */
	double f_sample[n+1];
	for (k=0; k<=n; k++)
		f_sample[k] = f(xnodes[k]);


		//----------
	/*
	 * DUAL FUNCTION EVALUATION PHASE
	 * ==============================
	 * 
	 * NEW FUNCTIONALITY: Evaluate both original and interpolated functions
	 * at a dense grid of points for smooth visualization.
	 * 
	 * EVALUATION STRATEGY:
	 * - Use same sampling approach as graph.cpp program
	 * - At each evaluation point t, compute:
	 *   1. Original function value: f(t)
	 *   2. Interpolated function value: P(t) using LagrangeInterp1D
	 * - Store both values for dual-curve visualization
	 * 
	 * MATHEMATICAL SIGNIFICANCE:
	 * - Shows interpolation accuracy across entire domain
	 * - Reveals Runge phenomenon oscillations
	 * - Demonstrates exact agreement at interpolation nodes
	 * - Provides visual comparison of approximation quality
	 */
	int i = 0;      // Array index counter
	double t;       // Evaluation parameter
	
	/*
	 * DENSE EVALUATION LOOP
	 * ====================
	 * 
	 * Sample both functions at ~51 points for smooth curve visualization.
	 * Same loop structure as graph.cpp, but with dual function evaluation.
	 */
	for (t = -1.0; t <= 1.0 + e; t += 1.0/25.0)
	{
		/*
		 * COORDINATE AND FUNCTION EVALUATION
		 * =================================
		 * 
		 * For each evaluation point t:
		 * 1. Set x-coordinate (direct assignment)
		 * 2. Compute original function value
		 * 3. Compute interpolated function value using barycentric formula
		 */
		double x = t;                                                           // Evaluation x-coordinate
		double y = f(x);                                                        // Original function value
		double f_interp = LagrangeInterp1D(f_sample, xnodes, n, w, t);        // Interpolated function value

		/*
		 * DUAL DATA STORAGE
		 * ================
		 * 
		 * Store both function values for visualization:
		 * - Graph.x[i]: Evaluation x-coordinate (same for both functions)
		 * - Graph.y1[i]: Original function value f(x) (blue curve)
		 * - Graph.y2[i]: Interpolated function value P(x) (red curve)
		 * 
		 * DATA RELATIONSHIP:
		 * At interpolation nodes: y1[i] ≈ y2[i] (exact interpolation property)
		 * Between nodes: y1[i] vs y2[i] shows approximation accuracy
		 */
		Graph.x[i] = x;          // Store evaluation coordinate
		Graph.y1[i] = y;         // Store original function value (for blue curve)
		Graph.y2[i] = f_interp;  // Store interpolated value (for red curve)
		i++;                     // Advance to next storage position
	}
	
	/*
	 * FINALIZE DATA COUNT
	 * ==================
	 * Record total number of evaluation points for graphics rendering.
	 */
	Graph.n = i;

	/*
	 * INTERACTIVE VISUALIZATION PHASE
	 * ==============================
	 * 
	 * Enter GUI event loop for interactive display.
	 * Same as previous graphics program - see detailed explanation there.
	 * 
	 * VISUAL RESULT:
	 * - Blue curve: Smooth Runge function (original)
	 * - Red curve: Polynomial interpolation (may show oscillations)  
	 * - Red circles: Interpolation nodes (where curves coincide)
	 * - Black axes: Coordinate reference system
	 * 
	 * EXPECTED OBSERVATIONS:
	 * - Perfect agreement at red circle locations
	 * - Good agreement near center (x ≈ 0)
	 * - Possible large oscillations near boundaries (x ≈ ±1)
	 * - Demonstration of Runge phenomenon in action
	 */
	Fl::run();
	return EXIT_SUCCESS;
}

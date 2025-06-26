/*
 * JACOBI ITERATIVE METHOD WITH GRAPHICAL VISUALIZATION
 *
 * GENERAL OVERVIEW:
 * This program implements the Jacobi iterative method to solve a system of linear equations, and visualizes the convergence of the solution using a graphical window (FLTK library).
 *
 * MATHEMATICAL BACKGROUND:
 * - System: 7x₁ - x₂ = 5
 *           3x₁ - 5x₂ = -7
 * - Exact solution: (x₁, x₂) = (1, 2)
 *
 * JACOBI PRINCIPLE:
 * - Rearranged equations:
 *   x₁ = (5 + x₂) / 7
 *   x₂ = (7 + 3x₁) / 5
 * - Each iteration uses only the previous values for both variables (unlike Gauss-Seidel, which uses the latest x₁ for x₂).
 *
 * VISUALIZATION:
 * - Each iteration's (x, y) values are stored and plotted as points in a window.
 * - The graph shows how the Jacobi method converges to the solution.
 *
 * DIFFERENCE FROM PREVIOUS FILES:
 * - This program uses FLTK to create a graphical window and plot the sequence of (x, y) values.
 * - Previous files only printed values to the console; this one provides a visual, step-by-step convergence plot.
 * - The mathematics and iteration logic are the same as the Jacobi console version, but here the results are visualized.
 */

#include <FL/Fl.H>           // FLTK core header
#include <FL/Fl_Window.H>    // FLTK window class
#include <FL/Fl_Box.H>       // FLTK box widget
#include <FL/fl_draw.H>      // FLTK drawing functions
#include <math.h>            // For math functions (not strictly needed here)

#define SEQ_MAX 10+1         // Maximum number of iterations (10 steps + initial)

// Structure to hold the sequence of (x, y) values and window pointer
struct SEQ
{
    float x[SEQ_MAX];        // Array of x₁ values for each iteration
    float y[SEQ_MAX];        // Array of x₂ values for each iteration
    int n;                   // Current number of points (iterations)
    Fl_Window *window;       // Pointer to the FLTK window for redraws
};

static struct SEQ Seq;       // Global sequence object

// Custom FLTK widget for drawing the graph
class GRAPHBOX : public Fl_Box
{
private:
    void draw(void)
    {
        int n;
        // Draw each (x, y) point as a red circle with a black outline
        for (n = 0; n < Seq.n; n++)
        {
            int xk = 500*Seq.x[n];                 // Scale x for window
            int yk = h()/2 - 25*Seq.y[n];          // Scale y and center vertically

            fl_color(fl_rgb_color(255, 0, 0));     // Red fill
            fl_begin_polygon();
            fl_arc(xk, yk, 5, 0, 360);             // Draw filled circle
            fl_end_polygon();

            fl_color(fl_rgb_color(0, 0, 0));       // Black outline
            fl_begin_line();
            fl_circle(xk, yk, 5);                  // Draw circle outline
            fl_end_line();
        }

        fl_color(fl_rgb_color(0, 0, 0));           // Black for axes and text
        fl_font(FL_COURIER, 16);
        char str[256];

        // Draw x-axis
        fl_line(0, h()/2, w(), h()/2);
        // Draw x-axis ticks and labels
        for (int xpos=0; xpos<=10; xpos++)
        {
            fl_line(50*xpos, h()/2-5, 50*xpos, h()/2+5);
            sprintf(str, "%1.1f", ((float) xpos)/10.0);
            fl_draw(str, 50*xpos, h()/2+20);
        }
        // Display latest (x, y) value at the bottom
        sprintf(str, "y(% 1.4f)=% 1.4f", Seq.x[n-1], Seq.y[n-1]);
        fl_draw(str, w()/2, h()-10);
    }
public:
    GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
    {
        /* nothing to be done */
    }
};

const float Pi = 3.1415; // Not used, but defined

// Timer callback for FLTK: performs one Jacobi iteration and updates the graph
static void Timer_CB(void *)
{
    if (Seq.n < SEQ_MAX)
    {
        // Linear System: 7x1-x2=5, 3x1-5x2=-7
        // Jacobi Method: x1=(5+x2)/7, x2=(7+3x1)/5
        float x = Seq.x[(Seq.n)-1];               // Previous x₁
        float y = Seq.y[(Seq.n)-1];               // Previous x₂
        Seq.x[Seq.n] = (5.0 + y)/7.0;             // New x₁ using previous x₂
        Seq.y[Seq.n] = (7.0 + 3.0*x)/5.0;         // New x₂ using previous x₁
        (Seq.n)++;                                // Advance iteration count

        if (Seq.window)
        {
            (Seq.window)->redraw();               // Redraw window to show new point
        }
        Fl::repeat_timeout(2, Timer_CB);          // Schedule next iteration in 2 seconds
    }
}

int main(void)
{
    Fl_Window window(530, 140, "Graph");         // Create window
    GRAPHBOX graphbox(0, 0, 530, 140);            // Create graph widget
    window.show();                                // Show window

    Seq.x[0] = 0.0;                              // Initial guess x₁ = 0
    Seq.y[0] = 0.0;                              // Initial guess x₂ = 0
    Seq.n = 1;                                   // Start with 1 point
    Seq.window = &window;                        // Store window pointer

    Fl::add_timeout(2, Timer_CB);                // Start timer for iterations
    Fl::run();                                   // Enter FLTK event loop
    return EXIT_SUCCESS;
}

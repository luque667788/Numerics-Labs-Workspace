// Top-level summary:
// This FLTK demo visualizes the function f(x)=exp(cos³(x)) over [0,2π].
// • The red polyline is a high-resolution “true” curve (n=100).
// • The blue filled polygons are trapezoidal approximations (n=10).
// • It also computes the integral via the trapezoidal rule and displays “area = …”.

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include <math.h>

#define	GRAPH_MAX		100

// Data container for two plots and the computed integral:
struct GRAPH
{
    double x1[GRAPH_MAX+1]; // x-coords for trapezoidal approx
    double y1[GRAPH_MAX+1]; // y-coords for trapezoidal approx
    int    n1;              // number of trapezoidal sample points

    double x2[GRAPH_MAX+1]; // x-coords for high-res “true” curve
    double y2[GRAPH_MAX+1]; // y-coords for true curve
    int    n2;              // number of true-curve sample points

    double val;             // computed integral value (trapezoidal)
};

static struct GRAPH Graph;

// Custom FLTK widget: draws axes, the two plots, and the area label
class GRAPHBOX : public Fl_Box
{
private:
    void draw(void)
    {
        // 1) Clear background
        Fl_Box::draw();

        // 2) Draw trapezoidal approximation as blue filled trapezoids
        fl_color(fl_rgb_color(0, 0, 255));
        int xk0 = x(), yk0 = y() + h()/2;  // start at left-center
        for (int i = 0; i < Graph.n1; i++)
        {
            // compute next corner scaled by 50 px/unit
            int xk1 = x() + 50*Graph.x1[i];
            int yk1 = y() + h()/2 - 50*Graph.y1[i];
            // draw each trapezoid
            fl_polygon(xk0, y()+h()/2,
                       xk0, yk0,
                       xk1, yk1,
                       xk1, y()+h()/2);
            xk0 = xk1; yk0 = yk1;
        }

        // 3) Draw exact function as red polyline
        fl_color(fl_rgb_color(255, 0, 0));
        fl_line_style(FL_SOLID, 2, NULL);
        xk0 = x(); yk0 = y() + h()/2;
        for (int i = 0; i < Graph.n2; i++)
        {
            int xk1 = x() + 50*Graph.x2[i];
            int yk1 = y() + h()/2 - 50*Graph.y2[i];
            fl_line(xk0, yk0, xk1, yk1);
            xk0 = xk1; yk0 = yk1;
        }

        // 4) Draw axes, ticks and labels
        fl_color(fl_rgb_color(0, 0, 0));
        fl_font(FL_COURIER, 16);
        char str[64];

        // X-axis
        fl_line(x(), y()+h()/2, x()+w(), y()+h()/2);
        for (int t = 0; t <= 10; t++)
        {
            fl_line(x()+50*t, y()+h()/2-5, x()+50*t, y()+h()/2+5);
            sprintf(str, "%d", t);
            fl_draw(str, x()+50*t, y()+h()/2+20);
        }

        // Y-axis
        fl_line(x(), y(), x(), y()+h());
        fl_draw("+5", x(), y()+h()/2-250+10);
        fl_draw("-5", x(), y()+h()/2+250);

        // 5) Display computed integral
        sprintf(str, "area = %.2f", Graph.val);
        fl_draw(str, x()+w()/4, y()+h()/4);
    }

public:
    GRAPHBOX(int x,int y,int w,int h,const char *l=0)
        : Fl_Box(x, y, w, h, l)
    {
        // grey flat background
        box(FL_FLAT_BOX);
        color(fl_rgb_color(127,127,127));
    }
};

// Function to integrate: f(x) = e^(cos^3(x))
static double f(double x)
{
    return exp(cos(x)*cos(x)*cos(x));
}

// Numeric integration using the trapezoidal rule on [a,b] with n subintervals
static double f_trap(double a, double b, int n)
{
    double h   = (b - a)/n;                  // step size
    double res = h*(f(a) + f(b))/2;          // endpoints
    for (int k = 1; k < n; k++)
    {
        double xk = a + k*(b - a)/n;         // current sample point
        res += h * f(xk);                    // add trapezoid area
    }
    return res;
}

int main(void)
{
    // 1) Create window and custom drawing box
    Fl_Double_Window window(530, 530, "Graph");
    GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);
    window.show();

    // 2) Prepare high-res “true” curve data (n=100)
    int    n      = 100;
    double two_pi = 2*acos(-1.0);
    for (int k = 0; k <= n; k++)
    {
        double xk = two_pi * k / n;
        Graph.x2[k] = xk;
        Graph.y2[k] = f(xk);
    }
    Graph.n2 = n+1;

    // 3) Prepare trapezoidal sample data (n=10)
    n = 10;
    for (int k = 0; k <= n; k++)
    {
        double xk = two_pi * k / n;
        Graph.x1[k] = xk;
        Graph.y1[k] = f(xk);
    }
    Graph.n1 = n+1;

    // 4) Compute and store the approximate integral
    Graph.val = f_trap(0, two_pi, 100);

    // 5) Enter FLTK event loop
    Fl::run();
    return EXIT_SUCCESS;
}

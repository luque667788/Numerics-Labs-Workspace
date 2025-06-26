/*
 * TAYLOR SERIES APPROXIMATION AND PLOTTING FOR sin(x)
 *
 * GENERAL OVERVIEW:
 * This program approximates sin(x) using the first four terms of its Taylor series and plots the result using FLTK.
 * The Taylor series for sin(x) is: sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
 * This version also handles periodicity by shifting x by Pi and flipping the sign as needed.
 * The program draws the resulting curve in a window.
 *
 * CODE LOGIC:
 * - RaisePower: Computes f^power for integer power (handles negative powers as reciprocals).
 * - Sin: Sums the first four terms of the Taylor series for sin(x), and handles periodicity for better accuracy.
 * - The main loop fills arrays with (x, sin(x)) values and the graph class draws the curve.
 */

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include <math.h>           // for comparison only

/* Lab 1.3 Write your own function to evaluate the trigonometric function sin(x) */

const float Pi = 3.1415;

/************************************************/
/******************** Sin(x) ********************/

static float RaisePower(float f, int power)
{
    if (power == 0)
        return 1.0;
    
    int sign = 1;

    if (power < 0)              // if power is negative then make positive and later invert result
    {
        sign = -1;
        power *= sign;
    }
    
    float result = f;

    while (power > 1)
    {
        result *= f;

        power--;
    }

    if (sign < 0)
        result = 1.0/result;    // if power was negative then invert result
    
    return result;
}

// sin(x) = x - x^3/3! + x^5/5! - x^7/7! (Taylor series, first four terms)
// This version also handles periodicity for better accuracy
static float Sin(float x)
{
	int sign = 1;

	// sin(x) is periodic so move Taylor-series working-point
	while (x > Pi)
	{
		x -= Pi;
		sign *= -1;
	}
	
    return sign * ( x - RaisePower(x, 3)/6 + RaisePower(x, 5)/120 - RaisePower(x, 7)/5040 );
}

/************************************************/

#define SEQ_MAX 100+1

struct SEQ
{
    float x[SEQ_MAX]; // x values
    float y[SEQ_MAX]; // y = sin(x) values
    int n;            // number of points
};

static struct SEQ Seq;

class GRAPHBOX : public Fl_Box
{
    private : void draw(void)
	{
		// line start coordinates
		int x0 = 0;
		int y0 = h()/2;

		int n;

		fl_color(fl_rgb_color(255, 0, 0));

		for (n = 1; n < Seq.n; n++)
		{
			// line end coordinates
			int x1 = 50*Seq.x[n];
			int y1 = h()/2 - 50*Seq.y[n];

			fl_line(x0, y0, x1, y1);

			// next start coordinates become current end coordinates
			x0 = x1;
			y0 = y1;
		}

		fl_color(fl_rgb_color(0, 0, 0));
		fl_font(FL_COURIER, 16);
		char str[256];

		// x-axis
		fl_line(0, h()/2, w(), h()/2);
		// y-ticks
		for (int xpos=0; xpos<=10; xpos++)
		{
			fl_line(50*xpos, h()/2-5, 50*xpos, h()/2+5);
			sprintf(str, "%i", xpos);
			fl_draw(str, 50*xpos, h()/2+20);
		}
    }

	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		/* nothing to be done */
    }
};

int main(void)
{
	Fl_Window window(530, 140, "Graph");
	GRAPHBOX graphbox(0, 0, 530, 140);
	window.show();

	int n;
	for (n = 1; n < SEQ_MAX; n++)
	{
		Seq.x[n] = (float (n))/10.0;

		Seq.y[n] = Sin(Seq.x[n]);
	}
	Seq.n = n;

	Fl::run();
	return EXIT_SUCCESS;
}

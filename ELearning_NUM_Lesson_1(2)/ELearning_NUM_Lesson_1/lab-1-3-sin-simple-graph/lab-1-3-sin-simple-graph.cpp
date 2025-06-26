/*
 * TAYLOR SERIES APPROXIMATION AND PLOTTING FOR sin(x) (NO PERIODICITY HANDLING)
 *
 * GENERAL OVERVIEW:
 * This program approximates sin(x) using the first four terms of its Taylor series and plots the result using FLTK.
 * The Taylor series for sin(x) is: sin(x) = x - x^3/3! + x^5/5! - x^7/7! + ...
 * This version does NOT handle periodicity, so accuracy decreases for large x.
 * The program draws the resulting curve in a window.
 *
 * CODE LOGIC:
 * - RaisePower: Computes f^power for integer power (handles negative powers as reciprocals).
 * - Sin: Sums the first four terms of the Taylor series for sin(x) (no periodicity handling).
 * - The main loop fills arrays with (x, sin(x)) values and the graph class draws the curve.
 *
 * DIFFERENCE FROM PERIODICITY-HANDLING VERSION:
 * - This version does not shift x or flip the sign for large x, so the Taylor approximation is only accurate for small x.
 * - The periodicity-handling version is more accurate for larger x.
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
static float Sin(float x)
{
    return ( x - RaisePower(x, 3)/6 + RaisePower(x, 5)/120 - RaisePower(x, 7)/5040 );
    // 3! = 6, 5! = 120, 7! = 5040
}

/************************************************/

#define SEQ_MAX 100+1

struct SEQ
{
    float x[SEQ_MAX]; // x values
    float y[SEQ_MAX]; // y = sin(x) values
    int n;            // number of points

    Fl_Window *window;
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
		// x-y display
		sprintf(str, "y(% 1.4f)=% 1.4f", Seq.x[n-1], Seq.y[n-1]);
		fl_draw(str, w()/2, h()-10);
    }

	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		/* nothing to be done */
    }
};

static void Timer_CB(void *)
{
	if (Seq.n < SEQ_MAX)
	{
		Seq.x[Seq.n] = (float (Seq.n))/10.0;

		Seq.y[Seq.n] = Sin(Seq.x[Seq.n]);

		// for comparison only
		// Seq.y[Seq.n] = sin(Seq.x[Seq.n]);

		(Seq.n)++;

		if (Seq.window)
		{
			(Seq.window)->redraw();
		}
		
		Fl::repeat_timeout(0.1, Timer_CB);
	}
}

int main(void)
{
	Fl_Window window(530, 140, "Graph");
	GRAPHBOX graphbox(0, 0, 530, 140);
	window.show();

	Seq.x[0] = 0.0;
	Seq.y[0] = 0.0;
	Seq.n = 1;

	Seq.window = &window;

	Fl::add_timeout(0, Timer_CB);

	Fl::run();
	return EXIT_SUCCESS;
}

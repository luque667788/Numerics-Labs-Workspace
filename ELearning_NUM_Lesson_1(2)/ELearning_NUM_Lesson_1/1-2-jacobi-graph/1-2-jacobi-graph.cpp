#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include <math.h>

#define	SEQ_MAX		10+1

struct SEQ
{
	float x[SEQ_MAX];
	float y[SEQ_MAX];
	int n;

	Fl_Window *window;
};

static struct SEQ Seq;

class GRAPHBOX : public Fl_Box
{
    private : void draw(void)
	{
		int n;

		for (n = 0; n < Seq.n; n++)
		{
			int xk = 500*Seq.x[n];
			int yk = h()/2 - 25*Seq.y[n];

			fl_color(fl_rgb_color(255, 0, 0));
			fl_begin_polygon();
			fl_arc(xk, yk, 5, 0, 360);
			fl_end_polygon();

			fl_color(fl_rgb_color(0, 0, 0));
			fl_begin_line();
			fl_circle(xk, yk, 5);
			fl_end_line();
		}

		fl_color(fl_rgb_color(0, 0, 0));
		fl_font(FL_COURIER, 16);
		char str[256];

		// x-axis
		fl_line(0, h()/2, w(), h()/2);
		// x-ticks
		for (int xpos=0; xpos<=10; xpos++)
		{
			fl_line(50*xpos, h()/2-5, 50*xpos, h()/2+5);
			sprintf(str, "%1.1f", ((float) xpos)/10.0);
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

const float Pi = 3.1415;

static void Timer_CB(void *)
{
	if (Seq.n < SEQ_MAX)
	{
		// Linear System: 7x1-x2=5, 3x1-5x2=-7
		// Algebraic Solution: (1 2)
		// Jacobi Method: x1=(5+x2)/7, x2=(7+3x1)/5

		float x = Seq.x[(Seq.n)-1];
		float y = Seq.y[(Seq.n)-1];
		Seq.x[Seq.n] = (5.0 + y)/7.0;
		Seq.y[Seq.n] = (7.0 + 3.0*x)/5.0;
		(Seq.n)++;

		if (Seq.window)
		{
			(Seq.window)->redraw();
		}
		
		Fl::repeat_timeout(2, Timer_CB);
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

	Fl::add_timeout(2, Timer_CB);

	Fl::run();
	return EXIT_SUCCESS;
}

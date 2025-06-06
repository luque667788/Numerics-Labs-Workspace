#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Draw.H>

#include <math.h>

#define	GRAPH_MAX	36+1

struct GRAPH
{
	double x[GRAPH_MAX+1];
	int n;

	Fl_Window *window;
};

static struct GRAPH Graph;

class GRAPHBOX : public Fl_Box
{
    private : void draw(void)
	{
		// redraw background

		Fl_Box::draw();

		fl_color(fl_rgb_color(0, 0, 0));
		fl_line(x(), y()+h()/2, x()+w(), y()+h()/2);
		fl_line(x(), y(), x(), y()+h());

		// redraw graph

		fl_color(fl_rgb_color(127, 0, 0));

		// line start coordinates
		int xk0 = 10;
		int yk0 = 10 + h()/2;

		int n;
		for (n=1; n<Graph.n; n++)
		{
			// line end coordinates
			int xk1 = x() + 10*(n + 1);
			int yk1 = y() + h()/2 - 100*Graph.x[n];
			
			fl_line(xk0, yk0, xk0, yk1);
			fl_line(xk0, yk1, xk1, yk1);

			// next start coordinates become current end coordinates
			xk0 = xk1;
			yk0 = yk1;
		}

		fl_color(fl_rgb_color(0, 0, 0));
		fl_font(FL_COURIER, 16);
		char str[256];
		n = Graph.n-1;
		sprintf(str, "x(%2d)=% 1.4lf", n, Graph.x[n]);
		fl_draw(str, x()+w()/2, y()+h()-20);
    }

	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		box(FL_FLAT_BOX);
		color(fl_rgb_color(127, 127, 127));
    }
};

static void Timer_CB(void *)
{
	if (Graph.n < GRAPH_MAX)
	{
		double x = Graph.x[Graph.n-1];
		x = x*x - 1.0;
		Graph.x[Graph.n] = x;
		Graph.n++;

		if (Graph.window)
			Graph.window->redraw();
		
		Fl::repeat_timeout(0.5, Timer_CB);
	}
}

int main(void)
{
	Fl_Window window(400, 400, "Graph");
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	window.show();

	Graph.window = &window;

	Graph.x[0] = -1.5;
	// Graph.x[0] = -1.618;
	// Graph.x[0] = -1.618-0.000033;
	// Graph.x[0] = -1.618-0.000034;
	Graph.n = 1;

	// draw graph
	Fl::add_timeout(0, Timer_CB);

	Fl::run();
	return EXIT_SUCCESS;
}

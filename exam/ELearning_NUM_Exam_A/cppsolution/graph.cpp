#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>

#include <math.h>

#define	GRAPH_MAX		100

struct GRAPH
{
	double t[GRAPH_MAX+1];
	double x1[GRAPH_MAX+1];
	double x2[GRAPH_MAX+1];
	double x3[GRAPH_MAX+1];
	int n;
};

static struct GRAPH Graph;

class GRAPHBOX : public Fl_Box
{
    private : void draw(void)
	{
		// redraw background

		Fl_Box::draw();

		// redraw graph

		fl_color(fl_rgb_color(0, 0, 255));

		// line start coordinates
		int xk0 = x();
		int yk0 = y() + h()/2;

		int n;
		for (n = 0; n < Graph.n; n++)
		{
			// line end coordinates
			int xk1 = x() + 50*Graph.t[n];
			int yk1 = y() + h()/2 - 250*Graph.x1[n];

			fl_line(xk0, yk0, xk1, yk1);

			// next start coordinates become current end coordinates
			xk0 = xk1;
			yk0 = yk1;
		}

		fl_color(fl_rgb_color(255, 0, 0));

		// line start coordinates
		xk0 = x();
		yk0 = y() + h()/2;

		for (n = 0; n < Graph.n; n++)
		{
			// line end coordinates
			int xk1 = x() + 50*Graph.t[n];
			int yk1 = y() + h()/2 - 250*Graph.x2[n];

			fl_line(xk0, yk0, xk1, yk1);

			// next start coordinates become current end coordinates
			xk0 = xk1;
			yk0 = yk1;
		}

		fl_color(fl_rgb_color(255, 128, 0));

		// line start coordinates
		xk0 = x();
		yk0 = y() + h()/2;

		for (n = 0; n < Graph.n; n++)
		{
			// line end coordinates
			int xk1 = x() + 50*Graph.t[n];
			int yk1 = y() + h()/2 - 250*Graph.x3[n];

			fl_line(xk0, yk0, xk1, yk1);

			// next start coordinates become current end coordinates
			xk0 = xk1;
			yk0 = yk1;
		}

		fl_color(fl_rgb_color(0, 0, 0));
		fl_font(FL_COURIER, 16);
		char str[256];

		// x-axis
		fl_line(x(), y()+h()/2, x()+w(), y()+h()/2);
		// x-ticks
		for (int xpos=0; xpos<=10; xpos++)
		{
			fl_line(x()+50*xpos, y()+h()/2-5, x()+50*xpos, y()+h()/2+5);
			sprintf(str, "%i", xpos);
			fl_draw(str, x()+50*xpos, y()+h()/2+20);
		}
		// y-axis
		fl_line(x(), y(), x(), 10+h());
		// y-ticks
		fl_draw("+1", x(), y()+h()/2-250+10);
		fl_draw("-1", x(), y()+h()/2+250);
    }

	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		box(FL_FLAT_BOX);
		color(fl_rgb_color(127, 127, 127));
    }
};

int main(void)
{
	Fl_Double_Window window(530, 530, "Graph");
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	window.show();

	// sampling time
	double T = 0.1;
	int n = 100;

	int k;
	double t[n+1];
	double u[n+1];
	double x[n+1];
	double a = 1.5;
	double x0 = 0.0;

	for (k=0; k<=n; k++)
		t[k] = 0.0 + k*T;
	for (k=0; k<=n; k++)
		u[k] = exp(-pow(t[k] - 5.0, 10.0));

	// Euler method
	x[0] = x0;
	for (k=0; k<n; k++)
		x[k+1] = (1-a*T)*x[k] + T*u[k];

	for (k=0; k<=n; k++)
	{
		Graph.t[k]  = t[k];
		Graph.x1[k] = u[k];    // input u(t)
		Graph.x2[k] = x[k];    // Euler solution x(t)
		Graph.x3[k] = 0.0;     // unused, or set to another solution if desired
	}
	Graph.n = k;

	Fl::run();
	return EXIT_SUCCESS;
}

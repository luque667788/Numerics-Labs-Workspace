#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Draw.H>

#define	GRAPH_MAX		100

struct GRAPH
{
	double x[GRAPH_MAX+1];
	double y[GRAPH_MAX+1];
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

		fl_color(fl_rgb_color(255, 0, 0));

		// line start coordinates
		int xk0 = x();
		int yk0 = y() + h()/2;

		int n;
		for (n = 0; n < Graph.n; n++)
		{
			// line end coordinates
			int xk1 = x() + 250*(1 + Graph.x[n]);
			int yk1 = y() + h()/2 - 250*Graph.y[n];

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
			sprintf(str, "%1.1lf", -1.0+((double) xpos)/5.0);
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

#define e	0.01

static double f(double x)
{
	return 1.0/(1.0 + 16.0*x*x);
}

int main(void)
{
	Fl_Double_Window window(530, 530, "Graph");
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	window.show();

	int i = 0;
	double t;
	for (t = -1.0; t <= 1.0 + e; t += 1.0/25.0)
	{
		double x = t;
		double y = f(x);

		Graph.x[i] = x;
		Graph.y[i] = y;
		i++;
	}
	Graph.n = i;

	Fl::run();
	return EXIT_SUCCESS;
}

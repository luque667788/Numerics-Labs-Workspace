#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Draw.H>

#include <math.h>
#include <time.h>

#include <lapacke.h>

#define	GRAPH_MAX		1000

struct GRAPH
{
	double x1[GRAPH_MAX+1];
	double y1[GRAPH_MAX+1];
	double x2[GRAPH_MAX+1];
	double y2[GRAPH_MAX+1];
	double x3[GRAPH_MAX+1];
	double y3[GRAPH_MAX+1];
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
			int xk1 = x() + 100*Graph.x1[n];
			int yk1 = y() + h()/2 - 100*Graph.y1[n];

			fl_line(xk0, yk0, xk1, yk1);

			// next start coordinates become current end coordinates
			xk0 = xk1;
			yk0 = yk1;
		}

		fl_color(fl_rgb_color(0, 0, 255));

		for (n = 0; n < Graph.n; n++)
		{
			int xk = x() + 100*Graph.x2[n];
			int yk = y() + h()/2 - 100*Graph.y2[n];

			fl_begin_line();
			fl_circle(xk, yk, 5);
			fl_end_line();
		}

		fl_color(fl_rgb_color(0, 255, 255));

		for (n = 0; n < Graph.n; n++)
		{
			int xk = x() + 100*Graph.x3[n];
			int yk = y() + h()/2 - 100*Graph.y3[n];

			fl_point(xk, yk);
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
			sprintf(str, "%.1f", ((double) xpos)/2.0);
			fl_draw(str, x()+50*xpos, y()+h()/2+20);
		}
		// y-axis
		fl_line(x(), y(), x(), 10+h());
		// y-ticks
		fl_draw("+2.5", x(), y()+h()/2-250+10);
		fl_draw("-2.5", x(), y()+h()/2+250);
    }

	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		box(FL_FLAT_BOX);
		color(fl_rgb_color(127, 127, 127));
    }
};

static double fx(double t, double a, double b, double th, double x0)
{
	return a*cos(th)*cos(t) - b*sin(th)*sin(t) + x0;
}

static double fy(double t, double a, double b, double th, double y0)
{
	return a*sin(th)*sin(t) + b*cos(th)*cos(t) + y0;
}

int main(void)
{
	Fl_Double_Window window(530, 530, "Graph");
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	window.show();

	// linear two-dimensional least-squares fit

	double a  = 2.0;
	double b  = 1.5;
	double th = acos(-1.0)/8.0;
	double x0 = 2.0;
	double y0 = 0.0;

	int n = 100;
	int k;
	double ts[n+1];
	for (k=0; k<=n; k++)
		ts[k] = 0.0 + 2.0*acos(-1.0)*k/n;
	
	double xs[n+1];
	for (k=0; k<=n; k++)
    	xs[k] = fx(ts[k], a, b, th, x0);
	double ys[n+1];
	for (k=0; k<=n; k++)
    	ys[k] = fy(ts[k], a, b, th, y0);
	
	// simulate measured data
	srand(clock());
	double x[n+1];
	for (k=0; k<=n; k++)
	{
		double d = rand();
		d /= RAND_MAX;
    	x[k] = xs[k] + 0.5*(-0.5 + d);
	}
	srand(clock());
	double y[n+1];
	for (k=0; k<=n; k++)
	{
		double d = rand();
		d /= RAND_MAX;
    	y[k] = ys[k] + 0.5*(-0.5 + d);
	}

	// linear two-dimensional least-squares fit
	// A = [x.^2; y.^2; x.*y; x; y]

	double A[n+1][5];
	for (k=0; k<=n; k++)
	{
		A[k][0] = x[k]*x[k];
		A[k][1] = y[k]*y[k];
		A[k][2] = x[k]*y[k];
		A[k][3] = x[k];
		A[k][4] = y[k];
	}

	// B = ones(n+1, 1)

	double B[n+1];
	for (k=0; k<=n; k++)
		B[k] = 1.0;

	// LAPACK::DGELS solves overdetermined or underdetermined linear systems

	lapack_int info;
	// LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', A_rows, A_columns, B_columns, A, A_columns, B, B_columns)))
	if (info = LAPACKE_dgels(LAPACK_ROW_MAJOR, 'N', n, 5, 1, (double *) A, 5, (double *) B, 1))
		return EXIT_FAILURE;
	
	double p[5] = {B[0], B[1], B[2], B[3], B[4]};
	
	for (k=0; k<=n; k++)
	{
		Graph.x1[k] = xs[k];
		Graph.y1[k] = ys[k];
		Graph.x2[k] = x[k];
		Graph.y2[k] = y[k];
	}

	double D = 0.025;
	double X, Y;
	k = 0;
	for (X=-0.5; X<=4.5; X+=D)
	{
    	for (Y=-2.5; Y<=2.5; Y+=D)
		{
        	double Z = p[0]*X*X + p[1]*Y*Y + p[2]*X*Y + p[3]*X + p[4]*Y - 1.0;
        	if (Z*Z < 0.0001)
			{
				Graph.x3[k] = X;
				Graph.y3[k] = Y;
				k++;
			}
		}
	}
	Graph.n = k;

	Fl::run();
	return EXIT_SUCCESS;
}

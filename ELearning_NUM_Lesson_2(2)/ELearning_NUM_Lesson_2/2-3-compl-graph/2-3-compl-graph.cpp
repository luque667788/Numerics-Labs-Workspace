#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Draw.H>

#define SIZE	600

struct GRAPH
{
	int plane[SIZE+1][SIZE+1];
};

static struct GRAPH Graph;

class GRAPHBOX : public Fl_Box
{
    private : void draw(void)
	{
		int xk, yk;
		for (xk=0; xk<SIZE; xk++)
			for (yk=0; yk<SIZE; yk++)
			{
				int n = Graph.plane[xk][yk];

				fl_color(fl_rgb_color(n, n, n));

				fl_point(x() + xk, y() + h() - 1 - yk);
			}
    }

	public : GRAPHBOX(int x, int y, int w, int h, const char *l=0) : Fl_Box(x, y, w, h, l)
	{
		/* nothing to be done */
    }
};

#define NMAX	250		// maximum iterations

static int Mandelbrot_findDepth(double a, double b)
{
	int n;
	double re, im, x = 0, y = 0, d = 0;
	
	for (n=0; n<NMAX; n++)		// iterations
	{
		re = x*x - y*y + a;		// value
		im = 2*x*y + b;
		d = re*re + im*im;		// distance
		if (d > 4.0)			// if > 4 : diverging
			break;
		x = re;					// next value
		y = im;
	}
	
	return n;	// iterations done
}

static void Mandelbrot(double rmin, double rmax, double imin, double imax)
{
	double dr, di, re, im;
	
	dr = (rmax-rmin)/SIZE;		// step size
	di = (imax-imin)/SIZE;
	re = rmin - dr;
	
	int xk, yk;
	for (xk=0; xk<SIZE; xk++)
	{
		re += dr;				// real part increment
		im = imin - di;

		for (yk=0; yk<SIZE; yk++)
		{
			im += di;			// imaginary part increment
			
			Graph.plane[xk][yk] = NMAX - Mandelbrot_findDepth(re, im);
		}
	}
}

int main(void)
{
	Fl_Window window(620, 620, "Graph");
	GRAPHBOX graphbox(10, 10, window.w()-20, window.h()-20);

	window.show();

	// Mandelbrot series

	Mandelbrot(-2.0, 2.0, -2.0, 2.0);

	Fl::run();
	return EXIT_SUCCESS;
}

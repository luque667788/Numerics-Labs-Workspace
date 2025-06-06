#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static double rotation(double ax, double ay, double bx, double by, double cx, double cy)
{
	return (by - ay) * (cx - bx) - (bx - ax) * (cy - by);
}

static int convex(double Sx[], double Sy[], double Hx[], double Hy[], int n)
{
	if (n < 3)		// set >= three points ?
		return 0;
	
	// find left-most point
	int l = 0;
	int i;
	for (i=1; i<n; i++)
		if (Sx[i] < Sx[l])
			l = i;
	
	int k = 0;
	int p = l;
	while (true)
	{
		int q = (p + 1) % n;
		
		// find convex hull
		for (i=0; i<n; i++)
			if (rotation(Sx[p], Sy[p], Sx[i], Sy[i], Sx[q], Sy[q]) < 0)
				q = i;	// if counterclockwise
		
		// add to convex hull set
		Hx[k] = Sx[q];
		Hy[k] = Sy[q];
		k++;

		if (k > n-1)
			break;

		// back to left-most point ?
		if (q == l)
			break;
		
		p = q;
	}

	return k;
}

int main(void)
{
	double Sx[] = {1, 1, 2, 3};
	double Sy[] = {1, 3, 2, 2};

	int n = 4;

	double Hx[n+1];
	double Hy[n+1];

	printf("The convex hull is:\n");

	n = convex(Sx, Sy, Hx, Hy, n);

	int i;
	for (i=0; i<n; i++)
		printf("(%.1f, %.1f)\n", Hx[i], Hy[i]);

    return EXIT_SUCCESS;
}

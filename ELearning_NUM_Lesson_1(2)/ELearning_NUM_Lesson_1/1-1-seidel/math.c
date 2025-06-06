#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <time.h>

static void WAIT(double time)
{
	int cnt = clock();
	int ticks = (int) (time*CLOCKS_PER_SEC);
	while (true)
	{
		if (clock() > (cnt + ticks))
			return;
	}
}

int main(void)
{
	float x = 0.0;
	float y = 0.0;
	int n = 1;
	
	while (n < 10)
	{
		printf("xy(%2d) = (% 1.2f, % 1.2f)\n", n, x, y);

		WAIT(1.0);

		// Linear System: 7x1-x2=5, 3x1-5x2=-7
		// Algebraic Solution: (1 2)
		// Seidel Method: x1=(5+x2)/7, x2=(7+3x1)/5

		x = (5.0 + y)/7.0;
		y = (7.0 + 3.0*x)/5.0;
		
		n++;
	}

    return EXIT_SUCCESS;
}

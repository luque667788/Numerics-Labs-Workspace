// math.c - Fine-grained brute-force optimization for a linear objective with constraints
//
// This program searches for the maximum of the function f(x, y) = 143x + 60y
// subject to the following constraints:
//   120x + 210y <= 15000
//   110x + 30y  <= 4000
//   x + y       <= 75
//   0 <= x, 0 <= y
//
// The search is performed over a fine grid by incrementing x and y in small steps (1/TICKS),
// allowing for a more precise (non-integer) solution than a simple integer brute-force search.
// All feasible (x, y) pairs are checked, and the one yielding the highest value of f(x, y)
// is reported at the end.
//
// Key variables:
//   XMAX, YMAX: Upper bounds for x and y (search space)
//   TICKS: Number of subdivisions per unit (controls grid fineness)
//   xk, yk, zk: Arrays to store all feasible solutions and their objective values
//   xs, ys, zs: Store the best (x, y) and corresponding maximum value found
//
// The nested loops increment x and y by 1.0/TICKS, checking all constraints for each pair.
// If a pair is feasible, it is stored, and if it yields a new maximum, the best solution is updated.
//
// At the end, the program prints the best (x, y) and the corresponding maximum value.

#include <stdio.h>
#include <stdlib.h>

// function
// f = 143*x + 60*y

// constraints
// 120x + 210y <= 15000
// 110x + 30y <= 4000
// x + y <= 75
// 0 <= x and 0 <= y

#define XMAX    75
#define YMAX    75
#define TICKS   20

static double xk[XMAX*TICKS*YMAX*TICKS+1];
static double yk[XMAX*TICKS*YMAX*TICKS+1];
static double zk[XMAX*TICKS*YMAX*TICKS+1];

int main(void)
{
    // search for maximum under constraints using a fine grid
    double xs = 0; // x value for max f(x, y)
    double ys = 0; // y value for max f(x, y)
    double zs = 0; // max value of f(x, y)
    int k  = 0;    // counter for feasible solutions
    double x;
    double y;
    // Brute-force search over all (x, y) pairs with step size 1/TICKS
    for (x=0; x<XMAX; x+=1.0/TICKS)
    {
        for (y=0; y<YMAX; y+=1.0/TICKS)
        {
            // Check all constraints for (x, y)
            if ((120*x + 210*y) <= 15000)
            {
                if ((110*x + 30*y) <= 4000)
                {
                    if ((x + y) <= 75)
                    {
                        // If feasible, store the solution
                        zk[k] = 143*x + 60*y;
                        xk[k] = x;
                        yk[k] = y;
                        // Update best solution if this is better
                        if (zk[k] > zs)
                        {
                            zs = zk[k];
                            xs = x;
                            ys = y;
                        }
                        k++;
                    }
                }
            }
        }
    }
    int n = k-1; // number of feasible solutions found (not used further)
    printf("Found %i feasible solutions.\n", n);
    // Print the best solution found
    printf("f(%.1f, %.1f) = %.1f\n", xs, ys, zs);
    return EXIT_SUCCESS;
}

// math.c - Brute-force optimization for a linear objective with constraints
//
// This program finds integer values of x and y that maximize f(x, y) = 143x + 60y
// subject to the following constraints:
//   120x + 210y <= 15000
//   110x + 30y  <= 4000
//   x + y       <= 75
//   0 <= x, 0 <= y
//
// The program searches all possible (x, y) pairs within the allowed range and
// prints the maximum value found and the corresponding (x, y).

#include <stdio.h>
#include <stdlib.h>

// Define the maximum values for x and y (search space upper bounds)
#define XMAX    75
#define YMAX    75

// Arrays to store feasible solutions (xk, yk) and their objective values (zk)
static int xk[XMAX*YMAX+1]; // x values for feasible points
static int yk[XMAX*YMAX+1]; // y values for feasible points
static int zk[XMAX*YMAX+1]; // objective values for feasible points

int main(void)
{
    // Variables to store the best solution found
    int xs = 0; // x value for max f(x, y)
    int ys = 0; // y value for max f(x, y)
    int zs = 0; // max value of f(x, y)
    int k  = 0; // counter for feasible solutions
    int x;
    int y;
    // Brute-force search over all possible (x, y) pairs
    for (x=0; x<XMAX; x++)
    {
        for (y=0; y<YMAX; y++)
        {
            // Check all constraints for (x, y)
            if ((120*x + 210*y) <= 15000)
            {
                if ((110*x + 30*y) <= 4000)
                {
                    if ((x + y) <= 75)
                    {
                        // If feasible, store the solution
                        zk[k] = 143*x + 60*y; // objective value
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
    printf("f(%i, %i) = %i\n", xs, ys, zs);

    // Write feasible solutions to a file for gnuplot
    FILE *fp = fopen("feasible_points.dat", "w");
    if (fp) {
        for (int i = 0; i < k; i++) {
            fprintf(fp, "%d %d %d\n", xk[i], yk[i], zk[i]);
        }
        fclose(fp);
        printf("Feasible points written to feasible_points.dat\n");
    } else {
        printf("Error: Could not write feasible_points.dat\n");
    }

    // Write the best solution to a file for gnuplot highlighting
    fp = fopen("best_point.dat", "w");
    if (fp) {
        fprintf(fp, "%d %d %d\n", xs, ys, zs);
        fclose(fp);
        printf("Best point written to best_point.dat\n");
    } else {
        printf("Error: Could not write best_point.dat\n");
    }

    // Print gnuplot instructions
    printf("\nTo visualize feasible solutions and the optimum, run:\n");
    printf("gnuplot -persist <<EOF\n");
    printf("set xlabel 'x'\n");
    printf("set ylabel 'y'\n");
    printf("set zlabel 'f(x,y)'\n");
    printf("set title 'Feasible Solutions and Optimum'\n");
    printf("splot 'feasible_points.dat' using 1:2:3 with points pt 7 lc rgb 'blue' title 'Feasible',\\\n");
    printf("      'best_point.dat' using 1:2:3 with points pt 7 ps 2 lc rgb 'red' title 'Optimum'\n");
    printf("EOF\n");
    return EXIT_SUCCESS;
}

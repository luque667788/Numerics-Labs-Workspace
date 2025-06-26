
/*
 * CONVEX HULL CALCULATION PROGRAM
 * 
 * GENERAL OVERVIEW:
 * This program implements the Gift Wrapping Algorithm (also known as Jarvis March)
 * to find the convex hull of a set of 2D points. A convex hull is the smallest 
 * convex polygon that contains all given points - imagine stretching a rubber band 
 * around all points, the resulting shape is the convex hull.
 * 
 * The algorithm works by:
 * 1. Finding the leftmost point (guaranteed to be on the hull)
 * 2. Starting from this point, finding the next point that makes the most 
 *    counterclockwise turn with respect to all other points
 * 3. Repeating until we return to the starting point
 * 
 * Time Complexity: O(nh) where n is number of points and h is number of hull points
 * Space Complexity: O(h) for storing hull points
 */

#include <stdio.h>     // For printf function
#include <stdlib.h>    // For EXIT_SUCCESS constant
#include <stdbool.h>   // For bool type and true/false values

/*
 * ROTATION FUNCTION - Cross Product Based Orientation Test
 * 
 * This function calculates the cross product of vectors AB and BC to determine
 * the orientation of three points A, B, and C.
 * 
 * Parameters:
 *   ax, ay: coordinates of point A
 *   bx, by: coordinates of point B  
 *   cx, cy: coordinates of point C
 * 
 * Returns:
 *   > 0: counterclockwise turn (left turn)
 *   < 0: clockwise turn (right turn) 
 *   = 0: collinear points
 * 
 * Mathematical explanation:
 * Cross product = (B-A) × (C-B) = (bx-ax)(cy-by) - (by-ay)(cx-bx)
 * This is equivalent to the determinant of the 2x2 matrix formed by vectors AB and BC
 */
static double rotation(double ax, double ay, double bx, double by, double cx, double cy)
{
	// Calculate cross product to determine orientation
	// Vector AB = (bx-ax, by-ay), Vector BC = (cx-bx, cy-by)
	// Cross product = AB × BC = (by-ay)(cx-bx) - (bx-ax)(cy-by)
	return (by - ay) * (cx - bx) - (bx - ax) * (cy - by);
}

/*
 * CONVEX HULL FUNCTION - Gift Wrapping Algorithm Implementation
 * 
 * This function implements the Gift Wrapping (Jarvis March) algorithm to find
 * the convex hull of a set of 2D points.
 * 
 * Parameters:
 *   Sx[], Sy[]: Input arrays containing x and y coordinates of points
 *   Hx[], Hy[]: Output arrays to store convex hull points coordinates
 *   n: Number of input points
 * 
 * Returns:
 *   Number of points in the convex hull (0 if less than 3 input points)
 * 
 * Algorithm Steps:
 * 1. Find the leftmost point (guaranteed to be on convex hull)
 * 2. Starting from leftmost point, find the next hull point by checking
 *    which point makes the most counterclockwise turn
 * 3. Continue until we return to the starting point
 */
static int convex(double Sx[], double Sy[], double Hx[], double Hy[], int n)
{
	// Check if we have enough points to form a convex hull
	// A convex hull requires at least 3 points
	if (n < 3)		// set >= three points ?
		return 0;
	
	// STEP 1: Find the leftmost point
	// The leftmost point is guaranteed to be on the convex hull
	int l = 0;  // Index of leftmost point
	int i;      // Loop counter
	for (i=1; i<n; i++)
		if (Sx[i] < Sx[l])  // If current point is more to the left
			l = i;          // Update leftmost point index
	
	// STEP 2: Gift wrapping algorithm main loop
	int k = 0;  // Counter for hull points found
	int p = l;  // Current point on hull (start with leftmost)
	
	while (true)
	{
		// Start with next point in array as candidate for next hull point
		int q = (p + 1) % n;  // Next point (wrap around if at end)
		
		// STEP 3: Find the point that makes most counterclockwise turn
		// Check all points to find the one that makes the most counterclockwise
		// turn from current hull point p
		for (i=0; i<n; i++)
		{
			// Check if point i makes a more counterclockwise turn than current candidate q
			// rotation < 0 means clockwise turn, so point i is better choice than q
			if (rotation(Sx[p], Sy[p], Sx[i], Sy[i], Sx[q], Sy[q]) < 0)
				q = i;	// Update candidate to point i (if counterclockwise)
		}
		
		// STEP 4: Add the found point to convex hull
		Hx[k] = Sx[q];  // Store x-coordinate of hull point
		Hy[k] = Sy[q];  // Store y-coordinate of hull point
		k++;            // Increment hull point counter

		// Safety check to prevent infinite loop
		// If we've found more hull points than input points, something is wrong
		if (k > n-1)
			break;

		// STEP 5: Check if we've completed the hull
		// If we've returned to the starting point (leftmost), we're done
		if (q == l)
			break;
		
		// Move to the next hull point for next iteration
		p = q;
	}

	// Return the number of points in the convex hull
	return k;
}

/*
 * MAIN FUNCTION - Program Entry Point and Test Case
 * 
 * This function demonstrates the convex hull algorithm with a simple test case.
 * It defines 4 points and finds their convex hull, then prints the result.
 * 
 * Test Points:
 *   Point 1: (1, 1)
 *   Point 2: (1, 3) 
 *   Point 3: (2, 2)
 *   Point 4: (3, 2)
 * 
 * Expected convex hull: Points (1,1), (1,3), (3,2) 
 * Point (2,2) should be inside the hull and not included.
 */
int main(void)
{
	// Define test points - 4 points in 2D space
	// These arrays store the x and y coordinates respectively
	double Sx[] = {1, 1, 2, 3};  // x-coordinates: [1, 1, 2, 3]
	double Sy[] = {1, 3, 2, 2};  // y-coordinates: [1, 3, 2, 2]
	                             // Points: (1,1), (1,3), (2,2), (3,2)

	int n = 4;  // Number of input points

	// Allocate arrays to store convex hull points
	// Size n+1 to be safe (hull can't have more than n points)
	double Hx[n+1];  // x-coordinates of hull points
	double Hy[n+1];  // y-coordinates of hull points

	// Print header for output
	printf("The convex hull is:\n");

	// Call convex hull function and get number of hull points
	n = convex(Sx, Sy, Hx, Hy, n);

	// Print all points in the convex hull
	int i;  // Loop counter
	for (i=0; i<n; i++)
		printf("(%.1f, %.1f)\n", Hx[i], Hy[i]);  // Print each hull point

    // Return success code to operating system
    return EXIT_SUCCESS;
}

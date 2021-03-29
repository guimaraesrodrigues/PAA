// A divide and conquer program in C/C++ to find the smallest distance from a
// given set of points.

#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>

// A structure to represent a Point in 2D plane
typedef struct Point
{
	double x, y;
} Point;

/* Following two functions are needed for library function qsort().
Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */

// Needed to sort array of points according to X coordinate
int compareX(const void* a, const void* b)
{
	Point *p1 = (Point *)a, *p2 = (Point *)b;
	return (p1->x - p2->x);
}
// Needed to sort array of points according to Y coordinate
int compareY(const void* a, const void* b)
{
	Point *p1 = (Point *)a, *p2 = (Point *)b;
	return (p1->y - p2->y);
}

// A utility function to find the distance between two points
float dist(Point p1, Point p2)
{
	return sqrt( (p1.x - p2.x)*(p1.x - p2.x) +
				(p1.y - p2.y)*(p1.y - p2.y)
			);
}

// A Brute Force method to return the smallest distance between two points
// in P[] of size n
float bruteForce(Point P[], int n)
{
	float min = FLT_MAX;
	for (int i = 0; i < n; ++i)
		for (int j = i+1; j < n; ++j)
			if (dist(P[i], P[j]) < min)
				min = dist(P[i], P[j]);
	return min;
}

// A utility function to find a minimum of two float values
float min(float x, float y)
{
	return (x < y)? x : y;
}


// A utility function to find the distance between the closest points of
// strip of a given size. All points in strip[] are sorted according to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
float stripClosest(Point strip[], int size, float d)
{
	float min = d; // Initialize the minimum distance as d

	qsort(strip, size, sizeof(Point), compareY);

	// Pick all points one by one and try the next points till the difference
	// between y coordinates is smaller than d.
	// This is a proven fact that this loop runs at most 6 times
	for (int i = 0; i < size; ++i)
		for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j)
			if (dist(strip[i],strip[j]) < min)
				min = dist(strip[i], strip[j]);

	return min;
}

// A recursive function to find the smallest distance. The array P contains
// all points sorted according to x coordinate
float closestUtil(Point P[], int n)
{
	// If there are 2 or 3 points, then use brute force
	if (n <= 3)
		return bruteForce(P, n);

	// Find the middle point
	int mid = n/2;
	Point midPoint = P[mid];

	// Consider the vertical line passing through the middle point
	// calculate the smallest distance dl on left of middle point and
	// dr on right side
	float dl = closestUtil(P, mid);
	float dr = closestUtil(P + mid, n-mid);

	// Find the smaller of two distances
	float d = min(dl, dr);

	// Build an array strip[] that contains points close (closer than d)
	// to the line passing through the middle point
	Point strip[n];
	int j = 0;
	for (int i = 0; i < n; i++)
		// if (abs(P[i].x - midPoint.x) < d)
        if (P[i].x - midPoint.x < d)
			strip[j] = P[i], j++;

	// Find the closest points in strip. Return the minimum of d and closest
	// distance is strip[]
	return min(d, stripClosest(strip, j, d) );
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
float closest(Point P[], int n)
{
	qsort(P, n, sizeof(Point), compareX);

	// Use recursive function closestUtil() to find the smallest distance
	return closestUtil(P, n);
}

Point string_to_points(char str[]) {
    Point p;

    sscanf(str, "%lf", &p.x);

    // printf("%lf", p.x);

    // while(token != NULL) {
    //   sscanf(token, "%lf", &aux);
    //   p.x = aux;
    //   sscanf(token, "%lf", &aux);
    //   p.y = aux;
    //   token = strtok(NULL, " ");
    // }

    return p;
}


int main(int argc, char *argv[]) {
    FILE *fp = NULL;
    int n_lines = 0;
    int n_pairs;
    char line_buffer[80];
    char first_line[10];

    /* Check for cmd arguments */
    if (argc != 2) {
        printf ("ERROR: you must specify file name!\n");
        return 1;
    }

    /* Open file */
    fp = fopen(argv[1], "r");
    if (!fp) {
        perror ("File open error!\n");
        return 1;
    }

    fscanf(fp, "%s", first_line);
    sscanf(first_line, "%d", &n_pairs);

    // printf("%d", n_pairs);
    Point points[n_pairs];

    // P[0].x = 2;
    // P[0].y = 3;

    while (!feof (fp)) {
        if (fgets(line_buffer, sizeof(line_buffer), fp)) {
            points[n_lines] = string_to_points(line_buffer);
            n_lines++;
        }
    }

    printf("%lf %lf", points[2].x, points[2].y);

    /* Done */
    fclose (fp);

	// int n = sizeof(P) / sizeof(P[0]);
	// printf("The smallest distance is %f ", closest(P, n));
	return 0;
}
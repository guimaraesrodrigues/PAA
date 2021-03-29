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
float bruteForce(Point P[], int n, Point minPoints[])
{
	float min = FLT_MAX;

	for (int i = 0; i < n; ++i)
		for (int j = i+1; j < n; ++j)
			if (dist(P[i], P[j]) < min){
				min = dist(P[i], P[j]);
				minPoints[0] = P[i];
				minPoints[1] = P[j];
			}
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
float stripClosest(Point strip[], int size, float d, Point minPoint[])
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
float closestUtil(Point P[], int n, Point minPoint[])
{
	// If there are 2 or 3 points, then use brute force
	if (n <= 3)
		return bruteForce(P, n, minPoint);

	// Find the middle point
	int mid = n/2;
	Point midPoint = P[mid];

	// Consider the vertical line passing through the middle point
	// calculate the smallest distance dl on left of middle point and
	// dr on right side
	float dl = closestUtil(P, mid, minPoint);
	float dr = closestUtil(P + mid, n-mid, minPoint);

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
	return min(d, stripClosest(strip, j, d, minPoint) );
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
float closest(Point P[], int n, Point minPoint[])
{
	qsort(P, n, sizeof(Point), compareX);

	// Use recursive function closestUtil() to find the smallest distance
	return closestUtil(P, n, minPoint);
}

Point create_points_list(FILE *file, Point* points) {
    Point p;

	int ret = 0;
	int n_lines = 0;
	char line_buffer[50]; //buffer to be used for each line

	while(fgets(line_buffer, sizeof line_buffer, file)) {
		ret = sscanf(line_buffer, "%lf %lf", &p.x, &p.y);
		if(ret < 1)
			continue;
		points[n_lines].x = p.x;
		points[n_lines].y = p.y;
		n_lines++;
	}

    return p;
}


int main(int argc, char *argv[]) {
    FILE *file = NULL;

    /* Check for cmd arguments */
    if (argc != 2) {
        printf ("ERROR: you must specify file name!\n");
        return 1;
    }

    /* Open file */
    file = fopen(argv[1], "r");
    if (!file) {
        perror ("File open error!\n");
        return 1;
    }

    char first_line[10];
    int n_pairs;
    fscanf(file, "%s", first_line);
    sscanf(first_line, "%d", &n_pairs);//convert string to int

    Point points[n_pairs];

	create_points_list(file, points);// Create list of pairs based on input.txt
	
	for (int i = 0; i < n_pairs; i++)
		printf("%lf %lf \n", points[i].x, points[i].y);

    /* Close file */
    fclose (file);

	Point minPoints[2]; 
	int n = sizeof(points) / sizeof(points[0]);
	printf("The smallest distance (optimized) is %lf \n", closest(points, n,minPoints));
	//printf("Closest points (optimized): \n");
	//printf("%lf %lf \n", minPoints[0].x, minPoints[0].y);
	//printf("%lf %lf \n", minPoints[1].x, minPoints[1].y);

	printf("The smallest distance (brute force) is %lf \n", bruteForce(points, n, minPoints));
	printf("Closest points (brute force): \n");
	printf("%lf %lf \n", minPoints[0].x, minPoints[0].y);
	printf("%lf %lf \n", minPoints[1].x, minPoints[1].y);
	return 0;
}
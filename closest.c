#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>

// Struct para representar o ponto
typedef struct Ponto {
	double x, y;
} Ponto;

void merge(int arr[], int l, int m, int r) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1+ j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; 
	j = 0; 
	k = l;
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			arr[k] = L[i];
			i++;
		}
		else {
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2) {
		arr[k] = R[j];
		j++;
		k++;
	}
}

void mergeSort(int arr[], int l, int r) {
	if (l < r) {
		int m = l+(r-l)/2;

		// Sort first and second halves
		mergeSort(arr, l, m);
		mergeSort(arr, m+1, r);

		merge(arr, l, m, r);
	}
}


/* Following two functions are needed for library function qsort().
Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */

// Needed to sort array of points according to X coordinate
int compareX(const void* a, const void* b) {
	Ponto *p1 = (Ponto *)a, *p2 = (Ponto *)b;
	return (p1->x - p2->x);
}
// Needed to sort array of points according to Y coordinate
int compareY(const void* a, const void* b) {
	Ponto *p1 = (Ponto *)a, *p2 = (Ponto *)b;
	return (p1->y - p2->y);
}

// A utility function to find the distance between two points
float dist(Ponto p1, Ponto p2) {
	return sqrt( (p1.x - p2.x)*(p1.x - p2.x) +
				(p1.y - p2.y)*(p1.y - p2.y)
			);
}

// A Brute Force method to return the smallest distance between two points
// in P[] of size n
float bruteForce(Ponto P[], int n, Ponto minPoints[]) {
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
float min(float x, float y) {
	return (x < y) ? x : y;
}


// A utility function to find the distance between the closest points of
// strip of a given size. All points in strip[] are sorted according to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
float stripClosest(Ponto strip[], int size, float d, Ponto minPoint[]) {
	float min = d; // Initialize the minimum distance as d

	// mergeSort(strip, 0, size - 1);

	qsort(strip, size, sizeof(Ponto), compareY);

	// Pick all points one by one and try the next points till the difference
	// between y coordinates is smaller than d.
	// This is a proven fact that this loop runs at most 6 times
	for (int i = 0; i < size; ++i)
		for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min; ++j)
			if (dist(strip[i],strip[j]) < min)
				min = dist(strip[i], strip[j]);

	return min;
}

// A recursive function to find the smallest distance. The array P contains
// all points sorted according to x coordinate
float closestUtil(Ponto P[], int n, Ponto minPoint[]) {
	// If there are 2 or 3 points, then use brute force
	if (n <= 3){
		float min = FLT_MAX;

		for (int i = 0; i < n; ++i)
			for (int j = i+1; j < n; ++j)
				if (dist(P[i], P[j]) < min)
					min = dist(P[i], P[j]);
		
		return min;
	}

	// Find the middle point
	int mid = n/2;
	Ponto midPoint = P[mid];

	// Consider the vertical line passing through the middle point
	// calculate the smallest distance dl on left of middle point and
	// dr on right side
	float dl = closestUtil(P, mid, minPoint);
	float dr = closestUtil(P + mid, n - mid, minPoint);

	// Find the smaller of two distances
	float d = min(dl, dr);

	// Build an array strip[] that contains points close (closer than d)
	// to the line passing through the middle point
	Ponto strip[n];
	int j = 0;
	for (int i = 0; i < n; i++)
		// if (abs(P[i].x - midPoint.x) < d)
        if ((P[i].x - midPoint.x) < d)
			strip[j] = P[i], j++;

	// Find the closest points in strip. Return the minimum of d and closest
	// distance is strip[]
	return min(d, stripClosest(strip, j, d, minPoint) );
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
float closest(Ponto P[], int n, Ponto minPoint[]) {
	
	// mergeSort(P, n, sizeof(P) - 1);
	qsort(P, n, sizeof(Ponto), compareX);

	// Use recursive function closestUtil() to find the smallest distance
	return closestUtil(P, n, minPoint);
}

// Cria uma lista pontos P baseado no arquivo passado como parâmetro
Ponto createPointsList(FILE *file, Ponto* points) {
    Ponto p;

	int ret = 0;
	int n_lines = 0;
	char line_buffer[50]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e adiciona o valor no array points
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


void test() {
	int arr[] = { 12, 11, 13, 5, 6, 7 };
    int arr_size = sizeof(arr) / sizeof(arr[0]);
 
    mergeSort(arr, 0, arr_size - 1);
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
    fscanf(file, "%s", first_line); // Le a primeira linha do arquivo para saber quantos pares foram gerados
    sscanf(first_line, "%d", &n_pairs);//converte string para int

    Ponto points[n_pairs];

	createPointsList(file, points);// Cria lista de pares com base no arquivo input.txt

    /* Close file */
    fclose (file);

	Ponto minPoints[2]; 
	int n = sizeof(points) / sizeof(points[0]);

	printf("The smallest distance (optimized) is %lf \n", closest(points, n, minPoints));

	//printf("Closest points (optimized): \n");
	//printf("%lf %lf \n", minPoints[0].x, minPoints[0].y);
	//printf("%lf %lf \n", minPoints[1].x, minPoints[1].y);

	printf("The smallest distance (brute force) is %lf \n", bruteForce(points, n, minPoints));
	printf("Closest points (brute force): \n");
	printf("%lf %lf \n", minPoints[0].x, minPoints[0].y);
	printf("%lf %lf \n", minPoints[1].x, minPoints[1].y);
	return 0;
}
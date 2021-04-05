#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Struct para representar o ponto
typedef struct Ponto {
	double x, y;
} Ponto;

void mergeX(Ponto arr[], int left, int mid, int right) {
	int i, j, k;
	int n1 = mid - left + 1;
	int n2 = right - mid;

	Ponto L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[left + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[mid + 1+ j];

	/* Merge the temp arrays back into arr[left..right]*/
	i = 0; 
	j = 0; 
	k = left;
	while (i < n1 && j < n2) {
		if (L[i].x <= R[j].x) {
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

void mergeSortX(Ponto arr[], int left, int right) {
	if (left < right) {
		int mid = left+(right-left)/2;

		// Sort first and second halves
		mergeSortX(arr, left, mid);
		mergeSortX(arr, mid+1, right);

		mergeX(arr, left, mid, right);
	}
}

void mergeY(Ponto arr[], int left, int mid, int right) {
	int i, j, k;
	int n1 = mid - left + 1;
	int n2 = right - mid;

	Ponto L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[left + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[mid + 1+ j];

	/* Merge the temp arrays back into arr[left..right]*/
	i = 0; 
	j = 0; 
	k = left;
	while (i < n1 && j < n2) {
		if (L[i].y <= R[j].y) {
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

void mergeSortY(Ponto arr[], int left, int right) {
	if (left < right) {
		int mid = left+(right-left)/2;

		// Sort first and second halves
		mergeSortY(arr, left, mid);
		mergeSortY(arr, mid+1, right);

		mergeY(arr, left, mid, right);
	}
}

// A utility function to find the distance between two points
double dist(Ponto p1, Ponto p2) {
	return sqrt( (p1.x - p2.x)*(p1.x - p2.x) +
				(p1.y - p2.y)*(p1.y - p2.y)
			);
}

// A Brute Force method to return the smallest distance between two points
// in P[] of size n
double bruteForce(Ponto P[], int n_pairs, Ponto minPoints[]) {
	double min = FLT_MAX;

	for (int i = 0; i < n_pairs; ++i)
		for (int j = i+1; j < n_pairs; ++j)
			if (dist(P[i], P[j]) < min){
				min = dist(P[i], P[j]);
				minPoints[0] = P[i];
				minPoints[1] = P[j];
			}
	return min;
}

// A utility function to find a minimum of two double values
double min(double x, double y) {
	return (x < y) ? x : y;
}

// A utility function to find the distance between the closest points of
// strip of a given size. All points in strip[] are sorted according to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
double stripClosest(Ponto strip[], int size, double d, Ponto closestPoints[]) {
	double min = d; // Initialize the minimum distance as d

	mergeSortY(strip, 0, size - 1);

	// Pick all points one by one and try the next points till the difference
	// between y coordinates is smaller than d.
	// This is a proven fact that this loop runs at most 6 times
	for (int i = 0; i < size; ++i)
		for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min; ++j)
			if (dist(strip[i],strip[j]) < min) {
				min = dist(strip[i], strip[j]);
				closestPoints[0] = strip[i];
				closestPoints[1] = strip[j];
			}
	return min;
}

// A recursive function to find the smallest distance. The array pontos contains
// all points sorted according to x coordinate
double closestUtil(Ponto pontos[], int n_pairs, Ponto closestPoints[]) {
	// If there are 2 or 3 points, then use brute force
	if (n_pairs <= 3){
		double min = FLT_MAX;
		for (int i = 0; i < n_pairs; ++i)
			for (int j = i+1; j < n_pairs; ++j)
				if (dist(pontos[i], pontos[j]) < min) {
					min = dist(pontos[i], pontos[j]);
					// closestPoints[0] = pontos[i];
					// closestPoints[1] = pontos[j];
				}
		
		return min;
	}

	// Find the middle point
	int mid = n_pairs/2;
	Ponto midPoint = pontos[mid];

	// Consider the vertical line passing through the middle point
	// calculate the smallest distance dl on left of middle point and
	// dr on right side
	double dl = closestUtil(pontos, mid, closestPoints);
	double dr = closestUtil(pontos + mid, n_pairs - mid, closestPoints);

	// Find the smaller of two distances
	double d = min(dl, dr);

	// Build an array strip[] that contains points close (closer than d)
	// to the line passing through the middle point
	Ponto strip[n_pairs];
	int j = 0;
	for (int i = 0; i < n_pairs; i++)
        if ((pontos[i].x - midPoint.x) < d)
			strip[j] = pontos[i], j++;

	// Find the closest points in strip. Return the minimum of d and closest
	// distance is strip[]
	return min(d, stripClosest(strip, j, d, closestPoints));
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
double closest(Ponto pontos[], int n_pairs, Ponto closestPoints[]) {
	
	mergeSortX(pontos, 0, n_pairs - 1);

	// Use recursive function closestUtil() to find the smallest distance
	return closestUtil(pontos, n_pairs, closestPoints);
}

// Cria uma lista pontos P baseado no arquivo passado como parâmetro
void createPointsList(FILE *file, Ponto* pontos) {
    Ponto p;

	int ret = 0;
	int n_lines = 0;
	char line_buffer[100]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e adiciona o valor no array pontos
	while(fgets(line_buffer, sizeof line_buffer, file)) {
		ret = sscanf(line_buffer, "%lf %lf", &p.x, &p.y);
		if(ret < 1)
			continue;
		pontos[n_lines].x = p.x;
		pontos[n_lines].y = p.y;
		n_lines++;
		printf("%lf %lf\n", p.x, p.y);
	}
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

    Ponto pontos[n_pairs];

	createPointsList(file, pontos);// Cria lista de pares com base no arquivo input.txt

    fclose (file);

	Ponto minPoints[2];//Armazena os pontos mais próximos

	clock_t beginOpt = clock();
	printf("The smallest distance (optimized) is %lf \n", closest(pontos, n_pairs, minPoints));
	clock_t endOpt = clock();
	
	printf("Time spent: %lf \n", (double)(endOpt - beginOpt) / CLOCKS_PER_SEC);
	printf("Closest points (optimized): \n");
	printf("%lf %lf \n", minPoints[0].x, minPoints[0].y);
	printf("%lf %lf \n", minPoints[1].x, minPoints[1].y);

	clock_t beginBF = clock();
	printf("The smallest distance (brute force) is %lf \n", bruteForce(pontos, n_pairs, minPoints));
	clock_t endBF = clock();

	printf("Time spent: %lf \n", (double)(endBF - beginBF) / CLOCKS_PER_SEC);
	printf("Closest points (brute force): \n");
	printf("%lf %lf \n", minPoints[0].x, minPoints[0].y);
	printf("%lf %lf \n", minPoints[1].x, minPoints[1].y);
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_VALUE 10000.000000

// Struct para representar o ponto
typedef struct Ponto {
	double x, y;
} Ponto;

void merge(Ponto arr[], int esq, int mid, int dir, char eixo) {
	int i, j, k;
	int n1 = mid - esq + 1;
	int n2 = dir - mid;

	Ponto L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[esq + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[mid + 1+ j];

	/* Merge the temp arrays back into arr[esq..dir]*/
	i = 0; 
	j = 0; 
	k = esq;
	while (i < n1 && j < n2) {
		if (eixo == 'y') {
			if (L[i].y <= R[j].y) {
				arr[k] = L[i];
				i++;
			} else {
				arr[k] = R[j];
				j++;
			}
		} else {
			if (L[i].x <= R[j].x) {
				arr[k] = L[i];
				i++;
			} else {
				arr[k] = R[j];
				j++;
			}
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

void mergeSort(Ponto arr[], int esq, int dir, char eixo) {
	if (esq < dir) {
		int mid = esq+(dir-esq)/2;

		// Ordena a primeira e a segunda metade
		mergeSort(arr, esq, mid, eixo);
		mergeSort(arr, mid+1, dir, eixo);

		merge(arr, esq, mid, dir, eixo);
	}
}

// A utility function to find a minimum of two double values
double min(double x, double y) {
	return (x < y) ? x : y;
}

// Calcula a distância euclidiana entre p1 e p2
double dist(Ponto p1, Ponto p2) {
	return sqrt( (p1.x - p2.x)*(p1.x - p2.x) +
				(p1.y - p2.y)*(p1.y - p2.y)
			);
}

// A Brute Force method to return the smallest distance between two points
// in P[] of size n
double bruteForce(Ponto P[], int n_pairs, Ponto *p1, Ponto *p2) {
	double min = MAX_VALUE;

	for (int i = 0; i < n_pairs; ++i)
		for (int j = i+1; j < n_pairs; ++j)
			if (dist(P[i], P[j]) < min) {
				min = dist(P[i], P[j]);
				p1->x = P[i].x, p1->y = P[i].y;
				p2->x = P[j].x, p2->y = P[j].y;
			}
	return min;
}

// A utility function to find the distance between the closest points of
// strip of a given size. All points in strip[] are sorted according to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times

// find the distance beween the closest points of
// strip of given size. All points in strip[] are sorted by y. 
double stripPoints(Ponto strip[], int tam, double d, Ponto *p1, Ponto *p2)
{
    double min = d;  // Initialize the minimum distance as d
 
    mergeSort(strip, 0, tam - 1, 'y');
 
    // Pick all points one by one and try the next points 
    // till the difference between y's is smaller than d.
    for (int i = 0; i < tam; ++i)
        for (int j = i+1; j < tam && (strip[j].y - strip[i].y) < min; ++j)
            if (dist(strip[i],strip[j]) < min) {
                min = dist(strip[i], strip[j]);
		p1->x = strip[i].x, p1->y = strip[i].y;
		p2->x = strip[j].x, p2->y = strip[j].y;
	    }
 
    return min;
}

// find the smallest distance recursively. 
// All point in array P are sorted by x
double closestUtil(Ponto P[], Ponto strip[], int n, Ponto *p1, Ponto *p2)
{
    Ponto *ptemp1 = malloc(sizeof(Ponto));
	Ponto *ptemp2 = malloc(sizeof(Ponto));
	Ponto *ptemp3 = malloc(sizeof(Ponto));
	Ponto *ptemp4 = malloc(sizeof(Ponto));

    // use brute force when there are not enough points
    if (n <= 3)
        return bruteForce(P, n, ptemp1, ptemp2);
 
    // mid point
    int mid = n/2;
    Ponto midPoint = P[mid];
 
    // calculate the smallest distance 
    // dl: left of mid point 
    // dr: right side of the mid point
    double dl = closestUtil(P, strip, mid, ptemp1, ptemp2);
    double dr = closestUtil(P + mid, strip, n-mid, ptemp3, ptemp4);
 
    // assign the pair that has smaller distance
    if(dl < dr) {
		p1->x = ptemp1->x; p1->y = ptemp1->y;
		p2->x = ptemp2->x; p2->y = ptemp2->y;
    }
    else {
		p1->x = ptemp3->x; p1->y = ptemp3->y;
		p2->x = ptemp4->x; p2->y = ptemp4->y;
    }

    double dmin = min(dl, dr);
 
    int j = 0;
    for (int i = 0; i < n; i++)
        if (abs(P[i].x - midPoint.x) < dmin)
            strip[j++] = P[i];
 
    double dmin_strip = stripPoints(strip, j, dmin, ptemp1, ptemp2);
    double final_min = dmin;
    if(dmin_strip < dmin) {
		p1->x = ptemp1->x; p1->y = ptemp1->y;
		p2->x = ptemp2->x; p2->y = ptemp2->y;
		final_min = dmin_strip;
    }
    return final_min;
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
double closest(Ponto pontos[], Ponto strip[], int n_pairs, Ponto *p1, Ponto *p2) {
	
	mergeSort(pontos, 0, n_pairs - 1, 'x');

	// Use recursive function closestUtil() to find the smallest distance
	return closestUtil(pontos, strip, n_pairs, p1, p2);
}

// Cria uma lista pontos P baseado no arquivo passado como parâmetro
void createPointsList(FILE *file, Ponto* pontos) {
    Ponto p;

	int ret = 0;
	int n_lines = 0;
	char line_buffer[100]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e insere o valor no array pontos
	while(fgets(line_buffer, sizeof(line_buffer), file)) {
		ret = sscanf(line_buffer, "%lf %lf", &p.x, &p.y);
		if(ret < 1)
			continue;
		pontos[n_lines].x = p.x;
		pontos[n_lines].y = p.y;
		n_lines++;
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
    int n_pontos;
    fscanf(file, "%s", first_line); // Le a primeira linha do arquivo para saber quantos pares foram gerados
    sscanf(first_line, "%d", &n_pontos);//converte string para int

    Ponto pontos[n_pontos];

	createPointsList(file, pontos);// Cria lista de pares com base no arquivo input.txt

    fclose (file);

	//Armazena os pontos mais próximos
	Ponto *p1 = malloc(sizeof(Ponto));
	Ponto *p2 = malloc(sizeof(Ponto));

	p1->x = MAX_VALUE;
	p1->y = MAX_VALUE;
	p2->x = MAX_VALUE;
	p2->y = MAX_VALUE;

	// array to store points in a strip
    Ponto strip[n_pontos];

	clock_t beginOpt = clock();
	printf("The smallest distance (optimized) is %lf \n", closest(pontos, strip, n_pontos, p1, p2));
	clock_t endOpt = clock();
	
	printf("Time spent: %lf \n", (double)(endOpt - beginOpt) / CLOCKS_PER_SEC);
	printf("Closest points (optimized): \n");
	printf("%lf %lf \n", p1->x, p1->y);
	printf("%lf %lf \n", p2->x, p2->y);

	clock_t beginBF = clock();
	printf("The smallest distance (brute force) is %lf \n", bruteForce(pontos, n_pontos,  p1, p2));
	clock_t endBF = clock();

	printf("Time spent: %lf \n", (double)(endBF - beginBF) / CLOCKS_PER_SEC);
	printf("Closest points (brute force): \n");
	printf("%lf %lf \n", p1->x, p1->y);
	printf("%lf %lf \n", p2->x, p2->y);
	return 0;
}
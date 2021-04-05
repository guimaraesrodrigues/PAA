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

	/* Copia dados para arrays temporarios L[] e R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[esq + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[mid + 1+ j];

	/* Faz merge dos arrays temporarios em arr[esq..dir]*/
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

	/* Copia os elementos restanted de  L[], se existirem */
	while (i < n1) {
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copia os elementos restanted de  R[], se existirem */
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

// Função para encontrar o minimo entre dois valores double
double min(double x, double y) {
	return (x < y) ? x : y;
}

// Calcula a distância euclidiana entre p1 e p2
double dist(Ponto p1, Ponto p2) {
	return sqrt( (p1.x - p2.x)*(p1.x - p2.x) +
				(p1.y - p2.y)*(p1.y - p2.y)
			);
}

// Método força bruta para retornar a menor distância entre pontos
// em P[] com tamanho n_pairs
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

// Função para encontrar a distância entre os pontos mais próximos de uma strip
// Todos os pontos da strip estão ordenados de acordo com a coordenada y. 
double stripPoints(Ponto strip[], int tam, double d, Ponto *p1, Ponto *p2)
{
    double min = d;  // Inicializa a distância minima como d
 
    mergeSort(strip, 0, tam - 1, 'y');
 
    for (int i = 0; i < tam; ++i)
        for (int j = i+1; j < tam && (strip[j].y - strip[i].y) < min; ++j)
            if (dist(strip[i],strip[j]) < min) {
                min = dist(strip[i], strip[j]);
				p1->x = strip[i].x, p1->y = strip[i].y;
				p2->x = strip[j].x, p2->y = strip[j].y;
	    		}
 
    return min;
}

// Encontra a menor distância recursivamente
double closestUtil(Ponto P[], Ponto strip[], int n, Ponto *p1, Ponto *p2)
{
    Ponto *ptemp1 = malloc(sizeof(Ponto));
	Ponto *ptemp2 = malloc(sizeof(Ponto));
	Ponto *ptemp3 = malloc(sizeof(Ponto));
	Ponto *ptemp4 = malloc(sizeof(Ponto));

    // Usa força bruta quando não há pontos suficientes no array
    if (n <= 3)
        return bruteForce(P, n, ptemp1, ptemp2);
 
    // Ponto médio
    int mid = n/2;
    Ponto midPoint = P[mid];
 
    // Calcula a menor distância
    // dl: à esquerda do ponto médio
    // dr: à direita do ponto médio
    double dl = closestUtil(P, strip, mid, ptemp1, ptemp2);
    double dr = closestUtil(P + mid, strip, n-mid, ptemp3, ptemp4);
 
    // Escolhe o par com a menor distância
    if(dr <= dl) {
		p1->x = ptemp3->x; p1->y = ptemp3->y;
		p2->x = ptemp4->x; p2->y = ptemp4->y;
    } else {
    		p1->x = ptemp1->x; p1->y = ptemp1->y;
		p2->x = ptemp2->x; p2->y = ptemp2->y;
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

// Função principal, chama o método closestUtil para encontrar a menor distância após a ordenação dos pontos
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

    /* Checa argumentos da linha de comando */
    if (argc != 2) {
        printf ("ERROR: you must specify file name!\n");
        return 1;
    }

    /* Abre o arquivo */
    file = fopen(argv[1], "r");
    if (!file) {
        perror ("File open error!\n");
        return 1;
    }
	
    char first_line[20];
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

	// Array para armazenar pontos
    Ponto strip[n_pontos];

	double fbt, fbd, fbx1, fby1, fbx2, fby2, dct, dcd, dcx1, dcy1, dcx2, dcy2;

	clock_t beginBF = clock();

	//menor distância força bruta
	fbd = bruteForce(pontos, n_pontos,  p1, p2);
	clock_t endBF = clock();

	//tempo força bruta
	fbt = (double)(endBF - beginBF) / CLOCKS_PER_SEC;

	//pontos mais próximos força bruta
	fbx1 = p1->x;
	fby1 = p1->y;
	fbx2 = p2->x;
	fby2 = p2->y;
	
	clock_t beginOpt = clock();

	//menor distância otimizado
	dcd = closest(pontos, strip, n_pontos, p1, p2);
	clock_t endOpt = clock();

	//tempo otimizado
	dct = (double)(endOpt - beginOpt) / CLOCKS_PER_SEC;

	//pontos mais próximos otimizado
	dcx1 = p1->x;
	dcy1 = p1->y;
	dcx2 = p2->x;
	dcy2 = p2->y;

	printf("%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", fbt, fbd, fbx1, fby1, fbx2, fby2, dct, dcd, dcx1, dcy1, dcx2, dcy2);
	
	return 0;
}

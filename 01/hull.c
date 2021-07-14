#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Struct para representar o ponto
typedef struct Ponto {
	float x, y;
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

float calcDist(Ponto p1, Ponto p2, Ponto p3) {
    float numerador = (
            (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x) *
            (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x)
        );
    float denominador = ((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    return sqrt(numerador)/sqrt(denominador);
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
		ret = sscanf(line_buffer, "%f %f", &p.x, &p.y);
		if(ret < 1)
			continue;
		pontos[n_lines].x = p.x;
		pontos[n_lines].y = p.y;
		n_lines++;
	}
}

void quickHull(Ponto* pontos) {

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

    mergeSort(pontos, 0, n_pontos - 1, 'x');// Ordena lista com base na cordenada x de cada ponto

    quickHull(pontos);

    for (int i = 0; i < n_pontos; i++) {
        printf("%f, %f\n", pontos[i].x, pontos[i].y);
    }
}
/**
 * Trabalho 01
 * 
 * Autores:
 * Eduardo Junior - 
 * Rodrigo Guimarães - 1441990
 * 
 * **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Struct para representar o ponto
typedef struct Ponto {
	float x, y;
} Ponto;

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

void quickHull(Ponto* pontos, int n_pontos) {
    Ponto menor_coord = pontos[0];
    Ponto maior_coord = pontos[0];

    for (int i = 1; i < n_pontos; i++) {
        if(pontos[i].x < menor_coord.x)
            menor_coord = pontos[i];
        else if (pontos[i].x == menor_coord.x && pontos[i].y > menor_coord.y)
            menor_coord = pontos[i];

        if(pontos[i].x > maior_coord.x)
            maior_coord = pontos[i];
        else if (pontos[i].x == maior_coord.x && pontos[i].y < maior_coord.y)
            maior_coord = pontos[i];
    }

    printf("Menor x: (%f, %f)\n", menor_coord.x, menor_coord.y);
    printf("Maior x: (%f, %f)\n", maior_coord.x, maior_coord.y);

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

    quickHull(pontos, n_pontos);

    // for (int i = 0; i < n_pontos; i++) {
    //     printf("%f, %f\n", pontos[i].x, pontos[i].y);
    // }
}
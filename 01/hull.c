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
#include "structs.h"
#include "lista.h"

/**
 * Calcula area do triangulo formado por p1, p2, p3 
 * para definir a posicao de p3 em relacao a reta p1->p2
 * 
 **/
float calcArea(Ponto p1, Ponto p2, Ponto p3) {
    return ((p2.x - p1.x) * (p3.y - p1.y)) - 
           ((p2.y - p1.y) * (p3.x - p1.x));
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

void recursiveHull(Ponto* pontos, Lista* hull, int n_pontos) {
    if(n_pontos <= 2)
        return;

    // Pega posição de cada ponto em relação a reta 'hull'
    for(int i = 0; i < n_pontos; i++) {
        // Pontos coincidentes
        // if(calcArea(hull[0], hull[1], pontos[i]) == 0)


        // Pontos a esquerda
        // if(calcArea(hull[0], hull[1], pontos[i]) > 0)

        // Pontos a direita
        // if(calcArea(hull[0], hull[1], pontos[i]) < 0)
    }

    // Calcula distância ponto-reta
    for(int i = 0; i < n_pontos; i++) {
        int maior_esquerda = 0;
        int maior_direita = 0;

        // Maior distância à esquerda
        // if(calcDist(hull[0], hull[1])

        // Maior distância à direita
    }

    //
    

    // Chamada recursiva com a reta hull[0], ponto aux

}

void quickHull(Ponto* pontos, int n_pontos, Lista* hull) {
    int menor_coord_idx = 0;
    int maior_coord_idx = 0;

    for (int i = 1; i < n_pontos; i++) {
        if(pontos[i].x < pontos[menor_coord_idx].x)
            menor_coord_idx = i;
        else if (pontos[i].x == pontos[menor_coord_idx].x && pontos[i].y > pontos[menor_coord_idx].y)//acho que posso remover o if de cima e deixar || ao inves de && aqui
            menor_coord_idx = i;

        if(pontos[i].x > pontos[maior_coord_idx].x)
            maior_coord_idx = i;
        else if (pontos[i].x == pontos[maior_coord_idx].x && pontos[i].y < pontos[maior_coord_idx].y)//idem comentario linha 98
            maior_coord_idx = i;
    }

    inserir(hull, pontos[menor_coord_idx]);
    inserir(hull, pontos[maior_coord_idx]);
    
    //encontra o fecho convexo a esquerda da reta pq
    recursiveHull(pontos, hull, n_pontos);
    //encontra o fecho convexo a direita da reta pq
    recursiveHull(pontos, hull, n_pontos);
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

    Lista* hull = criar_lista();

    quickHull(pontos, n_pontos, hull);

    imprimir_lista(hull);

    // for (int i = 0; i < n_pontos; i++) {
    //     printf("%f, %f\n", pontos[i].x, pontos[i].y);
    // }
}
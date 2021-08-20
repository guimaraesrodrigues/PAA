/**
 * Trabalho 02
 * 
 * Autor:
 * Rodrigo Guimarães Rodrigues de Almeida - 1441990
 * 
 * **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "utils/structs.h"
#include "utils/giftwrapping.h"

/**
 * Grava ciclo no arquivo cycle.txt
 * 
 * */
void gravaCiclo(int n_pontos, Ponto vertices[n_pontos]) {
   
    FILE *fp = fopen("ciclo.txt", "w");
	
    if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar ciclo.txt.\n");
		return;
	}
    
    int n = 0;
    while (n < n_pontos)
	{
        // fprintf(fp, "%d %d\n", vertices[n].pai, vertices[n].id);
        n++;
	}
    fclose(fp);
}

// void tsp(int hull_size, Ponto *hull, int n_pontos, Ponto pontos[]) {
//     int dist_i_j = 0;
//     int dist_i_k = 0;
//     int dist_j_k = 0;

//     int tripla = dist_i_k + dist_j_k - dist_i_j;

    //substitui aresta Vi -> Vj por Vi -> Vk e Vk -> Vj

    //hull = hull com Vi -> Vk e Vk -> Vj
// }


int main(int argc, char *argv[]) {
    FILE *file = NULL; //variavel para input.txt
    char first_line[20]; //buffer para leitura da primeira linha do arquivo
    int n_pontos; //quantidade de coordendas (x,y) no arquivo

    /* Checa argumentos da linha de comando */
    if (argc != 2) {
        printf ("ERRO: especifique o nome do arquivo!\n");
        return 1;
    }

    /* Abre o arquivo */
    file = fopen(argv[1], "r");
    if (!file) {
        perror ("Erro ao abrir o arquivo!\n");
        return 1;
    }

    fscanf(file, "%s", first_line); // Le a primeira linha do arquivo para saber quantas coordenadas foram geradas
    sscanf(first_line, "%d", &n_pontos);//converte a primeira linha para int

    Ponto pontos[n_pontos]; //Lista para representar os pontos lidos no arquivo input.txt

    // Cria vetor de pares com base no arquivo input.txt
	createPointsList(file, pontos);

    //fecha arquivo input.txt
    fclose (file);

    //vetor para armazenar pontos do fecho convexo
    Ponto *hull = (Ponto*) malloc(n_pontos * sizeof(Ponto));
    
    //Define e retorna a quantidade de pontos que compoem o fecho convexo 
    int hull_size = convexHull(n_pontos, pontos, hull);

    // criaGrafo(file, n_pontos, grafo, pontos);// Cria grafo com base no arquivo input.txt
    
    clock_t begin = clock();
    // gravaCiclo(n_pontos, busca);

    clock_t end = clock();

    //tempo algoritmo
	double tempo = (double)(end - begin) / CLOCKS_PER_SEC;

    // printf("%lf %f", tempo, custo_ciclo);
}
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
#include <float.h>
#include "../helpers/structs.h"
#include "../helpers/lista.h"

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

void criaGrafo(FILE *file, int n_pontos, Ponto pontos[]) {
	int ret = 0;
	int i = 0;
	char line_buffer[100]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e insere o valor na matriz de ajacências
	while(fgets(line_buffer, sizeof(line_buffer), file)) {
		ret = sscanf(line_buffer, "%f %f", &pontos[i].x, &pontos[i].y);
        // pontos[i].pai = -1;
        // pontos[i].key = FLT_MAX;
        // pontos[i].id = i;
        
        if(ret < 1)
			continue;
    }
}


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
    
    clock_t begin = clock();

    // criaGrafo(file, n_pontos, grafo, pontos);// Cria grafo com base no arquivo input.txt
    fclose (file);

    // gravaCiclo(n_pontos, busca);

    clock_t end = clock();

    //tempo algoritmo
	double tempo = (double)(end - begin) / CLOCKS_PER_SEC;

    // printf("%lf %f", tempo, custo_ciclo);
}
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

Ponto* removePontos(int n_pontos, Ponto pontos[n_pontos], Ponto* fecho, int tam_fecho, Ponto* pontos_internos) {
  
    int n_fecho = tam_fecho -1;
    int k = 0;

    for (int i = 0; i < n_pontos; i++)
        for (int j = 0; j < n_fecho; j++) {
            if(pontos[i].x == fecho[j].x && pontos[i].y == fecho[j].y)
                break;
            else if (j == n_fecho - 1) {
                pontos_internos[k] = pontos[i];
                k++;
            }
        }

    return pontos_internos;
}

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

/* Calcula a distancia euclidiana */
int calcDist(Ponto p1, Ponto p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

void tsp(int tam_hull, Ponto *hull, int n_pontos, Ponto *pontos_internos, Ponto *ciclo) {

    int n_internos = n_pontos - tam_hull;

    //insere primeiro e segundo ponto no ciclo

    ciclo[0] = hull[0];
    ciclo[1] = hull[1];

    int dist_i_j = 0;
    int dist_i_k = 0;
    int dist_j_k = 0;

    int tripla = 0;
    int aux = 0;

    for(int i = 2; i < tam_hull; i ++) {

        dist_i_j = calcDist(ciclo[i-2], ciclo[i-1]);

        for (int k = 0; k < n_internos; k++) {

            if(!pontos_internos[k].visitado) {
                
                dist_i_k = calcDist(ciclo[i-2], pontos_internos[k]);
                dist_j_k = calcDist(ciclo[i-1], pontos_internos[k]);
                aux = dist_i_k + dist_j_k - dist_i_j;

                if (aux < tripla) {
                    aux = tripla;
                    pontos_internos[k].visitado = 1; ///???
                }
            }

        }
    }

   

    //substitui aresta Vi -> Vj por Vi -> Vk e Vk -> Vj

    //hull = hull com Vi -> Vk e Vk -> Vj
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

    Ponto pontos[n_pontos]; //Lista para representar os pontos lidos no arquivo input.txt

    // Cria vetor de pares com base no arquivo input.txt
	createPointsList(file, pontos);

    //fecha arquivo input.txt
    fclose (file);

    //vetor para armazenar pontos do fecho convexo
    Ponto *fecho = (Ponto*) malloc(n_pontos * sizeof(Ponto));
    
    //Define e retorna a quantidade de pontos que compoem o fecho convexo 
    int tam_fecho = convexHull(n_pontos, pontos, fecho);

    //vetor para armazenar pontos que sobraram apos calculo do fecho convexo
    Ponto *pontos_internos = (Ponto*) malloc((n_pontos - tam_fecho) * sizeof(Ponto));

   removePontos(n_pontos, pontos, fecho, tam_fecho, pontos_internos);

    for (int i = 0; i < n_pontos - tam_fecho; i++) {
        printf("%d, %d\n", pontos_internos[i].x, pontos_internos[i].y);
    }

    //vetor para armazenar o ciclo computado em tsp
    Ponto *ciclo = (Ponto*) malloc((n_pontos + 1) * sizeof(Ponto));

    clock_t begin = clock();

    tsp(tam_fecho, fecho, n_pontos, pontos_internos, ciclo);

    clock_t end = clock();

    //tempo algoritmo
	double tempo = (double)(end - begin) / CLOCKS_PER_SEC;

    // printf("%lf %f", tempo, custo_ciclo);
}
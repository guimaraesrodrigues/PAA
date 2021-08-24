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
                pontos_internos[k].visitado = 0;
                k++;
            }
        }

    return pontos_internos;
}

/**
 * Grava ciclo no arquivo ciclo.txt
 * 
 * */
void gravaCiclo(int n_ciclo, Ponto *ciclo) {
   
    FILE *fp = fopen("ciclo.txt", "w");
	
    if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar ciclo.txt.\n");
		return;
	}
    
    int i = n_ciclo;
    
    while (i >= 0)
	{
        fprintf(fp, "%f %f\n", ciclo[i].x, ciclo[i].y);
        i--;
	}

    fclose(fp);
}
 
/* Calcula a distancia euclidiana */
int calcDist(Ponto p1, Ponto p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

float tsp(int tam_hull, Ponto *hull, int n_pontos, Ponto *pontos_internos, Ponto *ciclo) {

    int n_internos = n_pontos - tam_hull;

    float custo_ciclo = 0.0;

    //insere o primeiro ponto do fecho convexo no ciclo
    ciclo[0] = hull[0];

    int dist_i_j = 0;
    int dist_i_k = 0;
    int dist_j_k = 0;

    float tripla = FLT_MAX;
    int aux = tripla;
    int i, j = 0;

    for(int count = 1; count < tam_hull; count++) {
        i = count - 1;
        j = count;

        dist_i_j = calcDist(hull[i], hull[j]);
        tripla = FLT_MAX;

        for (int k = 0; k < n_internos; k++) {

            if(!pontos_internos[k].visitado) {
                
                dist_i_k = calcDist(hull[i], pontos_internos[k]);
                dist_j_k = calcDist(hull[j], pontos_internos[k]);
                
                aux = dist_i_k + dist_j_k - dist_i_j;

                if (aux < tripla) {
                    tripla = aux;
                    custo_ciclo += dist_i_k + dist_j_k;

                    //  printf("hull i %f, %f\n", hull[i].x, hull[i].y);
                    //  printf("hull j %f, %f\n\n", hull[j].x, hull[j].y);

                    //  printf("pontos k %f, %f\n\n", pontos_internos[k].x, pontos_internos[k].y);
                    pontos_internos[k].visitado = 1; ///???

                    ciclo[j] = pontos_internos[k];
                    ciclo[j + 1] = hull[j];
                }
            }

        }
    }

    ciclo[n_pontos] = ciclo[0];

    return custo_ciclo;
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
    Ponto *fecho = (Ponto*) malloc((n_pontos + 1) * sizeof(Ponto));
    
    /**
     * Author: https://github.com/AkdenizKutayOcal
     * Repo: https://github.com/AkdenizKutayOcal/Convex-Hull-Calculation-with-Gift-Wrapping-Algorithm
     **/
    //Define e retorna a quantidade de pontos que compoem o fecho convexo 
    int tam_fecho = convexHull(n_pontos, pontos, fecho);

    //vetor para armazenar pontos que sobraram apos calculo do fecho convexo
    Ponto *pontos_internos = (Ponto*) malloc((n_pontos - tam_fecho -1) * sizeof(Ponto));

    removePontos(n_pontos, pontos, fecho, tam_fecho, pontos_internos);

    //vetor para armazenar o ciclo computado em tsp
    Ponto *ciclo = (Ponto*) malloc((n_pontos + 1) * sizeof(Ponto));

    clock_t begin = clock();

    float custo_ciclo = tsp(tam_fecho, fecho, n_pontos, pontos_internos, ciclo);

    clock_t end = clock();

    // for (int i = 0; i < n_pontos; i++) {
    //     printf("%f, %f\n", ciclo[i].x, ciclo[i].y);
    // }

    //tempo algoritmo
	double tempo = (double)(end - begin) / CLOCKS_PER_SEC;

    gravaCiclo(n_pontos, ciclo);

    printf("%lf %f", tempo, custo_ciclo);
}
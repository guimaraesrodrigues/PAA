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

#include <vector>

#include "utils/structs.h"
#include "utils/giftwrapping.h"

using namespace std;

void removePontos(int n_pontos, Ponto pontos[], Ponto* fecho, int tam_fecho, vector<Ponto> &pontos_internos) {

    for (int i = 0; i < n_pontos; i++) {
        for (int j = 0; j < tam_fecho; j++) {
            if(pontos[i].x == fecho[j].x && pontos[i].y == fecho[j].y)
                break;
            else if (j == tam_fecho - 1) {
                pontos_internos.push_back(pontos[i]);
            }
        }
    }
}


/**
 * Grava ciclo no arquivo ciclo.txt
 * 
 * */
void gravaCiclo(vector<Ponto> ciclo) {
   
    FILE *fp = fopen("ciclo.txt", "w");
	
    if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar ciclo.txt.\n");
		return;
	}
    
    int i = 0;

    while ((unsigned)i < ciclo.size())
	{
        fprintf(fp, "%f %f\n", ciclo[i].x, ciclo[i].y);
        i++;
	}

    fclose(fp);
}
 
/* Calcula a distancia euclidiana */
int calcDist(Ponto p1, Ponto p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

/**
 * Entrada: - vector pontos_internos contendo os pontos que estao internos ao fecho convexo
 *          - vector ciclo com os pontos que representam o fecho convexo definido por convexHull()
 **/
float tsp(vector<Ponto> pontos_internos, vector<Ponto> &ciclo) {

    float custo_ciclo = 0.0;
    
    int Vi, Vj, Vk = 0;

    int dist_i_j = 0;
    int dist_i_k = 0;
    int dist_j_k = 0;

    float tripla = FLT_MAX;
    int aux = tripla;

    while(pontos_internos.size() > 0) {

        for(int k = 0; (unsigned)k < pontos_internos.size(); k++) {

            for (int i = 0; (unsigned)i < ciclo.size(); i++) {

                int j = i + 1;

                dist_i_k = calcDist(ciclo[i], pontos_internos[k]);

                dist_j_k = calcDist(ciclo[j], pontos_internos[k]);
                
                if ((unsigned)j == ciclo.size()) {
                    dist_i_j = calcDist(ciclo[i], ciclo[0]);
                    aux = dist_i_k + dist_j_k - dist_i_j;
                }
                else {
                    dist_i_j = calcDist(ciclo[i], ciclo[j]);
                    aux = dist_i_k + dist_j_k - dist_i_j;
                }

                if (aux < tripla) {
                    tripla = aux;
                    Vi = i;
                    Vj = j;
                    Vk = k;
                }
            }
        }

        ciclo.insert(ciclo.begin() + Vj, pontos_internos[Vk]);

        pontos_internos.erase(pontos_internos.begin() + Vk);

        custo_ciclo += tripla;
        tripla = FLT_MAX;
    }
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

    vector<Ponto> pontos_internos;
    vector<Ponto> ciclo;

    removePontos(n_pontos, pontos, fecho, tam_fecho, pontos_internos);

    for(int i= 0; i < tam_fecho; i++)
        ciclo.push_back(fecho[i]);

    clock_t begin = clock();

    float custo_ciclo = tsp(pontos_internos, ciclo);

    clock_t end = clock();

    // for(int i = 0; (unsigned)i < ciclo.size(); i++)
    //     printf("%f, %f\n", ciclo[i].x, ciclo[i].y);

    //tempo algoritmo
	double tempo = (double)(end - begin) / CLOCKS_PER_SEC;

    gravaCiclo(ciclo);

    printf("%lf %f", tempo, custo_ciclo);
}

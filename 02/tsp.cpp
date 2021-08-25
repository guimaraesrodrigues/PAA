//Trabalho 2 - Projeto e Analise de Algoritmos - UTFPR
//Professor Dr. Ricardo Dutra da Silva
//Aluno: Felipe Manikowski - 1457446
//Aluno: Rodrigo Guimarães Rodrigues de Almeida - 1441990

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <vector>
#include "utils/structs.h"
#include "utils/giftwrapping.h"

using namespace std;

//função que remove os pontos que estão no fecho convexo
//	Entrada: int n_pontos - ponteiro que apontará para o primeiro elemento da fila
//			 Ponto pontos[] - lista dos pontos recebidos pelo input
//           Ponto * fecho - vetor do fecho convexo   
//           int tam_fecho - quantidade de pontos que o fecho convexo possui
//           vector<Ponto> &pontos_internos - vector dos pontos internos que serão preenchidos   
//	Saída  : trata-se de uma função void que apenas realiza a remoção de pontos
void removePontos(int n_pontos, Ponto pontos[], Ponto* fecho, int tam_fecho, vector<Ponto> &pontos_internos) {

    for (int i = 0; i < n_pontos; i++) {                                //dois laços são feitos para percorrer os pontos
        for (int j = 0; j < tam_fecho; j++) {                           
            if(pontos[i].x == fecho[j].x && pontos[i].y == fecho[j].y)  //condição para caso o ponto ja esteja no conjunto do fecho convexo
                break;
            else if (j == tam_fecho - 1) {                              //condição para validar a inserção ao vector de pontos internos
                pontos_internos.push_back(pontos[i]);                   //função pushback adiciona o elemento ao final do vector
            }
        }
    }
}


//função que grava o ciclo em um arquivo
//	Entrada: vector<Ponto> ciclo - vector dos pontos do ciclo completo 
//	Saída  : trata-se de uma função void que apenas realiza a inserção dos pontos no arquivo ciclo.txt
void gravaCiclo(vector<Ponto> ciclo) {
   
    FILE *fp = fopen("ciclo.txt", "w"); //Leitura do arquivo
	
    if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar ciclo.txt.\n");
		return;
	}
    
    int i = 0;

    while ((unsigned)i < ciclo.size())                  //laço com a condição de ciclo.size() - size busca o tamanho do vector -  para enquanto 
	{                                                   //o tamanho do vector for maior que a variavel inicializada, o loop ele se mantem verdadeiro
        fprintf(fp, "%f %f\n", ciclo[i].x, ciclo[i].y); //faz a inserção dos elementos x e y da struct Ponto no arquivo
        i++;
	}

    fclose(fp);
}
 
//função que calcula a distância euclidiana entre dois nós
//	Entrada: Ponto p1 - um ponto
//			 Ponto n2 - outro ponto
//	Saida  : um valor float que corresponde à distância euclidiana entre os dois pontos.
int calcDist(Ponto p1, Ponto p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

//função que calcula o custo do ciclo apos fazer a inserção dos pontos 
// Entrada : vector<Ponto> pontos_internos: pontos que estao internos ao fecho convexo
//           vector<Ponto> ciclo com os pontos que representam o fecho convexo definido por convexHull()
// Saida   : float custo_ciclo: o custo calculado entre as distancias entre os pontos do ciclo
// No TSP foi utilizado o cheapest insertion, assim como temos três laços cada um sendo O(n),
// a complexidade temporal resultante, para a função tsp(), é portanto O(n³).
//No algoritmo é feito a varredura nos pontos internos para o ciclo. Assim é calculado a menor
// distancia entre ele o ciclo, que no começo é apenas o fecho calculado pelo convexHull(). O elemento que possuir a menor distância euclidiana será inserido no ciclo,
// então é removido do vector dos pontos internos. O custo é somado toda vez que é inserido e removido um elemento.

float tsp(vector<Ponto> pontos_internos, vector<Ponto> &ciclo) {

    float custo_ciclo = 0.0;
    
    int Vi, Vj, Vk = 0;

    int dist_i_j = 0;
    int dist_i_k = 0;
    int dist_j_k = 0;

    float tripla = FLT_MAX;
    int aux = tripla;

    while(pontos_internos.size() > 0) {                             // o primeiro loop com a condicional se mantenha verdadeira até que os elementos do vector
                                                                    // estejam vazios
        for(int k = 0; (unsigned)k < pontos_internos.size(); k++) { // o segundo loop é para percorrer o vector pontos_internos por todo o seu tamanho

            for (int i = 0; (unsigned)i < ciclo.size(); i++) {      // o terceiro loop é para percorrer o vector ciclo que contem os elementos do fecho convexo

                int j = i + 1;

                dist_i_k = calcDist(ciclo[i], pontos_internos[k]); //calcula e armazena a distancia entre a posição i e k, que corresponde a distancia euclidiana entre os elementos
                                                                   // da posição i do vector ciclo e posição k do vector pontos_internos
                dist_j_k = calcDist(ciclo[j], pontos_internos[k]); //calcula e armazena a distancia entre a posição j e k, que corresponde a distancia euclidiana entre os elementos
                                                                   // da posição j do vector ciclo e posição k do vector pontos_internos
                
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

        ciclo.insert(ciclo.begin() + Vj, pontos_internos[Vk]); //faz a inserção do elemento na posição Vk do vector pontos_internos no vector ciclo na posição Vj

        pontos_internos.erase(pontos_internos.begin() + Vk); //faz a remoção do elemento na posição Vk do vector pontos_internos

        custo_ciclo += tripla; //faz a soma do custo
        tripla = FLT_MAX;
    }
    return custo_ciclo;
}


int main(int argc, char *argv[]) {
    FILE *file = NULL; //variavel para input.txt
    char first_line[20]; //buffer para leitura da primeira linha do arquivo
    int n_pontos; //quantidade de coordendas (x,y) no arquivo

    
    if (argc != 2) { //Checa argumentos da linha de comando
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
   
	createPointsList(file, pontos); // Cria vetor de pares com base no arquivo input.txt
   
    fclose (file); //fecha arquivo input.txt

    Ponto *fecho = (Ponto*) malloc((n_pontos + 1) * sizeof(Ponto)); //vetor para armazenar pontos do fecho convexo
    
    /**
     * Author: https://github.com/AkdenizKutayOcal
     * Repo: https://github.com/AkdenizKutayOcal/Convex-Hull-Calculation-with-Gift-Wrapping-Algorithm
     **/
    
    int tam_fecho = convexHull(n_pontos, pontos, fecho); //Define e retorna a quantidade de pontos que compoem o fecho convexo 

    vector<Ponto> pontos_internos; //inicialização dos vectors 
    vector<Ponto> ciclo;

    removePontos(n_pontos, pontos, fecho, tam_fecho, pontos_internos);

    for(int i= 0; i < tam_fecho; i++) //loop para inserir os elementos do fecho no vector ciclo
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

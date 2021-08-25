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
//	Entrada: int n_pontos - quantidade de pontos em pontos[]
//			 Ponto pontos[] - array dos pontos recebidos pelo input.txt
//           Ponto * fecho - array com pontos do fecho convexo processado em convexHull()
//           int tam_fecho - quantidade de pontos que o fecho convexo possui
//           vector<Ponto> &pontos_internos - referencia para vector pontos_internos declarado na main() 
//	Saída  : trata-se de uma função void que apenas realiza a remoção de pontos
void removePontos(int n_pontos, Ponto pontos[], Ponto* fecho, int tam_fecho, vector<Ponto> &pontos_internos) {

    for (int i = 0; i < n_pontos; i++) {                                //percorre a lista de pontos lida do arquivo.txt
        for (int j = 0; j < tam_fecho; j++) {                           //percorre a lista de pontos no fecho convexo
            if(pontos[i].x == fecho[j].x && pontos[i].y == fecho[j].y)  //se pontos[i] está no fecho, não inclui no vector pontos_internos
                break;
            else if (j == tam_fecho - 1) {                              //Se a iteracao do loop na linha 27 chegou ao final, temos que pontos[i] não está no ciclo
                pontos_internos.push_back(pontos[i]);                   //assim, adicionamos o ponto no vector de pontos_internos
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
        fprintf(fp, "%f %f\n", ciclo[i].x, ciclo[i].y); //faz a inserção dos elementos x e y da struct Ponto no arquivo ciclo.txt
        i++;
	}

    fclose(fp);
}
 
//função que calcula a distancia euclidiana entre dois pontos p1 e p2
//	Entrada: Ponto p1
//			 Ponto p2
//	Saida  : Valor float que correspondea distancia euclidiana entre os dois pontos.
float calcDist(Ponto p1, Ponto p2) {
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

    float dist_i_j = 0;
    float dist_i_k = 0;
    float dist_j_k = 0;

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
                
                if ((unsigned)j == ciclo.size())
                    dist_i_j = calcDist(ciclo[i], ciclo[0]);
                else
                    dist_i_j = calcDist(ciclo[i], ciclo[j]);

                aux = dist_i_k + dist_j_k - dist_i_j;

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

/*
Função principal do programa
    Entrada: int argc - quantidade de argumentos recebidos
             char *argv[] - valor dos argumentos
    Saida: retorna 0 para quando a  funcao eh executada ate o fim
           retorn 1 para quando há algum erro na leitura do arquivo de input ou falta de argumento
*/
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

    Ponto *fecho = (Ponto*) malloc((n_pontos + 1) * sizeof(Ponto)); //aray para armazenar pontos do fecho convexo
    
    /**
     * Author: https://github.com/AkdenizKutayOcal
     * Repo: https://github.com/AkdenizKutayOcal/Convex-Hull-Calculation-with-Gift-Wrapping-Algorithm
     *
     * funcao para encontrar o fecho convexo utilizando o algoritmo gift wrapping 
     **/
    int tam_fecho = convexHull(n_pontos, pontos, fecho); //Define e retorna a quantidade de pontos que compoem o fecho convexo 

    vector<Ponto> pontos_internos; //vector que representara os pontos internos ao fecho convexo
    vector<Ponto> ciclo; //vector que representara o ciclo hamiltoniano

    //funcao que inicializa pontos_internos com os pontos internos ao fecho convexo
    removePontos(n_pontos, pontos, fecho, tam_fecho, pontos_internos);

    for(int i= 0; i < tam_fecho; i++) //loop para inicializar o vector ciclo com os pontos do fecho convexo
        ciclo.push_back(fecho[i]);

    clock_t begin = clock();//inicia contagem de tempo

    //chama funcao que processa o ciclo hamiltoniano
    //passando o vector de pontos_internos e a referencia para o vector ciclo
    //retorna em custo_ciclo o custo do ciclo computado
    float custo_ciclo = tsp(pontos_internos, ciclo);

    clock_t end = clock();//encerra contagem de tempo

    //tempo algoritmo tsp()
	double tempo = (double)(end - begin) / CLOCKS_PER_SEC;

    //grava os pontos do ciclo no arquivo ciclo.txt
    gravaCiclo(ciclo);

    //imprime no console o tempo de execucao do algoritmo tsp e o custo computado para o ciclo
    printf("%lf %f", tempo, custo_ciclo);

    return 0;
}

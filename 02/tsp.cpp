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

// função que calcula que processa ciclo e retorna o seu custo
//
// Entrada: vector<Ponto> pontos_internos: vector com pontos que nao pertecem ao fecho convexo
//          vector<Ponto> &ciclo: ja inicializado com pontos que representam o fecho convexo, vai receber o restante dos pontos em pontos_intenos
//
// Saida: ao fim do algoritmo, vector ciclo deve estar preenchido com um ciclo hamiltoniano formado pelos pontos de input.txt
//
// No TSP foi utilizado o cheapest insertion como estrategia gulosa para resolver os subproblemas
// a complexidade do algoritmo resulta em O(n³) pois, enquanto há pontos internos ainda não inseridos no ciclo, percorremos todos
// os pontos internos e os comparamos com todas as arestas inseridas no ciclo ate interacao corrente.
// Ao fazer a varredura nos pontos internos para o ciclo, calculamos a menor tripla entre ele e todas as arestas do ciclo
// A tripla que possuir o menor valor guardamos o valor dos indices de Vj e Vk para que, ao final da iteracao de pontos_internos, 
// O ponto Vk é inserido no ciclo e removido do vector pontos_internos
void tsp(vector<Ponto> pontos_internos, vector<Ponto> &ciclo) {

    int Vj, Vk = 0; //Variáveis para representar os indices dos vertices Vj e Vk

    //Variáveis para representar as distancias calculadas entre os vertices Vi, Vj e Vk
    float dist_i_j = 0;
    float dist_i_k = 0;
    float dist_j_k = 0;

    float tripla = FLT_MAX; //tripla representara a tripla minima entre os pontos Vi, Vj e Vk. Inicializamos FLT_MAX para garantir que, em algum momento da i-esima interacao, 
                           // vamos encontrar um valor minimo
    float aux = tripla; //variavel auxiliar para utilizar ao longo das interacoes

    // Enquanto há pontos em pontos_internos
    while(pontos_internos.size() > 0) {          
        // Percorremos todos os pontos em pontos_internos para compara 1 a 1 com as arestas em ciclo             
        for(int k = 0; (unsigned)k < pontos_internos.size(); k++) {
            // Percorremos o vector ciclo para calcular a tripla de cada pontos_internos[k] com ciclo[i] e ciclo[j], onde (ciclo[i], ciclo[j]) é uma aresta do ciclo
            for (int i = 0; (unsigned)i < ciclo.size(); i++) {

                int j = i + 1;//indice para acessarciclo[j]

                dist_i_k = calcDist(ciclo[i], pontos_internos[k]); //calcula e armazena a distancia entre a posição i e k, que corresponde a distancia euclidiana entre os elementos
                                                                   // da posição i do vector ciclo e posição k do vector pontos_internos

                dist_j_k = calcDist(ciclo[j], pontos_internos[k]); //calcula e armazena a distancia entre a posição j e k, que corresponde a distancia euclidiana entre os elementos
                                                                   // da posição j do vector ciclo e posição k do vector pontos_internos
                
                //se chegamos ao final do vector ciclo
                //entao usamos o ponto ciclo[0] como ciclo[j]
                if ((unsigned)j == ciclo.size())
                    dist_i_j = calcDist(ciclo[i], ciclo[0]);
                else
                    dist_i_j = calcDist(ciclo[i], ciclo[j]);

                //calcula a tripla minima
                aux = dist_i_k + dist_j_k - dist_i_j;

                 //se for menor do que a tripla minima ja calculada
                if (aux < tripla) {
                    tripla = aux; //atualiza o valor da tripla minima
                     //salva indices para usar na insercao e remocao ao final do loop da linha 99
                    Vj = j;
                    Vk = k;
                }
            }
        }

        ciclo.insert(ciclo.begin() + Vj, pontos_internos[Vk]); //faz a inserção do elemento na posição Vk do vector pontos_internos no vector ciclo na posição Vj

        pontos_internos.erase(pontos_internos.begin() + Vk); //faz a remoção do elemento na posição Vk do vector pontos_internos

        tripla = FLT_MAX;//reinicia o valor da tripla minima 
    }
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
    //ao fim do algoritmo, vector ciclo deve estar preenchido com um ciclo hamiltoniano formado pelos pontos de input.txt
    tsp(pontos_internos, ciclo);

    clock_t end = clock();//encerra contagem de tempo

    //tempo algoritmo tsp()
	double tempo = (double)(end - begin) / CLOCKS_PER_SEC;

    //grava os pontos do ciclo no arquivo ciclo.txt
    gravaCiclo(ciclo);

    float custo_ciclo = 0.0;

    //Percorre vector ciclo para calcular o custo do ciclo hamiltoniano
    for(int i = 0; (unsigned)i < ciclo.size(); i++) {
        if ((unsigned)i + 1 == ciclo.size())
            custo_ciclo += calcDist(ciclo[i], ciclo[0]);
        else
            custo_ciclo += calcDist(ciclo[i], ciclo[i+1]);
    }

    //imprime no console o tempo de execucao do algoritmo tsp e o custo computado para o ciclo
    printf("%lf %f", tempo, custo_ciclo);

    return 0;
}

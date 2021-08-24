#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>


//Trabalho 2 - Projeto e Analise de Algoritmos - UTFPR
//Professor Dr. Ricardo Dutra da Silva
//Aluno Felipe Manikowski - 1457446

using namespace std;

#define MAX_XAXIS 700
#define MAX_YAXIS 700
#define MAX_POINTS (MAX_XAXIS * MAX_YAXIS)
//Define a estrutura usada para um ponto
struct Ponto 
{   //Eixos x e y dos pontos
    long x, y;

    //Define operadores de comparacao para permitir ordenacao no SET da biblioteca stl
    bool operator<(const Ponto &other) const
    {
        if (this->y < other.y)
            return true;
        if (other.y < this->y)
            return false;
        return this->x < other.x;
    }
    bool operator>(const Ponto &other) const { return other < *this; }
    bool operator<=(const Ponto &other) const { return !(other < *this); }
    bool operator>=(const Ponto &other) const { return !(*this < other); }
};
//Usar um set da biblioteca stl para facilitar impressao e ordenacao
set<Ponto> hull; 

//Obtem o lado de p em relacao a linha
short get_lado(Ponto plinha1, Ponto plinha2, Ponto p)
{
    long lado = (p.y - plinha1.y) * (plinha2.x - plinha1.x) - (plinha2.y - plinha1.y) * (p.x - plinha1.x);

    if (lado > 0)
        return 1;
    if (lado < 0)
        return -1;

    return 0;
}

//Distancia entre linha e ponto
long calcular_distancia(Ponto plinha1, Ponto plinha2, Ponto p)
{
    long dist = (p.y - plinha1.y) * (plinha2.x - plinha1.x) - (plinha2.y - plinha1.y) * (p.x - plinha1.x);
    return abs(dist);
}

//Funcao: quickhull_recursivo
//Entrada da funcao: Carrega os pontos, quantidade pontos inseridos.
//Saida: recursividade da funcao quickhull_recursivo
//Ordem de Complexidade: O (n log(n))
//Corretude: O algoritmo esta correto e pode ser executado em tempo polinomial.
void quickhull_recursivo(Ponto pontos[], size_t pontos_count, Ponto plinha1, Ponto plinha2, short lado)
{

    long maior_distancia = 0;
    long maior_distancia_indice = -1; //Indice do ponto com maior dist
    //Localiza ponto mais distancia no lado valido
    for (size_t i = 0; i < pontos_count; i++) 
    {
        int temp = calcular_distancia(plinha1, plinha2, pontos[i]);
        if (get_lado(plinha1, plinha2, pontos[i]) == lado && temp > maior_distancia)
        {
            maior_distancia_indice = i;
            maior_distancia = temp;
        }
    }

    if (maior_distancia_indice == -1)
    {
        hull.insert(plinha1);
        hull.insert(plinha2);
        return;
    }

    quickhull_recursivo(pontos, pontos_count, pontos[maior_distancia_indice], plinha1, -get_lado(pontos[maior_distancia_indice], plinha1, plinha2));
    quickhull_recursivo(pontos, pontos_count, pontos[maior_distancia_indice], plinha2, -get_lado(pontos[maior_distancia_indice], plinha2, plinha1));
}

//Funcao GeraHull:
//Entrada da funcao: Carrega o hull em um set, quantidade de pontos inseridos.
//Saida: imprime a saida dos pontos em arquivo, ou retorna caso pontos seja menor que 3.
//Ordem de Complexidade: O (n log(n))
//Corretude: Se o algoritmo retorna na primeira condição não existe um conjunto de pontos que encontra hull. O algoritmo esta correto e pode ser executado em tempo polinomial
void gerar_hull(Ponto pontos[], size_t pontos_count)
{   //Nao encontra hull se numero de pontos menor que 3
    if (pontos_count < 3) 
        return;

    //Encontrar o indice dos pontos com minX e maxX
    size_t min_x_indice = 0, max_x_indice = 0;
    for (size_t i = 1; i < pontos_count; i++)
    {
        if (pontos[i].x < pontos[min_x_indice].x)
        {
            min_x_indice = i;
        }
        if (pontos[i].x > pontos[max_x_indice].x)
        {
            max_x_indice = i;
        }
    }

    //Formar linha entre os pontos de minX e maxX e econtrar o hull de ambos os lados
    quickhull_recursivo(pontos, pontos_count, pontos[min_x_indice], pontos[max_x_indice], 1);
    quickhull_recursivo(pontos, pontos_count, pontos[min_x_indice], pontos[max_x_indice], -1);
    //Escreve no arquivo de saida fecho
    FILE *arquivo_saida = fopen("fecho.txt", "w");
    assert(arquivo_saida != NULL);

    while (!hull.empty())
    {
        auto ponto = hull.begin();
        fprintf(arquivo_saida, "%ld %ld\n", ponto->x, ponto->y);
        hull.erase(hull.begin());
    }

    fclose(arquivo_saida);
}

//Funcao para leitura do arquivo input.txt
size_t ler_entrada(Ponto pontos[], const char *arquivo_nome)
{
    size_t pontos_count = 0;
    FILE *arquivo_entrada = fopen(arquivo_nome, "r");
    assert(arquivo_entrada != NULL);

    if (!fscanf(arquivo_entrada, "%zu", &pontos_count))
    {
        fprintf(stderr, "Erro na leitura do arquivo.");
        return EXIT_FAILURE;
    }

    if (pontos_count > MAX_POINTS)
    {
        fprintf(stderr, "numero de pontos deve ser menor do que %d.\n", MAX_POINTS);
        return 1;
    }

    for (size_t i = 0; i < pontos_count; i++)
    {
        if (!fscanf(arquivo_entrada, "%ld %ld", &pontos[i].x, &pontos[i].y))
        {
            fprintf(stderr, "Erro na leitura do arquivo.");
            return EXIT_FAILURE;
        }
    }

    fclose(arquivo_entrada);
    return pontos_count;
}

//definir como infinito (o calculo dos caminhos n ultrapassa)
#define infinito 0x7FFFFFFF

//representacao das coordenadas do Point
struct Point
{
    int x;
    int y;
};

//Aresta de valores (reprensetam os caminhos de menor custo)
struct Aresta
{
    double val;
    vector<int> caminhos;
    Aresta *prox;
};

/**
	* Classe para inserir/pegar uma Aresta de menor custo atraves do vetor caminhos (sao as chaves)
	*/
class PointsVisitas
{
    //instanciacao inicial
private:
    vector<Aresta *> ini;
    vector<Aresta *> fim;

public:
    /**
		* Construtor da classe, apenas inicializa os ponteiros
		* @param numPoints para facilitar a implementacao, colocamos para criar vetores do tamanho do numero
		* de Points.
		**/
    PointsVisitas(int numPoints)
    {
        ini.resize(numPoints + 1);
        fim.resize(numPoints + 1);
        for (int x = 0; x < numPoints + 1; x++)
        {
            ini[x] = NULL;
            fim[x] = NULL;
        }
    }

    /**
		* Metodo para inserir o menor valor encontrado entre as combinacoes 
		* @param vertice representa em qual vertice o caminho esta passando
		* @param val o menor valor encontrado entre as combinacoes
		* @param caminho representa o caminho passado ate chegar no vertice atual
		**/
    void inserir(int vertice, double val, vector<int> caminho)
    {

        Aresta *nova = new Aresta;

        //Caso seja o primeira Aresta a ser inserida
        if (ini[vertice] == NULL)
        {

            nova->val = val;
            nova->caminhos.resize(caminho.size());
            nova->caminhos = caminho;
            nova->prox = NULL;

            ini[vertice] = nova;
            fim[vertice] = ini[vertice];
        }
        else
        {
            //adicionar novas Arestas
            nova->val = val;
            nova->caminhos.resize(caminho.size());
            nova->caminhos = caminho;
            nova->prox = NULL;

            fim[vertice]->prox = nova;
            fim[vertice] = nova;

            nova = NULL;
        }
    }

    /**
		* Funcao para recuperar o valor guardado referente a combinacao do caminho passado
		* @param vertice representa em qual vertice o caminho esta passando
		* @param caminho representa a chave para encontrar o caminho
		**/
    double getVal(int vertice, vector<int> caminho)
    {
        double resposta = infinito;
        Aresta *temp = new Aresta;
        temp = ini[vertice];
        bool encontrado = true;

        if (caminho.size() == 0)
        {
            return (ini[vertice]->val);
        }
        else
        {
            //verificar todas as Arestas do vertice desejado
            while (temp != NULL)
            {
                //verificar se as chaves correspondem ao caminho procurado
                for (int x = 0; (unsigned)x < caminho.size(); x++)
                {
                    if (caminho[x] != temp->caminhos[x])
                    {
                        encontrado = false;
                    }
                }

                if (encontrado)
                {
                    resposta = temp->val;
                    break;
                }
                encontrado = true;
                temp = temp->prox;
            }
            return (resposta);
        }
    }
};

/**
	* Classe executora do problema do caixeiro viajante
	**/
class TSP
{
private:
    //Declaracao de variaveis globais
    FILE *arqEntrada;
    FILE *arqSaida;
    int n;
    int controle;
    Point *Points;
    vector<int> auxComb;
    vector<int> vetor;
    vector<vector<int>> combinacaoElementos;
    vector<vector<double>> mAdjacentes;
    vector<PointsVisitas *> custo;

public:
    /**
		* Construtor da classe TSP
		* Inicializar todas as variaveis necessarias para a execucao do algoritmo
		**/
    TSP(int qtd)
    {
        // char nome[256];
        // cout << "Informar nome arquivo entrada : ";
        // cin >> nome;
        arqEntrada = fopen("fecho.txt", "r");
        arqSaida = fopen("ciclo.txt", "w");
        // fscanf(arqEntrada, "%d\n", &n);
        n = qtd;
        vetor.resize(n - 1);
        Points = new Point[n + 1];
        Points[0].x = 0;
        Points[0].y = 0;
        for (int x = 1; x <= n; x++)
        {
            fscanf(arqEntrada, "%d %d\n", &Points[x].x, &Points[x].y);
        }
        fclose(arqEntrada);
        preencherMAdjacentes();
        for (int x = 0; x < (n - 1); x++)
        {
            vetor[x] = x + 2;
        }
    }

    /**
		* Metodo principal responsavel pela execucao e controle das estruturas para o caixeiro viajante
		**/
    void algoritmoTSP()
    {
        //inicializar as variaveis
        time_t inicio, fim;
        inicio = clock();
        double custoAux;
        double custoCaminho;
        custo.resize(n - 1);
        vector<int> inicial;
        inicial.resize(1);
        inicial[0] = 1;
        for (int x = 0; x < n - 1; x++)
        {
            custo[x] = new PointsVisitas(n);
        }

        //inserir os valores iniciais, ou seja, Point 1 -> 2, Point 1 -> 3, Point 1 -> 4
        for (int x = 2; x <= n; x++)
        {
            custo[0]->inserir(x, mAdjacentes[x][1], extrair(inicial, 1));
        }

        //numero de conjuntos formados a partir do numero de Points
        //se possuir 5 Points entao podemos formar 4 subconjuntos, tirando o conjunto vazio
        //aqui serao formados 3 subconjuntos
        for (int s = 1; s < (n - 1); s++)
        {
            //passar por todas os Points
            for (int x = 2; x <= n; x++)
            {
                //variavel global para o funcionamento do metodo combinacoes()
                controle = 0;
                //numero total de combinacoes a partir do tamanho do conjunto
                int numComb = calcularNumCombinacoes(extrair(vetor, x).size(), s);
                alocarCombinacoes(numComb, s);
                auxComb.resize(s);
                combinacoes(extrair(vetor, x), s, 0, auxComb);
                //realizar a quantidade combinacoes
                for (int j = 0; j < numComb; j++)
                {
                    vector<int> vetAux = removerVetor(s, j);
                    custoCaminho = infinito;

                    //fazer a quantidade de vezes para cada Point pertecente ao conjunto
                    for (int k = 0; k < s; k++)
                    {
                        //calcular o custo
                        custoAux = mAdjacentes[x][vetAux[k]] + custo[s - 1]->getVal(vetAux[k], extrair(vetAux, vetAux[k]));

                        //verificar se e' o menor custo passando por esta Point
                        if (custoAux < custoCaminho)
                        {
                            custoCaminho = custoAux;
                        }
                    }
                    //apos verificar qual a melhor Point de menor caminho adiciona-la a estrutura de dados
                    custo[s]->inserir(x, custoCaminho, vetAux);
                }
            }
        }
        //auxiliar para o custo
        custoCaminho = infinito;
        int PointBase = 0;
        //realizar a passagem no ultimo Point
        for (int x = 2; x <= n; x++)
        {

            //calcular o custo
            custoAux = mAdjacentes[1][x] + custo[n - 2]->getVal(x, extrair(vetor, x));

            //verificar se eh o menor custo passando por esta Point
            if (custoAux < custoCaminho)
            {
                custoCaminho = custoAux;
                PointBase = x;
            }
        }

        //Mostrar sem tempo
        //metodo para recuperar o caminho
        //fprintf(arqSaida, "%0.2f\n1 ", custoCaminho);
        //recuperarCaminho (PointBase, vetor, (n-2));
        //fclose(arqSaida);

        //Mostrar com tempo
        fprintf(arqSaida, "NUMERO PONTOS : %d\nCAMINHO\n ", n);
        recuperarCaminho(PointBase, vetor, (n - 2));
        fim = clock();
        fprintf(arqSaida, "\nCUSTO : %0.2f", custoCaminho);
        fprintf(arqSaida, "\nTEMPO : %0.2f\n", ((double(fim) - double(inicio)) / CLOCKS_PER_SEC));
        printf("\n");
        fclose(arqSaida);
    }

    //Destrutor da classe
    ~TSP() {}

private:
    /**
		* Metodo que aloca espaco para a matriz de combinacoes.
		* As combinacoes sao alocadas dinamicamente de acordo com o tamnho do conjunto e o numero de vertices
		* @param linhas recebe o numero de linhas da matriz
		* @param colunas recebe o numero de colunas da matriz
		**/
    void alocarCombinacoes(int linhas, int colunas)
    {
        combinacaoElementos.resize(linhas);
        for (int x = 0; x < linhas; x++)
        {
            combinacaoElementos[x].resize(colunas);
        }
    }

    /**
		* Metodo que realiza o calculo da distancia entre dois pontos
		* Aplicada a formula para o calcula da distancia entre dois pontos 
		* @param primeira recebe uma struct do tipo Point que possui as coordenadas 'X' e 'Y'
		* @param segunda recebe uma struct do tipo Point que possui as coordenadas 'X' e 'Y'
		**/
    double calcularDistancia(Point primeira, Point segunda)
    {
        double distancia = sqrt(pow((primeira.x - segunda.x), 2) + pow((primeira.y - segunda.y), 2));
        return (distancia);
    }

    /**
		* Metodo que realiza o preenchimento da matriz de distancias entre os Points
		* E' um grafo onde o valor para o proprio vertice eh preenchido com 0
		* o calculo eh realizado a partir da funcao 'calcularDistancia'
		**/
    void preencherMAdjacentes()
    {
        mAdjacentes.resize(n + 1);
        for (int x = 0; x < n + 1; x++)
        {
            mAdjacentes[x].resize(n + 1);
        }
        for (int y = 0; y < n + 1; y++)
        {
            mAdjacentes[y].resize(n + 1);
        }        
        for (int x = 0; x <= n; x++)
        {
            for (int y = 0; y <= n; y++)
            {
                if (x == 0 || y == 0 || x == y)
                {
                    mAdjacentes[x][y] = 0.0;
                }
                else
                {
                    mAdjacentes[x][y] = calcularDistancia(Points[x], Points[y]);
                }
            }
        }
        delete (Points);
    }

    /**
		* Metodo que realiza a extracao de um elemento do vetor (conjunto)
		* @param vet recebe o vetor (conjunto) com os vertices
		* @param elemento o elemento para realizar a extracao dentro do vetor (conjunto)
		* @return retorna o vetor (conjunto) sem o vertice extraido
		**/
    vector<int> extrair(vector<int> vet, int elemento)
    {
        vector<int> resposta;
        resposta.resize(vet.size() - 1);
        int pos = 0;
        for (int x = 0; (unsigned)x < vet.size(); x++)
        {
            if (vet[x] != elemento)
            {
                resposta[pos++] = vet[x];
            }
        }
        return (resposta);
    }

    /**
		* Funcao que calcula o numero de combinacoes do conjunto de vertices.
		* A funcao nao e' eficiente, pois o maximo de combinacoes e' 19, acima disso a variavel int
		* ultrapassa sua representacao
		* @param numEle recebe a quantidade de elementos para combinacao
		* @param tamConj o tamanho do conjunto para dividir como fatorial
		* @return retornar o numero de combinacoes
		**/
    int calcularNumCombinacoes(int numEle, int tamConj)
    {
        int combinacaoDividendo = 1;
        int combinacaDivisor = 1;
        int aux = tamConj;
        for (int x = 0; x < tamConj; x++)
        {
            if (numEle != aux)
            {
                combinacaoDividendo *= numEle--;
            }
            else if (numEle == aux)
            {
                numEle--;
                aux--;
            }
        }
        int aux2 = aux;
        for (int x = 0; x < aux2; x++)
        {
            combinacaDivisor *= aux--;
        }
        return (combinacaoDividendo / combinacaDivisor);
    }

    /**
		* Como nos guardamos todas as combinacoes em uma matriz. Cada linha da matriz representa uma combinacao entre os elementos
		* entao escolhemos em qual combinacao estamos, selecionamos a linhas e copiamos os elementos dela para a resposta.
		* @param escolher escolhe linha da matriz que sera copiada para a resposta
		* @param s o tamnho do conjunto (numero de colunas)
		* @return retornar um vetor com a combinacao escolhida
		**/
    vector<int> removerVetor(int s, int escolher)
    {
        vector<int> resp;
        resp.resize(s);
        for (int x = 0; x < s; x++)
        {
            resp[x] = combinacaoElementos[escolher][x];
        }
        return (resp);
    }

    /**
		* Metodo recursivo que realiza as combinacoes dos elementos do conjunto, de acordo com o tamanho do conjunto desejado. 
		* Por exemplo: {2,3,4} --> combinacao de tamnho 2 --> {2,3}, {2,4}, {3,4}
		* @param arr recebe o conjunto dos elementos
		* @param len o tamanho do conjunto
		* @param posicaoInicial posicao referencia para comecar a selecionar o elemento
		* @param aux vetor auxiliar para preencher os valores na matriz de combinacoes
		**/
    void combinacoes(vector<int> arr, int len, int posicaoInicial, vector<int> aux)
    {
        if (len == 0)
        {
            for (int x = 0; (unsigned)x < aux.size(); x++)
            {
                combinacaoElementos[controle][x] = aux[x];
            }
            controle++;
            return;
        }
        for (int i = posicaoInicial; (unsigned)i <= arr.size() - len; i++)
        {
            aux[aux.size() - len] = arr[i];
            //chamda recursiva
            combinacoes(arr, len - 1, i + 1, aux);
        }
    }

    /**
		* Metodo recursivo para realizar a recuperacao do caminho percorrido.
		* Ele realiza a extracao do vertice que possui o menor valor para aquele caminho
		* por exemplo: conjunto de tamanho 3 = {2,3,4} com o vertice base = 5
		* temos que : a primeira Point e' 5 verifica-se retirando cada elemento do conjunto qual 
		* dara o menor caminho por exemplo se for o 3, entao a proxima Point e' 3
		* @param caminho e' o vertice base que representara' o caminho passado
		* @param vet e' o vetor de elementos sempre reduzido pelo metodo recursivo, ate chegar o ultimo
		* @param c e' o tamanho do conjunto anterior ao atual, ou seja, se o atual e' 3 o anterior e' 2
		**/
    void recuperarCaminho(int caminho, vector<int> vet, int c)
    {
        vector<int> aux = extrair(vet, caminho);
        if (aux.size() != 0)
        {
            int custoAux;
            int custoCaminho = infinito;
            int temp = 0;
            for (int x = 0; (unsigned)x < aux.size(); x++)
            {
                //calcular o custo
                custoAux = custo[c - 1]->getVal(aux[x], extrair(aux, aux[x]));

                //verificar qual custo e' menor para aquele caminho
                if (custoAux < custoCaminho)
                {
                    custoCaminho = custoAux;
                    temp = aux[x];
                }
            }
            //chamada recursiva
            recuperarCaminho(temp, aux, (c - 1));
        }
    }
};

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        fprintf(stderr, "Uso: %s [arquivo]\n", argv[0]);
        return EXIT_FAILURE;
    }
    //Clock inicial para contagem
    clock_t begin = clock();

    Ponto pontos[MAX_POINTS];
    size_t pontos_count = ler_entrada(pontos, argv[1]);

    gerar_hull(pontos, pontos_count);
    TSP *novo = new TSP(pontos_count);
    novo->algoritmoTSP();
    delete (novo);
    //Clock final para contagem
    clock_t end = clock(); 
    //tempo de execucao
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    //Imprimir tempo de execucao
    printf("%lf\n", time_spent);

    return EXIT_SUCCESS;
}

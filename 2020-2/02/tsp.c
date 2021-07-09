/**
 * Trabalho 02
 * 
 * Autores:
 * Ana Yanaze - 1614673
 * Camila Puchta - 1043820
 * Rodrigo Guimarães - 1441990
 * 
 * **/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#define BUFLEN 512

typedef struct _Ponto Ponto;

// Struct para representar o ponto
// Ponto = vértice
typedef struct _Ponto {
  int id;
  float x, y;
  float key; // Para o algoritmo de PRIM
  int pai;  //para o algoritmo de PRIM - ID DO PONTO PAI
  int busca; //para busca em profundidade
  Ponto *filhos;
  float custo_ciclo;
} Ponto;

// ARESTA: inicio -> fim com peso distancia
typedef struct Aresta {
  Ponto inicio;
  Ponto fim;
  float distancia;
} Aresta;

int id_verificados[BUFLEN];

// Calcula a distância euclidiana entre p1 e p2
float dist(Ponto p1, Ponto p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

/**
 * Retorna aresta com inicio -> fim e peso representado
 * pela distancia euclidiana entre os pontos inicio e fim
 * 
 * */
Aresta inicializaAresta(Ponto inicio, Ponto fim) {
    Aresta a;
    a.inicio = inicio;
    a.fim = fim;
    a.distancia = dist(inicio, fim);
    return a;
}

// Cria uma matriz de adjacencias baseado no arquivo passado como parametro
//Ex. da matriz apos inicializacao, onde x eh lixo e [A..E] sao struct Aresta:
/**
 * A B C D E
 * B B x x x
 * C x C x x
 * D x x D x
 * E x x x E
 **/
void criaGrafo(FILE *file, int n_pontos, Aresta grafo[n_pontos][n_pontos], Ponto pontos[]) {
	int ret = 0;
	int i = 0;
	char line_buffer[100]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e insere o valor na matriz de ajacências
	while(fgets(line_buffer, sizeof(line_buffer), file)) {
		ret = sscanf(line_buffer, "%f %f", &pontos[i].x, &pontos[i].y);
        pontos[i].pai = -1;
        pontos[i].key = FLT_MAX;
        pontos[i].id = i;
        
        if(ret < 1)
			continue;

        //Inicializa linha 0
        grafo[0][i].inicio.x = pontos[i].x;
        grafo[0][i].inicio.y = pontos[i].y;

        //Inicializa coluna 0 
		grafo[i][0].inicio.x = pontos[i].x;
        grafo[i][0].inicio.y = pontos[i].y;

        grafo[i][0] = inicializaAresta(grafo[i][0].inicio, grafo[0][0].inicio);

        grafo[0][i].distancia =  grafo[i][0].distancia;
        grafo[0][i].inicio = grafo[i][0].fim;
        grafo[0][i].fim = grafo[i][0].inicio;
        
        // Inicializa diagonal da matriz
        grafo[i][i].inicio = grafo[i][0].inicio;
        grafo[i][i].fim = grafo[i][0].inicio;
        grafo[i][i].distancia = 0;

		i++;
	}
}

/**
 * Inicia com i e j = 1 pois a pimeira linha e coluna ja
 * foram inicializadas.
 * 
 **/
void preencheGrafo(int n_pontos, Aresta grafo[n_pontos][n_pontos]) {
    for(int i = 1; i < n_pontos; i++)
        for(int j = 1; j < n_pontos; j++) 
            //pula grafo[i][j], onde i == j, ja foi inicializada na funcao criaGrafo
            if( i != j) {
                grafo[j][i] = inicializaAresta(grafo[i-1][j].fim, grafo[i][j-1].inicio);
                
                //inicializa grafo[i][j]
                grafo[i][j].inicio = grafo[j][i].fim;
                grafo[i][j].fim = grafo[j][i].inicio;
                grafo[i][j].distancia =  grafo[j][i].distancia;
            } 

}

int verticeFoiVerificado(int id_vertice, int n_vertices_verificadas) {
    for (int i = 0; i < n_vertices_verificadas; i++ ) {
        if (id_verificados[i] == id_vertice) {
            return 1;
        }
    }

    return 0;
}

// Em tese, o algoritmo de Prim teria complexidade O(E log V), sendo E as vertices e E as arestas.
// Contudo, como não utilizamos um heap minimo pela dificuldade com a manipulacao de estrutura de dados
// na linguagem C, a complexidade aqui ficou quadrática.
void prim(int n_pontos, Aresta grafo[n_pontos][n_pontos], Ponto vertices[]) {
    // algoritmo guloso = escolhe algum pra começar e nao testa outros
    // chave e pai de cada vértice foram inicializados na função criaGrafo!
    int verificados = 0;

    vertices[0].pai = -1;
    vertices[0].key = 0;
    Ponto raiz = vertices[0];

    //começa do primeiro ponto.
    for (int i = 0; i < n_pontos; i++) {

        // vamos chamar a atual vértice que estamos olhando de pai. pois ela pode ser pai de alguém.
        Ponto pai = vertices[i];
        //apenas para adicionar um alias e melhorar visualização da lógica
        int vertice_atual_index = i;

        /* Para cada adjacencia, deve ser testada a distancia entre i (pai)
        e as subsequentes colunas. se for menor, atribui o valor no vértice (ponto) e
        atualiza o pai como sendo o ponto iterado pela variável i */
        for (int j = 0; j < n_pontos; j++) {
            // faz algumas verificações:
            // a aresta é de fato ligada ao vértice que estou olhando? ok se sim
            // o vertice adjacente que vou comparar esta na lista de verificados? ok se não
            // verifica também se nao estou comparando o vertice com ele mesmo. 
            // aplica a verificacao do Prim 
            // (se o que tenho no momento é maior que o valor oferecido, então substituo e coloco meu pai como o vértice atual)
            if (grafo[i][j].inicio.x == pai.x && grafo[i][j].inicio.y == pai.y &&
                ! (verticeFoiVerificado(vertices[j].id, verificados)) &&
                i != j && 
                grafo[i][j].distancia < vertices[j].key) {

                vertices[j].key = grafo[i][j].distancia;
                vertices[j].pai = pai.id;
                
            }
        }
        
        id_verificados[verificados] = vertice_atual_index;
        verificados++;
    }
}

/**
 * 
 * **/
Ponto buscaProf(int n_pontos, Aresta grafo[n_pontos][n_pontos], Ponto vertices[], Ponto busca[]){
    Ponto goal = vertices[0];
    int v = 0;
    for(int i= 0; i < n_pontos; i++){
        if(vertices[i].id == goal.id){
            return vertices[i]; //encontrou o vertice inicial
        } else {
            if (vertices[i].busca == -1){
                //se ainda não foi visitado, coloca na lista e visita filhos
                busca[v] = vertices[i];
                buscaProf(n_pontos, grafo, vertices[i].filhos, busca);
            }
        }
    }
    return goal;
};

/**
 * Grava AGM no arquivo tree.txt
 * 
 * */
void gravaAGM(int n_pontos, Ponto pontos[n_pontos]) {
    FILE *fp = fopen("tree.txt", "w");
	if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar tree.txt.\n");
		return;
	}
    int n = 1;
    while (n < n_pontos)
	{
        fprintf(fp, "%d %d\n", pontos[n].pai, pontos[n].id);
        n++;
	}
    fclose(fp);
}

/**
 * Grava ciclo no arquivo cycle.txt
 * 
 * */
void gravaCiclo(int n_pontos, Ponto vertices[n_pontos]) {
    FILE *fp = fopen("cycle.txt", "w");
	if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar tree.txt.\n");
		return;
	}
    int n = 0;
    while (n < n_pontos)
	{
        fprintf(fp, "%d %d\n", vertices[n].pai, vertices[n].id);
        n++;
	}
    fclose(fp);
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

    Aresta grafo[n_pontos][n_pontos];//Matriz de adjacencias
    Ponto pontos[n_pontos];
    
    clock_t begin = clock();

    criaGrafo(file, n_pontos, grafo, pontos);// Cria grafo com base no arquivo input.txt
    fclose (file);

    preencheGrafo(n_pontos, grafo);//inicializa as arestas que nao foram contempladas em criaGrafo

    prim(n_pontos, grafo, pontos);

    gravaAGM(n_pontos, pontos);

    Ponto busca[n_pontos];

    float custo_ciclo = buscaProf(n_pontos, grafo, pontos, busca).custo_ciclo;

    gravaCiclo(n_pontos, busca);

    clock_t end = clock();

    //tempo algoritmo
	double tempo = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("%lf %f", tempo, custo_ciclo);
}
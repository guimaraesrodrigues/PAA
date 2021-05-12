/**
 * Trabalho 02
 * 
 * Autores:
 * Ana Yanaze - RA
 * Camila
 * Rodrigo Guimarães - 1441990
 * 
 * **/
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#include <float.h>

// Struct para representar o ponto
// Ponto = vértice
typedef struct Ponto {
    int id;
  float x, y;
  float key; // Para o algoritmo de PRIM
  int pai;  //para o algoritmo de PRIM - ID DO PONTO PAI
} Ponto;

// ARESTA: inicio -> fim com peso distancia
typedef struct Aresta {
  Ponto inicio;
  Ponto fim;
  float distancia;
} Aresta;

// NAO TENHO CERTEZA DA NECESSIDADE DESSA STRUCT
typedef struct Grafo {
   int n_vertices; 
//    int n_arestas;  NAO TENHO CERTEZA DO USO DISSO
   Aresta **aresta; 
} Grafo;

// Calcula a distância euclidiana entre p1 e p2
float dist(Ponto p1, Ponto p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

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
 * 
 * 
 **/
void preencheGrafo(int n_pontos, Aresta grafo[n_pontos][n_pontos]) {
    for(int i = 1; i < n_pontos; i++)
        for(int j = 1; j < n_pontos; j++) 
            if( i != j) {
                grafo[j][i] = inicializaAresta(grafo[i-1][j].fim, grafo[i][j-1].inicio);
                
                //inicializa grafo[i][j]
                grafo[i][j].inicio = grafo[j][i].fim;
                grafo[i][j].fim = grafo[j][i].inicio;
                grafo[i][j].distancia =  grafo[j][i].distancia;
            } 

}

int ehLaco(Aresta a) {
    if((a.inicio.x == a.fim.x) && (a.inicio.y == a.fim.y))
        return 1;
    
    return 0;
}

// A utility function to find the vertex with 
// minimum key value, from the set of vertices 
// not yet included in MST 
int minKey(float key[], int mstSet[], int n_vertices) 
{ 
    // Initialize min value 
    float min = INT_MAX, min_index; 
  
    for (int v = 0; v < n_vertices; v++) 
        if (mstSet[v] == 0 && key[v] < min) 
            min = key[v], min_index = v; 
  
    return min_index; 
} 

// Function to construct and print MST for 
// a graph represented using adjacency 
// matrix representation 
void primMST(int n_vertices, Aresta grafo[n_vertices][n_vertices]) 
{ 
    // Array to store constructed MST 
    int parent[n_vertices]; 
      
    // Key values used to pick minimum weight edge in cut 
    float key[n_vertices]; 
      
    // To represent set of vertices included in MST 
    int mstSet[n_vertices]; 
  
    // Initialize all keys as INFINITE 
    for (int i = 0; i < n_vertices; i++) 
        key[i] = INT_MAX, mstSet[i] = 0; 
  
    // Always include first 1st vertex in MST. 
    // Make key 0 so that this vertex is picked as first vertex. 
    key[0] = 0; 
    parent[0] = -1; // First node is always root of MST 
  
    // The MST will have V vertices 
    for (int count = 0; count < n_vertices - 1; count++)
    { 
        // Pick the minimum key vertex from the 
        // set of vertices not yet included in MST 
        int u = minKey(key, mstSet, n_vertices); 
  
        // Add the picked vertex to the MST Set 
        mstSet[u] = 1; 
  
        // Update key value and parent index of 
        // the adjacent vertices of the picked vertex. 
        // Consider only those vertices which are not 
        // yet included in MST 
        for (int v = 0; v < n_vertices; v++) 
  
            // graph[u][v] is non zero only for adjacent vertices of m 
            // mstSet[v] is false for vertices not yet included in MST 
            // Update the key only if graph[u][v] is smaller than key[v] 
            if (ehLaco(grafo[u][v]) && mstSet[v] == 0 && grafo[u][v].distancia < key[v]) 
                parent[v] = u, key[v] = grafo[u][v].distancia; 
    } 
  
    // print the constructed MST 
    // printMST(parent, graph); 
}



void prim(int n_pontos, Aresta grafo[n_pontos][n_pontos], Ponto vertices[]) {
    // algoritmo guloso = escolhe algum pra começar e nao testa outros
    // chave e pai de cada vértice foram inicializados na função criaGrafo!

    //começa do primeiro ponto.
    for (int i = 0; i < n_pontos; i++) {
        Ponto pai = vertices[i];
        //apenas para adicionar um alias e melhorar visualização da lógica

        int pai_index = i;

        /* Para cada adjacencia, deve ser testada se a distancia entre i (pai)
        e as subsequentes colunas. se for menor, atribui o valor no vértice (ponto) e
        atualiza o pai como sendo o ponto iterado pela variável i */
        for (int j = 0; j < n_pontos; j++) {
            // a aresta é de fato ligada ao vértice que estou olhando?
            if (grafo[i][j].inicio.x == pai.x && grafo[i][j].inicio.y == pai.y ) {
                if (i != j && grafo[i][j].distancia < vertices[j].key) {
                    printf("%.0f\n",grafo[i][j].distancia );
                    vertices[j].key = grafo[i][j].distancia;
                    vertices[j].pai = pai.id;
                    printf("Id do pai: %d\n", vertices[j].pai);
                }
            }
        }

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

    Aresta grafo[n_pontos][n_pontos];//Matriz de adjacencias
    Ponto pontos[n_pontos];
    
    criaGrafo(file, n_pontos, grafo, pontos);// Cria grafo com base no arquivo input.txt

    preencheGrafo(n_pontos, grafo);//inicializa as arestas que nao foram contempladas em criaGrafo

    prim(n_pontos, grafo, pontos);

   /*for(int i = 0; i < n_pontos; i ++)
    for(int j = 0; j < n_pontos; j ++) {
        printf("%d %d\n", i, j);
        printf("(%.0f ,%.0f)->(%.0f, %.0f)\n", grafo[i][j].inicio.x, grafo[i][j].inicio.y, grafo[i][j].fim.x, grafo[i][j].fim.y);
        printf("--------------\n");
        printf("dist: %.2f\n", grafo[i][j].distancia);
    }*/

    for(int i = 0; i < n_pontos; i ++) {
        printf("%.0f %.0f\n", pontos[i].x, pontos[i].y);
        printf("%.2f\n", pontos[i].key);
        printf("%d\n", pontos[i].pai);
    }

    fclose (file);
}
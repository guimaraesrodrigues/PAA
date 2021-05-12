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
#include <math.h>
#include <time.h>

// Struct para representar o ponto
typedef struct Ponto {
//   int id; NAO TENHO CERTEZA DO USO DISSO
  float x, y;
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
void criaGrafo(FILE *file, int n_pontos, Aresta grafo[n_pontos][n_pontos]) {
    Ponto p;

	int ret = 0;
	int i = 0;
	char line_buffer[100]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e insere o valor na matriz de ajacências
	while(fgets(line_buffer, sizeof(line_buffer), file)) {
		ret = sscanf(line_buffer, "%f %f", &p.x, &p.y);
        
        if(ret < 1)
			continue;

        //Inicializa linha 0
        grafo[0][i].inicio.x = p.x;
        grafo[0][i].inicio.y = p.y;

        //Inicializa coluna 0 
		grafo[i][0].inicio.x = p.x;
        grafo[i][0].inicio.y = p.y;

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
                grafo[i][j] = inicializaAresta(grafo[i][j-1].fim, grafo[i][j].inicio);
                grafo[j][i].distancia =  grafo[i][j].distancia;
                
                grafo[j][i].inicio = grafo[i][j].fim;
                grafo[j][i].fim = grafo[i][j].inicio;
            } 

}

int main(int argc, char *argv[]) {
    FILE *file = NULL;

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
	
    char first_line[20];
    int n_pontos;
    fscanf(file, "%s", first_line); // Le a primeira linha do arquivo para saber quantos pares foram gerados
    sscanf(first_line, "%d", &n_pontos);//converte string para int

    Aresta grafo[n_pontos][n_pontos];

	criaGrafo(file, n_pontos, grafo);// Cria grafo com base no arquivo input.txt

    preencheGrafo(n_pontos, grafo);

   for(int i = 0; i < n_pontos; i ++)
    for(int j = 0; j < n_pontos; j ++) {
        printf("%d %d\n", i, j);
        printf("(%.0f ,%.0f)->(%.0f, %.0f)\n", grafo[i][j].inicio.x, grafo[i][j].inicio.y, grafo[i][j].fim.x, grafo[i][j].fim.y);
        printf("--------------\n");
        // printf("dist: %.2f\n", grafo[i][j].distancia);
    }

    fclose (file);
}
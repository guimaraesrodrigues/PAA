#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Struct para representar o ponto
typedef struct Ponto {
  int id;
  unsigned int x, y;
} Ponto;

typedef struct Aresta {
  Ponto inicio;
  Ponto fim;
  float distancia; 
} Aresta;


typedef struct Grafo {
   int vertices; 
   int arestas; 
   Aresta **aresta; 
} Grafo;

// Calcula a distância euclidiana entre p1 e p2
float dist(Ponto p1, Ponto p2) {
	return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// Cria uma matriz de adjacências baseado no arquivo passado como parâmetro
void criaGrafo(FILE *file, int n_pontos, Ponto grafo[n_pontos][n_pontos]) {
    Ponto p;

	int ret = 0;
	int i = 0;
	char line_buffer[100]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e insere o valor na matriz de ajacências
	while(fgets(line_buffer, sizeof(line_buffer), file)) {
		ret = sscanf(line_buffer, "%d %d", &p.x, &p.y);
        
        if(ret < 1)
			continue;

        //Inicializa coluna 0 e linhas y = i
		grafo[i][0].x = p.x;
        grafo[i][0].y = p.y;
        //Inicializa linha 0 e colunas y = i
        grafo[0][i].x = p.x;
        grafo[0][i].y = p.y;

		i++;
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

    Ponto grafo[n_pontos][n_pontos];

	criaGrafo(file, n_pontos, grafo);// Cria grafo com base no arquivo input.txt

    for (int i = 0; i < n_pontos; ++i)
        printf("%d %d\n", grafo[0][i].x, grafo[0][i].y);

    fclose (file);
}
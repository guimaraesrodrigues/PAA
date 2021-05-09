#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Calcula a distância euclidiana entre p1 e p2
double dist(unsigned int p1, unsigned int p2) {
	return sqrt( (p1 - p2)*(p1 - p2) +
				(p1 - p2)*(p1 - p2)
			);
}

// Cria uma matriz de adjacências baseado no arquivo passado como parâmetro
void criaGrafo(FILE *file, int n_pontos, unsigned int grafo[n_pontos][n_pontos]) {
    unsigned int x, y;

	int ret = 0;
	int i, j = 0;
	char line_buffer[100]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e insere o valor na matriz de ajacências
	while(fgets(line_buffer, sizeof(line_buffer), file)) {
		ret = sscanf(line_buffer, "%d %d", &x, &y);
        
        if(ret < 1)
			continue;

        if(i == j) {
            grafo[i][j] = 0;
            i++, j++;
            continue;
        }

        printf("%d %d\n", x, y);

		grafo[i][j] = dist(x, y);
		i++, j++;
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

    unsigned int grafo[n_pontos][n_pontos];

	criaGrafo(file, n_pontos, grafo);// Cria grafo com base no arquivo input.txt

    // for (int i = 0; i < n_pontos; ++i)
	// 	for (int j = i+1; j < n_pontos; ++j)
    //         printf("custo %d %d: %d\n", i, j, grafo[i][j]);

    fclose (file);
}
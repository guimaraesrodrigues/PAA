/**
 * Trabalho 01
 * 
 * Autores:
 * Eduardo Junior - 1458884
 * Rodrigo Guimarães - 1441990
 * 
 * **/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "structs.h"
#include "lista.h"

/**
 * Calcula area do triangulo formado por p1, p2, p3 
 * para definir a posicao de p3 em relacao a reta p1->p2
 * 
 **/
int calcArea(Ponto p1, Ponto p2, Ponto p3) {
    int area = abs(((p2.x - p1.x) * (p3.y - p1.y)) - 
           ((p2.y - p1.y) * (p3.x - p1.x)));
    
    if (area > 0)
        return 1;
    if (area < 0)
        return -1;
    return 0;
}

/* Calcula a distancia entre ponto e reta geometricamente */
int calcDist(Ponto p1, Ponto p2, Ponto p3) {
    int numerador = (
            (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x) *
            (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x)
        );
    int denominador = ((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    return fabs(sqrt(numerador)/sqrt(denominador));
}

// Cria uma lista pontos P baseado no arquivo passado como parâmetro
void createPointsList(FILE *file, Ponto* pontos) {
    Ponto p;

	int ret = 0;
	int n_lines = 0;
	char line_buffer[100]; //buffer a ser utilizado na leitura de cada linha

	// Percorre a stream, identifica as duas coordenadas na linha
	// e insere o valor no array pontos
	while(fgets(line_buffer, sizeof(line_buffer), file)) {
		ret = sscanf(line_buffer, "%d %d", &p.x, &p.y);
		if(ret < 1)
			continue;
		pontos[n_lines].x = p.x;
		pontos[n_lines].y = p.y;
		n_lines++;
	}
}



// A funcao  recursiveHull se utiliza primeiramente da formula de distancia entre ponto e reta para encontrar o po
// ponto que se encontra mais distante da reta definida pelos pontos passados como parâmetro da função.
// Ao encontrar o ponto pode-se fazer uma divisão na qual os pontos se encontram no conjuntos a esquerda ou direita
// dessa nova reta formada. Com esses novos conjuntos é traçado um triângulo entre a o ponto mais distante 
// a reta passada  os pontos internos a area desse triângulo são excluidos e os externos são adicionados ao 
// fecho convexo. Apos terminar a verificação o algoritimo cai no caso base da recursão onde adiciona os pontos 
// iniciais da reta, denominada pelo enunciado como P e Q, ao fecho convexo e o algoritmo é encerrado.

// podemos delimitar a complexidade desse algoritmo para a formula  3n x Logn +c (onde N e o numero de pontos na lista), logo podemos afirmar que o algoritmo
// é da ordem de O(nlogn), para afirmar tal complexidade assumimos que o algoritimo dividi igualmente em 2 subproblemas 
// com o mesmo tamanho (n/2). 

void recursiveHull(Ponto* pontos, Lista* hull, Ponto p, Ponto q, int n_pontos, int lado) {

    /*Definicao de variaveis locais*/
    int idx = -1;
    int max_dist = 0;

    /* Laco para encontrar o ponto mais distante da reta P-Q */
    for (int i = 0; i < n_pontos; i++)
    {
        int aux = calcDist(p, q, pontos[i]);
        if (calcArea(p, q, pontos[i]) == lado && aux > max_dist)
        {
            idx = i;
            max_dist = aux;
        }
    } 

    /* Caso base da recursão*/
    // quando o conjunto de pontos estiver para analise estiver vazio
    // adiciona os pontos p e q da reta separadora ao fecho convexo e encerra a recursao.
    if (idx == -1) {
        inserir_primeiro(hull, p);
        inserir_primeiro(hull, q);
        return;
    }

    //encontra o fecho convexo a esquerda da reta pq
    recursiveHull(pontos, hull, pontos[idx], p, n_pontos, -calcArea(pontos[idx], p, q));
    //encontra o fecho convexo a direita da reta pq
    recursiveHull(pontos, hull, pontos[idx], q, n_pontos, -calcArea(pontos[idx], q, p));

    return;
}


/*Entrada: <lista de pontos>*/
/* Saida  <pontos pertencentes ao fecho convexo> */



void quickHull(Ponto* pontos, int n_pontos, Lista* hull) {
   
    /* Variaveis para os pontos da reta P-Q */
    int menor_coord_idx = 0;
    int maior_coord_idx = 0;

    /* Laço para encontrar os pontos que formam a reta p-q */
    /* Os IF's comparam os pontos com menor e maior valor no atributo x */
    /* Os elseif's utilizam o criterio para caso haja empate entre pontos */ 
    for (int i = 1; i < n_pontos; i++) {
        if(pontos[i].x < pontos[menor_coord_idx].x)
            menor_coord_idx = i;
        else if (pontos[i].x == pontos[menor_coord_idx].x && pontos[i].y > pontos[menor_coord_idx].y)
            menor_coord_idx = i;

        if(pontos[i].x > pontos[maior_coord_idx].x)
            maior_coord_idx = i;
        else if (pontos[i].x == pontos[maior_coord_idx].x && pontos[i].y < pontos[maior_coord_idx].y)
            maior_coord_idx = i;
    }
    

    /* Chamada da funcao recursiva do HULL*/
    //encontra o fecho convexo a esquerda da reta pq
    recursiveHull(pontos, hull, pontos[menor_coord_idx], pontos[maior_coord_idx], n_pontos, 1);
    //encontra o fecho convexo a direita da reta pq
    recursiveHull(pontos, hull, pontos[menor_coord_idx], pontos[maior_coord_idx], n_pontos, -1);
}

int main(int argc, char *argv[]) {

    /* Definicao de variaveis*/
    FILE *file = NULL;

    /* Checa argumentos da linha de comando */
    if (argc != 2) {
        printf ("ERROR: you must specify file name!\n");
        return 1;
    }

    /* Abre o arquivo */
    file = fopen(argv[1], "r");
    if (!file) {
        perror ("File open error!\n");
        return 1;
    }
	
    /* Definicao de variaveis*/
    char first_line[20];
    int n_pontos;

    // Le a primeira linha do arquivo para saber quantos pares foram gerados
    fscanf(file, "%s", first_line); 

    //converte string para int
    sscanf(first_line, "%d", &n_pontos);

    // Defini um lista de pontos
    Ponto pontos[n_pontos];

    // Cria lista de pares com base no arquivo input.txt
	createPointsList(file, pontos);

    //fim da operação com arquivos
    fclose (file);

    //Cria a lista para a armazenar o fecho convexo
    Lista* hull = criar_lista();

    //Variavel para o calculo de tempo
    double qh_time;

    //Armazena o tempo antes do inicio da execucao do algoritmo
    clock_t beginQH = clock();

    // executa o algoritmo
    quickHull(pontos, n_pontos, hull);

    //Armazena o tempo apos o fim da execucao algoritmo
    clock_t endQH = clock();

    //tempo quickhull
	qh_time = (double)(endQH - beginQH) / CLOCKS_PER_SEC;

    // Cria o arquivo com o fecho convexo
    cria_arquivo(hull);

    //Imprime o tempo
    printf("%f", qh_time);

    // for (int i = 0; i < n_pontos; i++) {
    //     printf("%f, %f\n", pontos[i].x, pontos[i].y);
    // }
    return 0;
}
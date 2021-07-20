#ifndef STRUCTS_H
#define STRUCTS_H

// Struct para representar o ponto
typedef struct Ponto {
	float x, y;
} Ponto;

// Struct para representar um nó de uma lista
typedef struct No {
    Ponto p;
    No* proximo;
    No* anterior;
} No;

// Struct para representar uma lista
typedef struct Lista {
    int tamanho;
    No* nos;
} Lista;

#endif // STRUCTS_H
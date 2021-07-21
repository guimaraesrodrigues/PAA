#ifndef STRUCTS_H
#define STRUCTS_H

// Essas structs são utilizadas para armazenamento das informações obtidas ao decorrer do trabalho.


// Struct para representar o ponto
typedef struct Ponto {
	int x, y;
} Ponto;

// Struct para representar um nó de uma lista
typedef struct No {
    Ponto p;
    struct No* proximo;
} No;

// Struct para representar uma lista
typedef struct Lista {
    int tamanho;
    No* nos;
} Lista;

#endif // STRUCTS_H
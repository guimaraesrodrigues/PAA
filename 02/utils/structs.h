#ifndef STRUCTS_H
#define STRUCTS_H

// Struct para representar o ponto
typedef struct Ponto {
	float x, y;
} Ponto;
typedef struct Tripla {
	Ponto i;
    Ponto k;
    Ponto j;
    float distancia;
} Tripla;

#endif // STRUCTS_H
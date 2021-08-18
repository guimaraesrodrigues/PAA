#ifndef GIFTWRAPPING_H
#define GIFTWRAPPING_H

// Struct para representar o ponto
typedef struct Ponto Ponto;

void convexHull(int n_pontos, Ponto pontos[]);
void createPointsList(FILE *file, Ponto* pontos); 

#endif // LISTA_H
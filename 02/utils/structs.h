#ifndef STRUCTS_H
#define STRUCTS_H

// Estrutura Ponto:
// armazena um ponto do Grafo
// Utilizado para:
// - armazenar elementos do vector fecho convexo
// - armazenar elementos do vector ciclo
// - armazenar elementos do vector pontos_internos
// Utilização:
// - função removePontos();
// - função gravaCiclo();
// - função tsp();
typedef struct Ponto {
	float x, y;
} Ponto;

#endif // STRUCTS_H
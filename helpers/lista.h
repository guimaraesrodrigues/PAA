#ifndef LISTA_H
#define LISTA_H

//////////////////// STRUCTS ////////////////////
// Prototipos de struct para tipagem dos prototipos de função de uma lista encadeada;

// Struct para representar um nó de uma lista
typedef struct Lista Lista;

// Struct para representar uma lista
typedef struct No No;

// Struct para representar o ponto
typedef struct Ponto Ponto;

//////////////////// FUNÇÕES ////////////////////

// Função para criar uma struct lista com os valores inicializados
Lista* criar_lista();

// Função para inserir uma struct Ponto de uma lista, em um índice específico
void inserir(Lista* lista, Ponto p, int indice);

// Função para inserir uma struct Ponto em uma lista na primeira posição
void inserir_primeiro(Lista* lista, Ponto p);

// Função para remover uma struct Ponto de uma lista
Ponto remover(Lista* lista, Ponto* p);

// Função para apagar uma lista
void apagar_lista(Lista* lista);

// Função para imprimir lista
void imprimir_lista(Lista* lista);


// Função para criar arquivo com pontos do fecho convexo
void cria_arquivo(Lista* lista);
#endif // LISTA_H
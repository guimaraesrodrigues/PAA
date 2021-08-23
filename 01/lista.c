#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include "lista.h"
#include "structs.h"


// A estrutura de lista encadeada é utilizada para armazenar os pontos que são adicionados a fecho.
// Função auxiliar para criar um nó
static No* criar_no(No* proximo, Ponto p) {
    No* no = (No*) malloc(sizeof(No));
    no->proximo = proximo;
    no->p = p;
    return no;
}

// Função para desalocar um nó da memória
static void remover_no(No* no) {
    free(no->proximo);
}

// Função auxiliar para verificar se a lista está vazia
static int vazia(Lista* lista) {
    if (lista->nos == NULL) return 1;
    return 0;
}

//Cria o cabecalho da lista
Lista* criar_lista() {
    Lista* lista = (Lista*) malloc(sizeof(Lista));
    lista->nos = NULL;
    lista->tamanho = 0;
    return lista;
}

//insere um elemento no inicio da fila
void inserir_primeiro(Lista* lista, Ponto p) {
    if(lista->nos == NULL) {
        lista->nos = criar_no(NULL, p);
    } else {
        No* tmp = criar_no(lista->nos, p);
        lista->nos = tmp;
    }      
    lista->tamanho++;
}

// TODO: Não está inserindo na posição correta, mas se utilizar o índice = 0,
// ela insere um elemento na primeira posição da lista.
void inserir(Lista* lista, Ponto ponto, int indice) {
    // Verificação de tamanho
    if (indice > lista->tamanho) {
        const int MAX_SIZE = 128;
        char msg[MAX_SIZE];
        snprintf(msg, MAX_SIZE, "Não foi possível inserir no índice \'%d\' porque o tamanho da lista é \'%d\'", indice, lista->tamanho);
        puts(msg);
        return;
    }

    if(vazia(lista)) {
        lista->nos = criar_no(NULL, ponto);
    } else {
        if(indice == 0) {
            inserir_primeiro(lista, ponto);
        } else {
            No* atual = lista->nos;
            int contador = 0;
            while(1) {
                if(contador == indice) {
                    No* novo = criar_no(atual, ponto);

                    break;
                }
                atual = atual->proximo;
                contador++;
            }
        }
    }
    lista->tamanho++;
}

// imprime elementos da lista
void imprimir_lista(Lista* lista) {
    int i = 0;
    No* aux = lista->nos;
    while(aux != NULL) {
        printf("Indice %d: (%d, %d)\n", i, aux->p.x, aux->p.y);
        i++;
        aux = aux->proximo;
    }
}

// libera memoria 
void apagar_lista(Lista* lista) {
    free(lista);
}

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

// Imprime informações de uma struct No
static void print_no(No* no) {
    const int MAX_SIZE = 16;
    char prox[MAX_SIZE];
    if(no->proximo == NULL) {
        snprintf(prox, MAX_SIZE, "NULL");
    } else {
        snprintf(prox, MAX_SIZE, "0x%06X", no->proximo);
    }
    printf("Nó<0x%06X>(proximo=%s, p=Ponto(%d, %d))\n", no, prox, no->p.x, no->p.y);
}

Lista* criar_lista() {
    Lista* lista = (Lista*) malloc(sizeof(Lista));
    lista->nos = NULL;
    lista->tamanho = 0;
    return lista;
}

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

// Ponto remover(Lista* lista, Ponto* ponto) {
//     No* aux = lista->nos;
//     while(aux != NULL) {
//         Ponto* _ponto = &lista->nos->p;
//         if(_ponto->x == ponto->x && _ponto->y == ponto->y){
//             No* removido = lista->nos;
//             lista->nos = removido->proximo;
//             return removido->p;
//         }
//         aux = aux->proximo;
//     }
//     lista->tamanho--;
// }

// imprime elementos da lista
void imprimir_lista(Lista* lista) {
    int i = 0;
    No* aux = lista->nos;
    while(aux != NULL) {
        printf("Indice %d: ", i);
        print_no(aux);
        i++;
        aux = aux->proximo;
    }
}

// cria arquivo com as coordenadas do fecho convexo
void cria_arquivo(Lista* lista) {
    FILE *fp = NULL;
    fp  = fopen ("fecho.txt", "w");
    
    if (fp == NULL)
	{
		fprintf(stderr, "Falha ao criar input.txt.\n");
		return;
	}

    No* aux = lista->nos;
    while(aux != NULL) {
        Ponto* ponto = &aux->p;
        fprintf(fp, "%d %d\n", ponto->x, ponto->y);
        aux = aux->proximo;
    }
    fclose (fp);
}

// libera memoria 
void apagar_lista(Lista* lista) {
    free(lista);
}

// int main() {
//     Lista* l = criar_lista();
//     Ponto p0 = {0.0, 0.0};
//     Ponto p1 = {0.0, 1.0};
//     Ponto p2 = {1.0, 0.0};
//     Ponto p3 = {1.0, 1.0};

//     inserir(l, p0);
//     inserir(l, p1);
//     inserir(l, p2);
//     inserir(l, p3);

//     imprimir_lista(l);

//     Ponto rem = remover(l, &p0);
//     printf("No removido: Ponto(%f, %f)", rem.x, rem.y);

//     imprimir_lista(l);
// }
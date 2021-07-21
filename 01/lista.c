#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "structs.h"

// Função auxiliar para criar um nó
static No* criar_no(No* proximo, Ponto p) {
    No* no = (No*) malloc(sizeof(No));
    no->anterior = NULL;
    no->proximo = proximo;
    no->p = p;
    return no;
}

// Função para desalocar um nó da memória
static void remover_no(No* no) {
    free(no->anterior);
    free(no->proximo);
}

Lista* criar_lista() {
    Lista* lista = (Lista*) malloc(sizeof(Lista));
    lista->nos = NULL;
    lista->tamanho = 0;
    return lista;
}

void inserir(Lista* lista, Ponto ponto) {
    if(lista->nos == NULL) {
        lista->nos = criar_no(NULL, ponto);
    } else {
        No* tmp = criar_no(lista->nos, ponto);
        lista->nos = tmp;
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

void imprimir_lista(Lista* lista) {
    int i = 0;
    No* aux = lista->nos;
    while(aux != NULL) {
        Ponto* ponto = &aux->p;
        printf("Indice %d: Ponto(%d, %d)\n", i, ponto->x, ponto->y);
        i++;
        aux = aux->proximo;
    }
}

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
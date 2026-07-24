#include "fila.h"
#include <stdio.h>

void fila_init(Fila* f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

int fila_vazia(Fila* f) {
    return f->tamanho == 0;
}

void enfileirar(Fila* f, PCB* p) {
    if (f->tamanho == MAX_FILA) {
        printf("ERRO: fila cheia\n");
        return;
    }
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->tamanho++;
}

PCB* desenfileirar(Fila* f) {
    if (fila_vazia(f)) return NULL;
    PCB* p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->tamanho--;
    return p;
}

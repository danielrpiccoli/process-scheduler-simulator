#ifndef FILA_H
#define FILA_H

#include "pcb.h"

#define MAX_FILA 100

typedef struct {
    PCB* itens[MAX_FILA];
    int inicio;
    int fim;
    int tamanho;
} Fila;

void fila_init(Fila* f);
int  fila_vazia(Fila* f);
void enfileirar(Fila* f, PCB* p);
PCB* desenfileirar(Fila* f);

#endif

#ifndef ESCALONADOR_H
#define ESCALONADOR_H

#include "fila.h"

// Agrupa todo o estado do sistema em uma struct
typedef struct {
    Fila alta; // Fila de prontos - alta prioridade
    Fila baixa; // Fila de prontos - baixa prioridade
    Fila io_disco; // Bloqueados aguardando disco
    Fila io_fita; // Bloqueados aguardando fita
    Fila io_impressora; // Bloqueados aguardando impressora

    int clock; // Tick atual
    int processos_terminados;
    int total_processos;
} Escalonador;

void escalonador_init(Escalonador* e, int total_processos);
void simular(Escalonador* e, PCB* todos[], int n);

#endif

#ifndef PROCESSO_H
#define PROCESSO_H

#include "pcb.h"

// Cria e preenche um PCB com valores (parcialmente aleatorios)
PCB* criar_processo(int pid, int ppid, int instante_chegada);

// Nome legivel do tipo de I/O (para os logs)
const char* nome_io(TipoIO tipo);

#endif

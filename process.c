#include "processo.h"
#include "config.h"
#include <stdlib.h>

PCB* criar_processo(int pid, int ppid, int instante_chegada) {
    PCB* p = malloc(sizeof(PCB));

    p->pid = pid;
    p->ppid = ppid;
    p->prioridade = 0; // Todo processo novo entra na prioridade alta
    p->status = PRONTO;

    p->tempo_servico = SERVICO_MIN + rand() % (SERVICO_MAX - SERVICO_MIN + 1);
    p->tempo_executado = 0;

    p->instante_chegada = instante_chegada;
    p->chegou = 0; // Ainda nao entrou no sistema

    // Decide se esse processo vai fazer I/O
    if (rand() % 100 < CHANCE_IO) {
        // Sorteia o tipo de I/O (1 = disco, 2 = fita, 3 = impressora)
        p->tipo_io = (TipoIO)(1 + rand() % 3);
        // Define após quantos ticks de CPU ele pede o I/O
        p->instante_io = 1 + rand() % p->tempo_servico;
    } else {
        p->tipo_io = SEM_IO;
        p->instante_io = -1;        // Nunca pede
    }

    p->tempo_restante_io = 0;

    return p;
}

const char* nome_io(TipoIO tipo) {
    switch (tipo) {
        case DISCO:      return "Disco";
        case FITA:       return "Fita";
        case IMPRESSORA: return "Impressora";
        default:         return "Nenhum";
    }
}

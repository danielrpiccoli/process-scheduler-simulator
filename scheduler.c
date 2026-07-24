#include "escalonador.h"
#include "config.h"
#include "processo.h"
#include <stdio.h>

void escalonador_init(Escalonador* e, int total_processos) {
    fila_init(&e->alta);
    fila_init(&e->baixa);
    fila_init(&e->io_disco);
    fila_init(&e->io_fita);
    fila_init(&e->io_impressora);
    e->clock = 0;
    e->processos_terminados = 0;
    e->total_processos = total_processos;
}

// Funcoes auxiliares (static = visiveis só nesse arquivo)

// Coloca um processo na fila de I/O certa e inicia o contador de duração
static void enviar_para_io(Escalonador* e, PCB* p) {
    p->status = BLOQUEADO;
    switch (p->tipo_io) {
        case DISCO:
            p->tempo_restante_io = DURACAO_DISCO;
            enfileirar(&e->io_disco, p);
            break;
        case FITA:
            p->tempo_restante_io = DURACAO_FITA;
            enfileirar(&e->io_fita, p);
            break;
        case IMPRESSORA:
            p->tempo_restante_io = DURACAO_IMPRESSORA;
            enfileirar(&e->io_impressora, p);
            break;
        default:
            break;
    }
    printf("[clock %2d] PID %d -> I/O (%s)\n",
           e->clock, p->pid, nome_io(p->tipo_io));
}

/* Decrementa o I/O de uma fila. Quem termina volta para a fila de prontos.
   destino_alta = 1 -> volta para ALTA
   destino_alta = 0 -> volta para BAIXA */
static void atualizar_fila_io(Escalonador* e, Fila* fila_io, int destino_alta) {
    int n = fila_io->tamanho;
    for (int i = 0; i < n; i++) {
        PCB* p = desenfileirar(fila_io);
        p->tempo_restante_io--;

        if (p->tempo_restante_io <= 0) {
            // I/O concluido. Esse processo nao pede I/O de novo
            p->tipo_io = SEM_IO;
            p->instante_io = -1;
            p->status = PRONTO;

            if (destino_alta) {
                enfileirar(&e->alta, p);
                printf("[clock %2d] PID %d <- I/O concluido, vai p/ ALTA\n",
                       e->clock, p->pid);
            } else {
                enfileirar(&e->baixa, p);
                printf("[clock %2d] PID %d <- I/O concluido, vai p/ BAIXA\n",
                       e->clock, p->pid);
            }
        } else {
            // Ainda nao terminou, devolve para a mesma fila
            enfileirar(fila_io, p);
        }
    }
}

// Pega o próximo processo a executar. Alta tem prioridade sobre baixa
static PCB* selecionar_processo(Escalonador* e) {
    if (!fila_vazia(&e->alta))  return desenfileirar(&e->alta);
    if (!fila_vazia(&e->baixa)) return desenfileirar(&e->baixa);
    return NULL;
}

void simular(Escalonador* e, PCB* todos[], int n) {
    printf("\n===== INICIO DA SIMULACAO =====\n\n");

    while (e->processos_terminados < e->total_processos) {

        /* 1. Chegada: processo entra assim que o clock alcança sua chegada.
           Usa flag 'chegou' + '<=' para nunca pular um processo cuja
           chegada caia num tick que o clock pulou por causa do quantum. */
        for (int i = 0; i < n; i++) {
            if (!todos[i]->chegou && todos[i]->instante_chegada <= e->clock) {
                todos[i]->chegou = 1;
                enfileirar(&e->alta, todos[i]);
                printf("[clock %2d] PID %d criado -> fila ALTA\n",
                       e->clock, todos[i]->pid);
            }
        }

        // 2. Ataualiza I/O (disco volta para baixa. Fita e impressora para alta)
        atualizar_fila_io(e, &e->io_disco, 0);       // 0 = baixa
        atualizar_fila_io(e, &e->io_fita, 1);        // 1 = alta
        atualizar_fila_io(e, &e->io_impressora, 1);  // 1 = alta

        // 3. Seleciona processo para a CPU
        PCB* atual = selecionar_processo(e);

        if (atual == NULL) {
            // CPU ociosa: ninguem pronto, mas ainda tem gente em I/O
            e->clock++;
            continue;
        }

        // 4. Executa respeitando o quantum
        atual->status = EXECUTANDO;
        int quantum_usado = 0;
        printf("[clock %2d] PID %d entra na CPU (servico %d/%d)\n",
               e->clock, atual->pid, atual->tempo_executado, atual->tempo_servico);

        while (quantum_usado < QUANTUM) {
            atual->tempo_executado++;
            quantum_usado++;
            e->clock++;

            // 5a. Terminou o serviço?
            if (atual->tempo_executado >= atual->tempo_servico) {
                atual->status = TERMINADO;
                e->processos_terminados++;
                printf("[clock %2d] PID %d TERMINOU\n", e->clock, atual->pid);
                atual = NULL;
                break;
            }

            // 5b. Chegou a hora de pedir I/O?
            if (atual->tipo_io != SEM_IO &&
                atual->tempo_executado == atual->instante_io) {
                enviar_para_io(e, atual);
                atual = NULL;
                break;
            }
        }

        // 5c. Estourou o quantum sem terminar nem pedir I/O -> Preempção
        if (atual != NULL) {
            atual->status = PRONTO;
            enfileirar(&e->baixa, atual);
            printf("[clock %2d] PID %d sofreu PREEMPCAO -> fila BAIXA\n",
                   e->clock, atual->pid);
        }
    }

    printf("\n===== FIM DA SIMULACAO (clock final: %d) =====\n", e->clock);
}

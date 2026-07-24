#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "config.h"
#include "processo.h"
#include "escalonador.h"

int main() {
    srand(time(NULL));

    int n = MAX_PROCESSOS;
    PCB* todos[MAX_PROCESSOS];

    // Cria os processos com chegadas espalhadas no tempo
    int chegada = 0;
    for (int i = 0; i < n; i++) {
        todos[i] = criar_processo(i + 1, 1, chegada);
        chegada += rand() % 4;   // Proximo chega de 0 a 3 ticks depois
    }

    // Mostra a tabela de processos antes de simular
    printf("===== PROCESSOS CRIADOS =====\n");
    for (int i = 0; i < n; i++) {
        printf("PID %d | chegada=%d | servico=%d | io=%s | pede_io_apos=%d\n",
               todos[i]->pid, todos[i]->instante_chegada,
               todos[i]->tempo_servico, nome_io(todos[i]->tipo_io),
               todos[i]->instante_io);
    }

    // Inicializa e roda uma vez
    Escalonador e;
    escalonador_init(&e, n);
    simular(&e, todos, n);

    // Libera memoria
    for (int i = 0; i < n; i++) free(todos[i]);

    return 0;
}

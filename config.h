#ifndef CONFIG_H
#define CONFIG_H

// Premissas do simulador

#define MAX_PROCESSOS   5   // Limite maximo de processos criados
#define QUANTUM         3    // Fatia de tempo, em ticks

// Faixa do tempo de servico (CPU) de cada processo
#define SERVICO_MIN     2
#define SERVICO_MAX     10

// Duracao de cada tipo de I/O, em ticks
#define DURACAO_DISCO       4
#define DURACAO_FITA        3
#define DURACAO_IMPRESSORA  5

// Probabilidade de um processo solicitar I/O
#define CHANCE_IO       60

#endif

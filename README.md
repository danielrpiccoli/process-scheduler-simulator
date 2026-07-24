# Escalonador de Processos — Round Robin com Feedback

Simulador de escalonamento de processos escrito em **C**, implementando a estratégia de seleção **Round Robin (Circular) com Feedback**.

Trabalho 2 da disciplina **ICP246 — Arquitetura de Computadores e Sistemas Operacionais** (UFRJ — Instituto de Computação, 2026-1).

---

## Visão geral

O simulador modela a gerência de processos de curto prazo de um sistema operacional. A cada *tick* do relógio, ele cria processos, disputa a CPU entre eles respeitando uma fatia de tempo (quantum), aplica preempção, bloqueia processos que solicitam I/O e os devolve às filas de prontos conforme o tipo de dispositivo utilizado.

A estratégia reproduz, em forma simplificada, o escalonamento do **UNIX tradicional**: Feedback com múltiplas filas de prioridade e Round Robin internamente em cada fila.

### Principais características

- **PCB (Process Control Block)** com identificação (PID, PPID), estado e atributos de escalonamento.
- **Cinco filas:** duas de prontos (alta e baixa prioridade) e três de I/O (disco, fita e impressora).
- **Preempção por quantum:** processos que esgotam a fatia sem terminar são rebaixados para a fila de baixa prioridade.
- **Feedback por tipo de I/O:**
  - Disco → retorna à fila de **baixa** prioridade
  - Fita magnética → retorna à fila de **alta** prioridade
  - Impressora → retorna à fila de **alta** prioridade
- **Tempos aleatórios** de serviço e de I/O para cada processo.
- **Log detalhado** de cada evento (criação, entrada na CPU, preempção, bloqueio, retorno de I/O e término).

---

## Estrutura do projeto

| Arquivo | Responsabilidade |
|---|---|
| `config.h` | Premissas e parâmetros numéricos da simulação |
| `pcb.h` | Estrutura do PCB e enumerações de estado e tipo de I/O |
| `fila.h` / `fila.c` | Estrutura genérica de fila circular (FIFO) |
| `processo.h` / `processo.c` | Criação de processos com atributos aleatórios |
| `escalonador.h` / `escalonador.c` | Núcleo: laço de simulação Round Robin com Feedback |
| `main.c` | Montagem dos processos e disparo da simulação |

---

## Como compilar e executar

O projeto depende apenas de um compilador C padrão (GCC ou Clang) e da biblioteca padrão da linguagem. Não há dependências externas.

### Linux / macOS

```bash
gcc -Wall main.c fila.c processo.c escalonador.c -o escalonador
./escalonador
```

Com Clang, basta trocar `gcc` por `clang`.

### Windows

Com o **MinGW** (GCC para Windows):

```bash
gcc -Wall main.c fila.c processo.c escalonador.c -o escalonador.exe
escalonador.exe
```

Com o compilador da **Microsoft (MSVC)**, em um *Developer Command Prompt*:

```bash
cl main.c fila.c processo.c escalonador.c /Fe:escalonador.exe
escalonador.exe
```

> A cada execução os tempos de serviço e de I/O são sorteados novamente (o gerador é semeado com `srand(time(NULL))`), de modo que a saída varia entre execuções.

---

## Ajustando as premissas

Todos os parâmetros da simulação estão centralizados em `config.h` e podem ser alterados antes de compilar:

```c
#define MAX_PROCESSOS   10   /* limite maximo de processos criados */
#define QUANTUM         3    /* fatia de tempo, em ticks           */

#define SERVICO_MIN     5    /* tempo de servico minimo            */
#define SERVICO_MAX     20   /* tempo de servico maximo            */

#define DURACAO_DISCO       4
#define DURACAO_FITA        3
#define DURACAO_IMPRESSORA  5

#define CHANCE_IO       60   /* probabilidade (%) de solicitar I/O */
```

Por exemplo, aumentar o `QUANTUM` aproxima o comportamento do FCFS; reduzi-lo aumenta a frequência de preempções.

---

## Exemplo de saída

```
===== PROCESSOS CRIADOS =====
PID 1 | chegada=0 | servico=6 | io=Fita | pede_io_apos=3
PID 2 | chegada=3 | servico=19 | io=Fita | pede_io_apos=17
...

===== INICIO DA SIMULACAO =====

[clock  0] PID 1 criado -> fila ALTA
[clock  0] PID 1 entra na CPU (servico 0/6)
[clock  3] PID 1 -> I/O (Fita)
[clock  3] PID 2 entra na CPU (servico 0/19)
[clock  6] PID 2 sofreu PREEMPCAO -> fila BAIXA
...
[clock  9] PID 1 <- I/O concluido, vai p/ ALTA
...

===== FIM DA SIMULACAO (clock final: NN) =====
```

Cada linha registra o instante do relógio e o evento de escalonamento correspondente.

---

## Referências

- BASTOS, Valeria M. *Sistemas Operacionais I — Unidade II: Processos.* UFRJ/IM/DCC.
- BASTOS, Valeria M. *Sistemas Operacionais I — Unidade III: Escalonamento de Processos.* UFRJ/IM/DCC.
- STALLINGS, William. *Operating Systems: Internals and Design Principles.* Pearson.
- TANENBAUM, A. S.; BOS, H. *Modern Operating Systems.* Pearson.
- SILBERSCHATZ, A.; GALVIN, P. B.; GAGNE, G. *Operating System Concepts.* Wiley.

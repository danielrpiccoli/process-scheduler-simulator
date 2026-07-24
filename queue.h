#ifndef QUEUE_H
#define QUEUE_H

#include "pcb.h"

#define MAX_QUEUE 100

typedef struct {
    PCB* items[MAX_QUEUE];
    int start;
    int end;
    int size;
} Queue;

void queue_init(Queue* q);
int  queue_empty(Queue* q);
void enqueue(Queue* q, PCB* p);
PCB* dequeue(Queue* q);

#endif
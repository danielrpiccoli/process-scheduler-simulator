#include "queue.h"
#include <stdio.h>

void queue_init(Queue* q) {
    q->start = 0;
    q->end = 0;
    q->size = 0;
}

int queue_empty(Queue* q) {
    return q->size == 0;
}

void enqueue(Queue* q, PCB* p) {
    if (q->size == MAX_QUEUE) {
        printf("ERROR: queue full\n");
        return;
    }
    q->items[q->end] = p;
    q->end = (q->end + 1) % MAX_QUEUE;
    q->size++;
}

PCB* dequeue(Queue* q) {
    if (queue_empty(q)) return NULL;
    PCB* p = q->items[q->start];
    q->start = (q->start + 1) % MAX_QUEUE;
    q->size--;
    return p;
}
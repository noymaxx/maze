#ifndef QUEUE_H
#define QUEUE_H

// standard lib 
#include <stdbool.h>

// ======== STRUCT ========

typedef struct {
  int *data;      
  int front;     
  int rear;       
  int capacity;   
  int size;       
} Queue;

// ======= FUNCTIONS ===========

Queue* create_queue(int capacity);

bool enqueue(Queue* queue, int value);

bool dequeue(Queue* queue, int* value);

bool is_queue_empty(const Queue* queue);

void free_queue(Queue* queue);

#endif
#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

// create queue
Queue* create_queue(int capacity) {
  Queue* queue;
  
  if (capacity <= 0) {
    fprintf(stderr, "[ERROR]: invalid queue capacity %d\n", capacity);
    return NULL;
  }
  
  // allocate queue structure
  queue = malloc(sizeof(Queue));
  if (!queue) {
    fprintf(stderr, "[ERROR]: failed to allocate queue\n");
    return NULL;
  }

  queue->data = malloc(capacity * sizeof(int));
  if (!queue->data) {
    fprintf(stderr, "[ERROR]: failed to allocate queue data\n");
    free(queue);
    return NULL;
  }

  // initializing fields
  queue->front = 0;
  queue->rear = -1;
  queue->capacity = capacity;
  queue->size = 0;
  
  return queue;
}

// enqueue
bool enqueue(Queue* queue, int value) {
  if (!queue) {
      return false;
  }

  if (queue->size >= queue->capacity) {
    fprintf(stderr, "Error: queue is full\n");
    return false;
  }

  queue->rear = (queue->rear + 1) % queue->capacity;
  
  queue->data[queue->rear] = value;
  queue->size++;
  
  return true;
}

// dequeue
bool dequeue(Queue* queue, int* value) {
    if (!queue || !value) {
        return false;
    }

    if (queue->size == 0) {
        return false;
    }

    *value = queue->data[queue->front];

    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    
    return true;
}

// is empty
bool is_queue_empty(const Queue* queue) {
  if (!queue) {
    return true;
  }
  return (queue->size == 0);
}

// free queue
void free_queue(Queue* queue) {
  if (!queue) {
    return;
  }
  
  if (queue->data) {
    free(queue->data);
  }
  free(queue);
}
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>

// Node for priority queue
typedef struct {
  int index;      // maze cell index
  int priority;   // fscore for A*
} PQNode;

// Priority Queue (min-heap)
typedef struct {
  PQNode* nodes;
  int size;
  int capacity;
} PriorityQueue;

// Create priority queue
PriorityQueue* create_pq(int capacity);

// Insert with priority
void pq_push(PriorityQueue* pq, int index, int priority);

// Remove and return minimum priority element
bool pq_pop(PriorityQueue* pq, int* index);

// Check if empty
bool pq_is_empty(PriorityQueue* pq);

// Free memory
void free_pq(PriorityQueue* pq);

#endif

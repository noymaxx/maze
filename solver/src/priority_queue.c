#include "priority_queue.h"
#include <stdlib.h>
#include <stdio.h>

// Helper functions for heap operations
static void swap_nodes(PQNode* a, PQNode* b) {
  PQNode temp = *a;
  *a = *b;
  *b = temp;
}

static void heapify_up(PriorityQueue* pq, int idx) {
  while (idx > 0) {
    int parent = (idx - 1) / 2;
    if (pq->nodes[idx].priority >= pq->nodes[parent].priority) {
      break;
    }
    swap_nodes(&pq->nodes[idx], &pq->nodes[parent]);
    idx = parent;
  }
}

static void heapify_down(PriorityQueue* pq, int idx) {
  while (1) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < pq->size && pq->nodes[left].priority < pq->nodes[smallest].priority) {
      smallest = left;
    }
    if (right < pq->size && pq->nodes[right].priority < pq->nodes[smallest].priority) {
      smallest = right;
    }

    if (smallest == idx) {
      break;
    }

    swap_nodes(&pq->nodes[idx], &pq->nodes[smallest]);
    idx = smallest;
  }
}

PriorityQueue* create_pq(int capacity) {
  PriorityQueue* pq = malloc(sizeof(PriorityQueue));
  if (!pq) {
    return NULL;
  }

  pq->nodes = malloc(sizeof(PQNode) * capacity);
  if (!pq->nodes) {
    free(pq);
    return NULL;
  }

  pq->size = 0;
  pq->capacity = capacity;
  return pq;
}

void pq_push(PriorityQueue* pq, int index, int priority) {
  if (pq->size >= pq->capacity) {
    return;  // queue full
  }

  pq->nodes[pq->size].index = index;
  pq->nodes[pq->size].priority = priority;
  heapify_up(pq, pq->size);
  pq->size++;
}

bool pq_pop(PriorityQueue* pq, int* index) {
  if (pq->size == 0) {
    return false;
  }

  *index = pq->nodes[0].index;
  pq->size--;

  if (pq->size > 0) {
    pq->nodes[0] = pq->nodes[pq->size];
    heapify_down(pq, 0);
  }

  return true;
}

bool pq_is_empty(PriorityQueue* pq) {
  return pq->size == 0;
}

void free_pq(PriorityQueue* pq) {
  if (pq) {
    free(pq->nodes);
    free(pq);
  }
}

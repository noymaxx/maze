#include "solver.h"
#include "queue.h"
#include <string.h>

// reconstruct path from parent array and return path length
static int reconstruct_path(Maze* maze, const int* parent, int start_idx, int finish_idx) {
  int current;
  int path_length = 0;

  // start from finish
  current = finish_idx;

  while (current != start_idx) {
    if (current != start_idx && current != finish_idx) {
      maze->grid[current] = 'o';
    }
    path_length++;
    current = parent[current];
  }
  maze->grid[start_idx] = 'o';
  maze->grid[finish_idx] = 'o';
  path_length++;  // count start

  return path_length;
}

// count visited cells
static int count_visited(const bool* visited, int size) {
  int count = 0;
  int i;
  for (i = 0; i < size; i++) {
    if (visited[i]) {
      count++;
    }
  }
  return count;
}

// bfs - main algo
bool solve_bfs(Maze* maze) {
  bool* visited;      
  int* parent;        
  Queue* queue;       
  int start_idx;      
  int finish_idx;     
  int current_idx;    
  int neighbor_idx;   
  int x, y;           
  int nx, ny;         
  int i;              
  bool found;      
  
  int dx[] = {0, 0, -1, 1};
  int dy[] = {-1, 1, 0, 0};
  
  // validate input
  if (!maze || !maze->grid) {
    fprintf(stderr, "Error: invalid maze in solve_bfs\n");
    return false;
  }
  
  // calculate start and finish idx
  start_idx = IDX(0, 0, maze->width);
  finish_idx = IDX(maze->width - 1, maze->height - 1, maze->width);
  
  // allocate aux arr
  visited = calloc(maze->width * maze->height, sizeof(bool));
  if (!visited) {
    fprintf(stderr, "Error: failed to allocate visited array\n");
    return false;
  }

  parent = malloc(maze->width * maze->height * sizeof(int));
  if (!parent) {
    fprintf(stderr, "Error: failed to allocate parent array\n");
    free(visited);
    return false;
  }

  for (i = 0; i < maze->width * maze->height; i++) {
    parent[i] = -1;
  }
  
  // create queue
  queue = create_queue(maze->width * maze->height);
  if (!queue) {
    fprintf(stderr, "Error: failed to create queue\n");
    free(visited);
    free(parent);
    return false;
  }
  
  // init bfs
  visited[start_idx] = true;
  enqueue(queue, start_idx);
  
  // bfs loop
  found = false;
  
  while (!is_queue_empty(queue)) {
    if (!dequeue(queue, &current_idx)) {
      break;  
    }
    
    if (current_idx == finish_idx) {
      found = true;
      break;  
    }
    x = current_idx % maze->width;
    y = current_idx / maze->width;
    
    for (i = 0; i < 4; i++) {
      nx = x + dx[i];
      ny = y + dy[i];
      
      if (!IS_VALID_POS(nx, ny, maze->width, maze->height)) {
          continue;  
      }
      neighbor_idx = IDX(nx, ny, maze->width);
      
      if (visited[neighbor_idx]) {
          continue;  
      }
      
      if (maze->grid[neighbor_idx] != '*') {
          continue;
      }
      visited[neighbor_idx] = true;
      parent[neighbor_idx] = current_idx;
      enqueue(queue, neighbor_idx);
    }
  }

  // results
  if (found) {
    reconstruct_path(maze, parent, start_idx, finish_idx);
  }

  // cleanup
  free(visited);
  free(parent);
  free_queue(queue);

  return found;
}

// bfs with statistics
bool solve_bfs_stats(Maze* maze, SolverStats* stats) {
  bool* visited;
  int* parent;
  Queue* queue;
  int start_idx;
  int finish_idx;
  int current_idx;
  int neighbor_idx;
  int x, y;
  int nx, ny;
  int i;
  bool found;
  clock_t start_time, end_time;

  int dx[] = {0, 0, -1, 1};
  int dy[] = {-1, 1, 0, 0};

  // validate input
  if (!maze || !maze->grid || !stats) {
    fprintf(stderr, "Error: invalid input in solve_bfs_stats\n");
    return false;
  }

  // initialize stats
  stats->algorithm = "BFS";
  stats->cells_visited = 0;
  stats->path_length = 0;
  stats->time_ms = 0.0;

  start_time = clock();

  // calculate start and finish idx
  start_idx = IDX(0, 0, maze->width);
  finish_idx = IDX(maze->width - 1, maze->height - 1, maze->width);

  // allocate aux arr
  visited = calloc(maze->width * maze->height, sizeof(bool));
  if (!visited) {
    fprintf(stderr, "Error: failed to allocate visited array\n");
    return false;
  }

  parent = malloc(maze->width * maze->height * sizeof(int));
  if (!parent) {
    fprintf(stderr, "Error: failed to allocate parent array\n");
    free(visited);
    return false;
  }

  for (i = 0; i < maze->width * maze->height; i++) {
    parent[i] = -1;
  }

  // create queue
  queue = create_queue(maze->width * maze->height);
  if (!queue) {
    fprintf(stderr, "Error: failed to create queue\n");
    free(visited);
    free(parent);
    return false;
  }

  // init bfs
  visited[start_idx] = true;
  enqueue(queue, start_idx);

  // bfs loop
  found = false;

  while (!is_queue_empty(queue)) {
    if (!dequeue(queue, &current_idx)) {
      break;
    }

    if (current_idx == finish_idx) {
      found = true;
      break;
    }
    x = current_idx % maze->width;
    y = current_idx / maze->width;

    for (i = 0; i < 4; i++) {
      nx = x + dx[i];
      ny = y + dy[i];

      if (!IS_VALID_POS(nx, ny, maze->width, maze->height)) {
          continue;
      }
      neighbor_idx = IDX(nx, ny, maze->width);

      if (visited[neighbor_idx]) {
          continue;
      }

      if (maze->grid[neighbor_idx] != '*') {
          continue;
      }
      visited[neighbor_idx] = true;
      parent[neighbor_idx] = current_idx;
      enqueue(queue, neighbor_idx);
    }
  }

  // collect statistics
  stats->cells_visited = count_visited(visited, maze->width * maze->height);

  // results
  if (found) {
    stats->path_length = reconstruct_path(maze, parent, start_idx, finish_idx);
  }

  end_time = clock();
  stats->time_ms = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;

  // cleanup
  free(visited);
  free(parent);
  free_queue(queue);

  return found;
}
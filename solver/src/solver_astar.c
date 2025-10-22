#include "solver.h"
#include "priority_queue.h"
#include <string.h>
#include <limits.h>

// Manhattan distance heuristic
static int heuristic(int x, int y, int goal_x, int goal_y) {
  int dx = (x > goal_x) ? (x - goal_x) : (goal_x - x);
  int dy = (y > goal_y) ? (y - goal_y) : (goal_y - y);
  return dx + dy;
}

// reconstruct path from parent array and return path length
static int reconstruct_path_astar(Maze* maze, const int* parent, int start_idx, int finish_idx) {
  int current = finish_idx;
  int path_length = 0;

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

// count cells in closed set
static int count_closed(const bool* in_closed, int size) {
  int count = 0;
  int i;
  for (i = 0; i < size; i++) {
    if (in_closed[i]) {
      count++;
    }
  }
  return count;
}

// A* - main algo
bool solve_astar(Maze* maze) {
  int* g_score;        // cost from start to current
  int* f_score;        // estimated total cost (g + h)
  int* parent;
  bool* in_closed;
  PriorityQueue* open_set;
  int start_idx;
  int finish_idx;
  int current_idx;
  int neighbor_idx;
  int x, y;
  int nx, ny;
  int goal_x, goal_y;
  int i;
  bool found;
  int tentative_g;

  int dx[] = {0, 0, -1, 1};
  int dy[] = {-1, 1, 0, 0};

  // validate input
  if (!maze || !maze->grid) {
    fprintf(stderr, "Error: invalid maze in solve_astar\n");
    return false;
  }

  // calculate start and finish
  start_idx = IDX(0, 0, maze->width);
  finish_idx = IDX(maze->width - 1, maze->height - 1, maze->width);

  goal_x = maze->width - 1;
  goal_y = maze->height - 1;

  // allocate arrays
  g_score = malloc(maze->width * maze->height * sizeof(int));
  if (!g_score) {
    fprintf(stderr, "Error: failed to allocate g_score array\n");
    return false;
  }

  f_score = malloc(maze->width * maze->height * sizeof(int));
  if (!f_score) {
    fprintf(stderr, "Error: failed to allocate f_score array\n");
    free(g_score);
    return false;
  }

  parent = malloc(maze->width * maze->height * sizeof(int));
  if (!parent) {
    fprintf(stderr, "Error: failed to allocate parent array\n");
    free(g_score);
    free(f_score);
    return false;
  }

  in_closed = calloc(maze->width * maze->height, sizeof(bool));
  if (!in_closed) {
    fprintf(stderr, "Error: failed to allocate closed set\n");
    free(g_score);
    free(f_score);
    free(parent);
    return false;
  }

  // initialize scores
  for (i = 0; i < maze->width * maze->height; i++) {
    g_score[i] = INT_MAX;
    f_score[i] = INT_MAX;
    parent[i] = -1;
  }

  // create priority queue
  open_set = create_pq(maze->width * maze->height);
  if (!open_set) {
    fprintf(stderr, "Error: failed to create priority queue\n");
    free(g_score);
    free(f_score);
    free(parent);
    free(in_closed);
    return false;
  }

  // initialize start
  g_score[start_idx] = 0;
  f_score[start_idx] = heuristic(0, 0, goal_x, goal_y);
  pq_push(open_set, start_idx, f_score[start_idx]);

  // A* main loop
  found = false;

  while (!pq_is_empty(open_set)) {
    if (!pq_pop(open_set, &current_idx)) {
      break;
    }

    // skip if already processed
    if (in_closed[current_idx]) {
      continue;
    }

    in_closed[current_idx] = true;

    // found finish?
    if (current_idx == finish_idx) {
      found = true;
      break;
    }

    x = current_idx % maze->width;
    y = current_idx / maze->width;

    // check all neighbors
    for (i = 0; i < 4; i++) {
      nx = x + dx[i];
      ny = y + dy[i];

      if (!IS_VALID_POS(nx, ny, maze->width, maze->height)) {
        continue;
      }

      neighbor_idx = IDX(nx, ny, maze->width);

      if (in_closed[neighbor_idx]) {
        continue;
      }

      if (maze->grid[neighbor_idx] != '*') {
        continue;
      }

      tentative_g = g_score[current_idx] + 1;

      if (tentative_g < g_score[neighbor_idx]) {
        parent[neighbor_idx] = current_idx;
        g_score[neighbor_idx] = tentative_g;
        f_score[neighbor_idx] = tentative_g + heuristic(nx, ny, goal_x, goal_y);
        pq_push(open_set, neighbor_idx, f_score[neighbor_idx]);
      }
    }
  }

  // reconstruct path if found
  if (found) {
    reconstruct_path_astar(maze, parent, start_idx, finish_idx);
  }

  // cleanup
  free(g_score);
  free(f_score);
  free(parent);
  free(in_closed);
  free_pq(open_set);

  return found;
}

// A* with statistics
bool solve_astar_stats(Maze* maze, SolverStats* stats) {
  int* g_score;
  int* f_score;
  int* parent;
  bool* in_closed;
  PriorityQueue* open_set;
  int start_idx;
  int finish_idx;
  int current_idx;
  int neighbor_idx;
  int x, y;
  int nx, ny;
  int goal_x, goal_y;
  int i;
  bool found;
  int tentative_g;
  clock_t start_time, end_time;

  int dx[] = {0, 0, -1, 1};
  int dy[] = {-1, 1, 0, 0};

  // validate input
  if (!maze || !maze->grid || !stats) {
    fprintf(stderr, "Error: invalid input in solve_astar_stats\n");
    return false;
  }

  // initialize stats
  stats->algorithm = "A*";
  stats->cells_visited = 0;
  stats->path_length = 0;
  stats->time_ms = 0.0;

  start_time = clock();

  // calculate start and finish
  start_idx = IDX(0, 0, maze->width);
  finish_idx = IDX(maze->width - 1, maze->height - 1, maze->width);

  goal_x = maze->width - 1;
  goal_y = maze->height - 1;

  // allocate arrays
  g_score = malloc(maze->width * maze->height * sizeof(int));
  if (!g_score) {
    fprintf(stderr, "Error: failed to allocate g_score array\n");
    return false;
  }

  f_score = malloc(maze->width * maze->height * sizeof(int));
  if (!f_score) {
    fprintf(stderr, "Error: failed to allocate f_score array\n");
    free(g_score);
    return false;
  }

  parent = malloc(maze->width * maze->height * sizeof(int));
  if (!parent) {
    fprintf(stderr, "Error: failed to allocate parent array\n");
    free(g_score);
    free(f_score);
    return false;
  }

  in_closed = calloc(maze->width * maze->height, sizeof(bool));
  if (!in_closed) {
    fprintf(stderr, "Error: failed to allocate closed set\n");
    free(g_score);
    free(f_score);
    free(parent);
    return false;
  }

  // initialize scores
  for (i = 0; i < maze->width * maze->height; i++) {
    g_score[i] = INT_MAX;
    f_score[i] = INT_MAX;
    parent[i] = -1;
  }

  // create priority queue
  open_set = create_pq(maze->width * maze->height);
  if (!open_set) {
    fprintf(stderr, "Error: failed to create priority queue\n");
    free(g_score);
    free(f_score);
    free(parent);
    free(in_closed);
    return false;
  }

  // initialize start
  g_score[start_idx] = 0;
  f_score[start_idx] = heuristic(0, 0, goal_x, goal_y);
  pq_push(open_set, start_idx, f_score[start_idx]);

  // A* main loop
  found = false;

  while (!pq_is_empty(open_set)) {
    if (!pq_pop(open_set, &current_idx)) {
      break;
    }

    // skip if already processed
    if (in_closed[current_idx]) {
      continue;
    }

    in_closed[current_idx] = true;

    // found finish?
    if (current_idx == finish_idx) {
      found = true;
      break;
    }

    x = current_idx % maze->width;
    y = current_idx / maze->width;

    // check all neighbors
    for (i = 0; i < 4; i++) {
      nx = x + dx[i];
      ny = y + dy[i];

      if (!IS_VALID_POS(nx, ny, maze->width, maze->height)) {
        continue;
      }

      neighbor_idx = IDX(nx, ny, maze->width);

      if (in_closed[neighbor_idx]) {
        continue;
      }

      if (maze->grid[neighbor_idx] != '*') {
        continue;
      }

      tentative_g = g_score[current_idx] + 1;

      if (tentative_g < g_score[neighbor_idx]) {
        parent[neighbor_idx] = current_idx;
        g_score[neighbor_idx] = tentative_g;
        f_score[neighbor_idx] = tentative_g + heuristic(nx, ny, goal_x, goal_y);
        pq_push(open_set, neighbor_idx, f_score[neighbor_idx]);
      }
    }
  }

  // collect statistics
  stats->cells_visited = count_closed(in_closed, maze->width * maze->height);

  // reconstruct path if found
  if (found) {
    stats->path_length = reconstruct_path_astar(maze, parent, start_idx, finish_idx);
  }

  end_time = clock();
  stats->time_ms = ((double)(end_time - start_time)) / CLOCKS_PER_SEC * 1000.0;

  // cleanup
  free(g_score);
  free(f_score);
  free(parent);
  free(in_closed);
  free_pq(open_set);

  return found;
}

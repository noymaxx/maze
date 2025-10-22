#ifndef SOLVER_H
#define SOLVER_H

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// ======== STRUCTS ===========

// Positions
typedef struct {
  int x;
  int y;
}  Position;

// Maze
typedef struct {
  char *grid; // (* = free, X = wall, O = path)
  int width;
  int height;
} Maze;

// Performance statistics
typedef struct {
  double time_ms;           // execution time in milliseconds
  int cells_visited;        // number of cells explored
  int path_length;          // length of solution path (0 if no solution)
  const char* algorithm;    // "BFS" or "A*"
} SolverStats;

// ========= MACROS ===========

// 2D position (x, y) to 1D array - index = y * width + x
#define IDX(x, y, width) ((y) * (width) + (x))

// check and verify maze positions
#define IS_VALID_POS(x, y, w, h) ((x) >= 0 && (x) < (w) && (y) >= 0 && (y) <(h))

// ======== FUNCTIONS ==========

// load maze from file
Maze* load_maze(const char* filename);

// free memory
void free_maze(Maze* maze);

// print maze
void print_maze(const Maze* maze);

// validate maze
bool validate_maze(const Maze* maze);

// ======= ALGO FUNCTIONS ==========

// solve bfs
bool solve_bfs(Maze* maze);

// solve bfs with stats
bool solve_bfs_stats(Maze* maze, SolverStats* stats);

// solve astar
bool solve_astar(Maze* maze);

// solve astar with stats
bool solve_astar_stats(Maze* maze, SolverStats* stats);

// ======= UTILITY FUNCTIONS ==========

// print statistics
void print_stats(const SolverStats* stats);

#endif
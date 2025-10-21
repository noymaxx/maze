#ifndef SOLVER_H
#define SOLVER_H

// Standard libraries
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
bool solve_bfs(const Maze* maze);

// solve astar
bool solve_astar(Maze* maze);

#endif
#include "solver.h"

// =========== VALIDATE ==========

bool validate_maze(const Maze *maze) {
  int start_idx, finish_idx;
  int i;
  char c;

  // check null
  if (!maze) {
    fprintf(stderr, "[ERROR]: Maze is null");
    return false;
  }

  if (!maze -> grid) {
    fprintf(stderr, "[ERROR]: Maze is null");
    return false;
  }

  // check dimensions
  if (maze->width < 1 || maze->width > 10000) {
    fprintf(stderr, "Error: invalid width %d (must be 1-10000)\n", maze->width);
    return false;
  }
    
  if (maze->height < 1 || maze->height > 10000) {
    fprintf(stderr, "Error: invalid height %d (must be 1-10000)\n", maze->height);
    return false;
  }

  // start positions
  start_idx = IDX(0, 0, maze->width);
  if (maze->grid[start_idx] != '*') {
    return false;
  }

  // finish position
  finish_idx = IDX(maze->width - 1, maze->height - 1, maze->width);

  if (maze->grid[finish_idx] != '*') {
    return false;
  }

  // check characters
  for (i = 0; i < maze->width * maze->height; i++) {
    c = maze->grid[i];

    if (c != '*' && c != 'X') {
      fprintf(stderr, "Error: invalid character '%c' at index %d\n", c, i);
      fprintf(stderr, "Valid characters are: '*' (free) and 'X' (wall)\n");
      return false;
    }
  }

  return true;
}
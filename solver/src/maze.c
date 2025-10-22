#include "solver.h"

// ======= HELPER FUNCTIONS ========

// Get line length
static int get_line_length(FILE *fp) {
  int count = 0;
  int c;

  while ((c = fgetc(fp)) != EOF) {
    if (c == '\n') {
      break;
    }
    count++;
  }
  return count;
}

// Count numbers of lines
static int count_lines(FILE *fp) {
  int lines = 0;
  int c;
  int last_char = '\n';

  while((c = fgetc(fp)) != EOF) {
    if (c == '\n') {
      lines++;
    }
    last_char = c;
  }

  if (last_char != '\n' && last_char != EOF) {
    lines++;
  }

  return lines;
}

// ========= MAIN FUNCTIONS =========

Maze *load_maze(const char *filename) {
  FILE *fp;
  Maze *maze;
  int width, height;
  int x, y;
  int c;
  
  // open file
  fp = fopen(filename, "r");
  if (!fp) {
    fprintf(stderr, "[ERROR]: Can't open file '%s'\n", filename);
    return NULL;
  }

  // detect width
  width = get_line_length(fp);

  if (width == 0) {
    fprintf(stderr, "[ERROR]: Empty file or invalid maze...\n");
    fclose(fp);
    return NULL;
  }

  // detect height
  rewind(fp);
  height = count_lines(fp);

  if (height == 0) {
    fprintf(stderr, "[ERROR]: dimensions out of \n");
    fprintf(stderr, "Got: width = %d, height = %d\n", width, height);
    fclose(fp);
    return NULL;
  }

  // allocate maze
  maze = malloc(sizeof(Maze));
  if (!maze) {
    fprintf(stderr, "[ERROR]: memory allocation failed in maze\n");
    fclose(fp);
    return NULL;
  }

  // allocate grid
  maze -> grid = malloc(width * height * sizeof(char));
  if (!maze -> grid) {
    fprintf(stderr, "[ERROR]: memory allocation failed in grid\n");
    free(maze);
    fclose(fp);
    return NULL;
  }

  maze -> width = width;
  maze -> height = height;

  // read maze data into grid
  rewind(fp);

  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      c = fgetc(fp);

      if (c == EOF || c == '\n') {
        if (x < width - 1) {
          fprintf(stderr, "[ERROR]: line %d is shorter than expected\n", y+1);
          free_maze(maze);
          fclose(fp);
          return NULL;
        }

        if (c == '\n' && x == width - 1) {
          c = fgetc(fp);
          if (c == EOF) {
            fprintf(stderr, "[ERROR]: unexpected EOF\n");
            free_maze(maze);
            fclose(fp);
            return NULL;
          }
        }
      }
      maze -> grid[IDX(x, y, width)] = (char)c;
    }

    if (y < height -1) {
      while ((c = fgetc(fp)) != EOF && c != '\n') {
        // skip to end of line
      }
    }
  }
  fclose(fp);
  return maze;
}

// ======= FREE FUNCTIONS ======

void free_maze(Maze *maze) {
  if (!maze){
    return;
  }

  if (maze->grid) {
    free(maze->grid);
  }

  free(maze);
}

// ====== PRINT FUNCTION ========

void print_maze(const Maze *maze) {
  int x, y;

  if (!maze || !maze -> grid) {
    return;
  }

  for (y = 0; y < maze -> height; y++) {
    for (x = 0; x < maze -> width; x++) {
      printf("%c", maze -> grid[IDX(x, y, maze -> width)]);
    }
    printf("\n");
  }
}

// ====== STATS FUNCTION ========

void print_stats(const SolverStats* stats) {
  if (!stats) {
    return;
  }

  fprintf(stderr, "\n========== Performance Statistics ==========\n");
  fprintf(stderr, "Algorithm:       %s\n", stats->algorithm);
  fprintf(stderr, "Time:            %.3f ms\n", stats->time_ms);
  fprintf(stderr, "Cells visited:   %d\n", stats->cells_visited);
  fprintf(stderr, "Path length:     %d\n", stats->path_length);
  fprintf(stderr, "============================================\n\n");
}
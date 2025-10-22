#include "solver.h"

typedef enum {
  ALGO_BFS,
  ALGO_ASTAR
} Algorithm;

void print_usage(const char* program_name) {
  fprintf(stderr, "Usage: %s <maze_file> [OPTIONS]\n", program_name);
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "  --bfs       Use BFS algorithm (default)\n");
  fprintf(stderr, "  --astar     Use A* algorithm\n");
  fprintf(stderr, "  --stats     Show performance statistics\n");
  fprintf(stderr, "  --help      Show this help message\n");
}

int main(int argc, char* argv[]) {
  Maze* maze;
  bool solved;
  Algorithm algo = ALGO_BFS;
  bool show_stats = false;
  const char* maze_file = NULL;
  SolverStats stats;
  int i;

  // parse arguments
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  // first argument is the maze file
  maze_file = argv[1];

  // parse options
  for (i = 2; i < argc; i++) {
    if (strcmp(argv[i], "--bfs") == 0) {
      algo = ALGO_BFS;
    } else if (strcmp(argv[i], "--astar") == 0) {
      algo = ALGO_ASTAR;
    } else if (strcmp(argv[i], "--stats") == 0) {
      show_stats = true;
    } else if (strcmp(argv[i], "--help") == 0) {
      print_usage(argv[0]);
      return 0;
    } else {
      fprintf(stderr, "Unknown option: %s\n", argv[i]);
      print_usage(argv[0]);
      return 1;
    }
  }

  // load maze from file
  maze = load_maze(maze_file);
  if (!maze) {
    fprintf(stderr, "[ERROR] Failed to load maze\n");
    return 1;
  }

  // validate maze structure
  if (!validate_maze(maze)) {
    printf("no solution found\n");
    printf("UNSOLVED :(\n");
    free_maze(maze);
    return 0;
  }

  // solve maze with selected algorithm
  if (show_stats) {
    // solve with statistics
    if (algo == ALGO_BFS) {
      solved = solve_bfs_stats(maze, &stats);
    } else {
      solved = solve_astar_stats(maze, &stats);
    }

    // print result
    if (solved) {
      print_maze(maze);
      printf("SOLVED :)\n");
      print_stats(&stats);
    } else {
      printf("no solution found\n");
      printf("UNSOLVED :(\n");
      print_stats(&stats);
    }
  } else {
    // solve without statistics
    if (algo == ALGO_BFS) {
      solved = solve_bfs(maze);
    } else {
      solved = solve_astar(maze);
    }

    // print result
    if (solved) {
      print_maze(maze);
      printf("SOLVED :)\n");
    } else {
      printf("no solution found\n");
      printf("UNSOLVED :(\n");
    }
  }

  free_maze(maze);
  return 0;
}

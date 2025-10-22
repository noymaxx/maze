# Alice's Journey - Maze Solver for Epitech


Alice just saw a talking white rabbit wearing a waistcoat, running past her shouting *"Oh dear! Oh dear! I shall be too late!"*. Weird.

She needs to follow the rabbit through a maze-like place. Our goal in this project is to **solve mazes in a reasonable amount of time** and show the solution.

---

## How? 

- **Two pathfinding algorithms:**
  - **BFS (Breadth-First Search)** - Guarantees shortest path
  - **A-star** - Heuristic-based, more efficient exploration

- **Performance measurement:**
  - Execution time (milliseconds)
  - Cells visited during search
  - Solution path length

- **Flexible maze support:**
  - Maze sizes: 1x1 to 10000x10000
  - ASCII format (`*` = free space, `X` = wall, `o` = solution path)
  - 4-directional movement (up, down, left, right)

- **Smart validation:**
  - Detects blocked start/finish positions
  - Handles invalid maze formats
  - Reports "no solution found" when appropriate

---

##  Start and Commands


| Category | Command | Description |
|----------|---------|-------------|
| **Build** | `make` | Compile the project |
| | `make re` | Clean and recompile |
| | `make clean` | Remove object files |
| | `make fclean` | Remove all generated files |
| **Main Test** | `make test` | **Comprehensive test suite**: All mazes with both BFS & A* + stats. Saves results to `tests/results/` |
| **Algorithm Tests** | `make test-bfs` | Run all tests with BFS only |
| | `make test-astar` | Run all tests with A* only |
| | `make test-stats` | All tests with BFS + stats |
| | `make test-astar-stats` | All tests with A* + stats |
| | `make test-compare` | Compare BFS vs A* side-by-side |
| **Single File** | `make test-single FILE=<path>` | Test specific maze |
| | `make test-single-stats FILE=<path>` | Test specific maze with both algorithms + stats |


---

## Outputs 

Every test execution will display:

1. **The solved maze** - Path marked with `o` from start (0,0) to finish
2. **Status message:**
   - **SOLVED :)** - When a solution path is found
   - **UNSOLVED :(** - When no solution exists
3. **Performance statistics**:
   - Algorithm used (BFS or A*)
   - Execution time in milliseconds
   - Number of cells visited
   - Solution path length

---

## Maze Format

### Format Description

- **Rectangular mazes** coded in ASCII
- `*` represents **free spaces** (passable)
- `X` represents **walls** (impassable)
- **Start:** Upper-left corner `(0, 0)`
- **Finish:** Bottom-right corner `(width-1, height-1)`
- **Solution:** Marked with `o` characters from start to finish
- Last line doesn't terminate with a newline

### Example

**Input (24x6):**
```
*****XX****X********XXXX
XX******XX***XXXXX***XXX
XX***XXXX**XXXXX****XXXX
XX***XXXXXXXXXXXXXX****X
*****XXXXXX****XX***XXXX
XX*************XXXX*****
```

**Solved:**
```
oooooXXooooXooooooooXXXX
XX**ooooXXoooXXXXX*o*XXX
XX***XXXX**XXXXX***oXXXX
XX***XXXXXXXXXXXXXXo***X
*****XXXXXX****XX**oXXXX
XX*************XXXXooooo
```


---

## Performance Analysis

#### BFS (Breadth-First Search)
- **Time complexity:** O(W × H)
- **Space complexity:** O(W × H)
- **Guarantees:** Shortest path
- **Best for:** Small to medium mazes, when shortest path is critical

#### A* (A-star)
- **Time complexity:** O(W × H) worst case, often much better
- **Space complexity:** O(W × H)
- **Heuristic:** Manhattan distance
- **Best for:** Large mazes, when efficiency matters

---

## Project Structure

```
solver/
├── include/
│   ├── queue.h              # Queue data structure (for BFS)
│   ├── priority_queue.h     # Min-heap (for A*)
│   └── solver.h             # Main header with structs and functions
├── src/
│   ├── main.c               # Entry point, CLI parsing
│   ├── maze.c               # Maze loading and I/O
│   ├── validation.c         # Maze validation
│   ├── queue.c              # Queue implementation
│   ├── priority_queue.c     # Priority queue implementation
│   ├── solver_bfs.c         # BFS algorithm
│   └── solver_astar.c       # A* algorithm
├── tests/
│   └── generated/           # Test cases (1x1 to 5000x5000)
|   └── results/             # Tests results
├── Makefile                 # Build system and run tests and algorithms
└── README.md                # This file
```

---

## Algorithms:

### BFS (Breadth-First Search)

Implemented with queue structures, BFS explores the maze level by level, ensuring the **shortest path** is found:

1. Start from (0,0)
2. Explore all neighbors at distance 1
3. Then all neighbors at distance 2
4. Continue until reaching the goal or exploring all reachable cells

**Pros:**
- Guarantees shortest path
- Simple to implement

**Cons:**
- Explores many unnecessary cells
- Memory-intensive for large mazes

### A* (A-star)

A* uses a **heuristic function** (Manhattan distance) to prioritize which cells to explore:

1. Maintain `g(n)` = actual cost from start to current cell
2. Calculate `h(n)` = heuristic (Manhattan distance to goal)
3. Priority = `f(n) = g(n) + h(n)`
4. Always explore the cell with lowest `f(n)`

**Manhattan Distance Heuristic:**
```
h(x, y) = |x - goal_x| + |y - goal_y|
```

**Pros:**
- More efficient exploration
- Fewer cells visited
- Still guarantees shortest path (with admissible heuristic)

**Cons:**
- Slightly more complex implementation
- Requires priority queue

---

## Testing

The project includes comprehensive test mazes and multiple testing modes.

### Available Test Mazes

All test files are located in `tests/generated/`:

| File | Size | Description |
|------|------|-------------|
| `test_5x5_simple.txt` | 5×5 | Simple maze |
| `test_24x6.txt` | 24×6 | Wide maze |
| `test_20x20.txt` | 20×20 | Medium complexity |
| `test_complex_20x20.txt` | 20×20 | High complexity pattern |
| `test_medium.txt` | 20×20 | Nested paths |
| `test_large.txt` | 28×19 | Large complex maze |
| `test_no_solution.txt` | 3×3 | Unsolvable maze |

### Running Tests

```bash
# Comprehensive test (RECOMMENDED)
make test              # All mazes with BOTH algorithms + stats
                       # Saves results to tests/results/test_results_YYYYMMDD_HHMMSS.txt

# Individual algorithm tests
make test-bfs          # All tests with BFS only
make test-astar        # All tests with A* only
make test-stats        # All tests with BFS + stats
make test-astar-stats  # All tests with A* + stats
make test-compare      # Side-by-side BFS vs A* comparison

# Single file testing
make test-single FILE=tests/generated/test_5x5_simple.txt
make test-single-stats FILE=tests/generated/test_24x6.txt
```

---

### Merci :) 
# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -Iinclude
LDFLAGS = 

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

# Target binary name
NAME = solver

# Source files
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/maze.c \
			$(SRC_DIR)/validation.c \
			$(SRC_DIR)/queue.c \
			$(SRC_DIR)/priority_queue.c \
			$(SRC_DIR)/solver_bfs.c \
			$(SRC_DIR)/solver_astar.c

# Object files 
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# ============= RULES ==============

# build everything
all: $(NAME)

# Link object files to create the binary
$(NAME): $(OBJ)
	@echo " Linking $(NAME)..."
	@$(CC) $(OBJ) -o $(NAME) $(LDFLAGS)
	@echo "Build complete! Run with: ./$(NAME) <maze_file>"

# Compile .c files to .o files
# $< = source file 
# $@ = target file 
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo " Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

# Remove object files
clean:
	@echo " Cleaning object files..."
	@rm -rf $(OBJ_DIR)
	@echo " Clean complete!"

# Remove object files AND binary
fclean: clean
	@echo " Removing binary..."
	@rm -f $(NAME)
	@echo " Full clean complete!"

# Rebuild everything from scratch
re: fclean all

# Test directory
TEST_DIR = tests/generated
RESULTS_DIR = tests/results
RESULT_FILE = $(RESULTS_DIR)/test_results_$(shell date +%Y%m%d_%H%M%S).txt

# Comprehensive test - runs all tests with both algorithms and comparison
test: $(NAME)
	@mkdir -p $(RESULTS_DIR)
	@echo "=========================================" | tee $(RESULT_FILE)
	@echo "   COMPREHENSIVE MAZE SOLVER TEST SUITE" | tee -a $(RESULT_FILE)
	@echo "   Date: $$(date '+%Y-%m-%d %H:%M:%S')" | tee -a $(RESULT_FILE)
	@echo "=========================================" | tee -a $(RESULT_FILE)
	@echo "" | tee -a $(RESULT_FILE)
	@for maze in $(TEST_DIR)/*.txt; do \
		echo "========================================" | tee -a $(RESULT_FILE); \
		echo "Testing: $$maze" | tee -a $(RESULT_FILE); \
		echo "========================================" | tee -a $(RESULT_FILE); \
		echo "" | tee -a $(RESULT_FILE); \
		echo "--- BFS Algorithm ---" | tee -a $(RESULT_FILE); \
		./$(NAME) $$maze --bfs --stats 2>&1 | tee -a $(RESULT_FILE) || echo "FAILED: $$maze (BFS)" | tee -a $(RESULT_FILE); \
		echo "" | tee -a $(RESULT_FILE); \
		echo "--- A* Algorithm ---" | tee -a $(RESULT_FILE); \
		./$(NAME) $$maze --astar --stats 2>&1 | tee -a $(RESULT_FILE) || echo "FAILED: $$maze (A*)" | tee -a $(RESULT_FILE); \
		echo "" | tee -a $(RESULT_FILE); \
		echo "========================================" | tee -a $(RESULT_FILE); \
		echo "" | tee -a $(RESULT_FILE); \
	done
	@echo "=========================================" | tee -a $(RESULT_FILE)
	@echo "   ALL TESTS COMPLETED!" | tee -a $(RESULT_FILE)
	@echo "   Results saved to: $(RESULT_FILE)" | tee -a $(RESULT_FILE)
	@echo "=========================================" | tee -a $(RESULT_FILE)

# Run all tests with BFS algorithm
test-bfs: $(NAME)
	@echo "========================================="
	@echo "Running BFS tests on $(TEST_DIR)"
	@echo "========================================="
	@for maze in $(TEST_DIR)/*.txt; do \
		echo ""; \
		echo "Testing: $$maze (BFS)"; \
		echo "-----------------------------------------"; \
		./$(NAME) $$maze --bfs || echo "FAILED: $$maze"; \
		echo ""; \
	done
	@echo "========================================="
	@echo "BFS tests completed!"
	@echo "========================================="

# Run all tests with A* algorithm
test-astar: $(NAME)
	@echo "========================================="
	@echo "Running A* tests on $(TEST_DIR)"
	@echo "========================================="
	@for maze in $(TEST_DIR)/*.txt; do \
		echo ""; \
		echo "Testing: $$maze (A*)"; \
		echo "-----------------------------------------"; \
		./$(NAME) $$maze --astar || echo "FAILED: $$maze"; \
		echo ""; \
	done
	@echo "========================================="
	@echo "A* tests completed!"
	@echo "========================================="

# Run all tests with BFS and statistics
test-stats: $(NAME)
	@echo "========================================="
	@echo "Running tests with statistics (BFS)"
	@echo "========================================="
	@for maze in $(TEST_DIR)/*.txt; do \
		echo ""; \
		echo "Testing: $$maze (BFS + Stats)"; \
		echo "-----------------------------------------"; \
		./$(NAME) $$maze --bfs --stats || echo "FAILED: $$maze"; \
		echo ""; \
	done
	@echo "========================================="
	@echo "Stats tests completed!"
	@echo "========================================="

# Run all tests with A* and statistics
test-astar-stats: $(NAME)
	@echo "========================================="
	@echo "Running tests with statistics (A*)"
	@echo "========================================="
	@for maze in $(TEST_DIR)/*.txt; do \
		echo ""; \
		echo "Testing: $$maze (A* + Stats)"; \
		echo "-----------------------------------------"; \
		./$(NAME) $$maze --astar --stats || echo "FAILED: $$maze"; \
		echo ""; \
	done
	@echo "========================================="
	@echo "A* stats tests completed!"
	@echo "========================================="

# Compare BFS vs A* performance on all tests
test-compare: $(NAME)
	@echo "========================================="
	@echo "Comparing BFS vs A* on $(TEST_DIR)"
	@echo "========================================="
	@for maze in $(TEST_DIR)/*.txt; do \
		echo ""; \
		echo "========================================"; \
		echo "Maze: $$maze"; \
		echo "========================================"; \
		echo ""; \
		echo "--- BFS ---"; \
		./$(NAME) $$maze --bfs --stats | tail -n 5 || echo "FAILED"; \
		echo ""; \
		echo "--- A* ---"; \
		./$(NAME) $$maze --astar --stats | tail -n 5 || echo "FAILED"; \
		echo ""; \
	done
	@echo "========================================="
	@echo "Comparison completed!"
	@echo "========================================="

# Run a single test file (usage: make test-single FILE=path/to/maze.txt)
test-single: $(NAME)
	@if [ -z "$(FILE)" ]; then \
		echo "Error: Please specify FILE=path/to/maze.txt"; \
		exit 1; \
	fi
	@echo "Testing: $(FILE)"
	@echo "-----------------------------------------"
	@./$(NAME) $(FILE)

# Run a single test with stats (usage: make test-single-stats FILE=path/to/maze.txt)
test-single-stats: $(NAME)
	@if [ -z "$(FILE)" ]; then \
		echo "Error: Please specify FILE=path/to/maze.txt"; \
		exit 1; \
	fi
	@echo "Testing: $(FILE) (with stats)"
	@echo "-----------------------------------------"
	@./$(NAME) $(FILE) --bfs --stats
	@echo ""
	@./$(NAME) $(FILE) --astar --stats

# Phony targets
.PHONY: all clean fclean re test test-bfs test-astar test-stats test-astar-stats test-compare test-single test-single-stats
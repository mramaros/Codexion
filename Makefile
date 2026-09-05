RED     = \033[0;31m
GREEN   = \033[0;32m
YELLOW  = \033[0;33m
BLUE    = \033[0;34m
CYAN    = \033[0;36m

CC       = cc
FLAG     = -Wall -Werror -Wextra -pthread
OBJ      = codexion
ALL_FILE = coder_routine.c \
            dongle_compile.c \
            dongles_utils.c \
            display_manager.c \
            edf_queue.c \
            fifo_queue.c \
            list_coders.c \
            list_dongles.c \
            main.c \
            monitor_for_burn-out.c \
            neighbor_check.c \
            parsers.c \
            simulation_start.c \
            utils.c
.PHONY: all run clean help

all: run

help:
	@echo "$(CYAN) =============================================================="
	@echo "$(CYAN) |         CODEXION - Available Targets                       |"
	@echo "$(CYAN) =============================================================="
	@echo ""
	@echo "$(YELLOW)make run$(NC)    - Compile and run the project"
	@echo "$(YELLOW)make all$(NC)    - Same as 'make run'"
	@echo "$(YELLOW)make clean$(NC)  - Remove compiled executable"
	@echo "$(YELLOW)make help$(NC)   - Display this help message"
	@echo ""

run:
	@echo "$(CYAN) =============================================================="
	@echo "$(BLUE) |                 Building CODEXION...                       |"
	@echo "$(CYAN) =============================================================="
	@$(CC) $(FLAG) $(ALL_FILE) -o $(OBJ)
	@echo "$(GREEN)! Compilation successful!"
	@echo "$(GREEN)! Executable: $(OBJ)"
	@echo "$(GREEN)! Ready to run!"

clean:
	@echo "$(YELLOW).... Cleaning up..."
	@rm -rf $(OBJ)
	@echo "$(GREEN)! Clean complete!"

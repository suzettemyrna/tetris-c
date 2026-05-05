# ============================================
# Settings
# ============================================

# Name
LIB_NAME = libtetris.a
EXEC = tetris

# ============================================
# Paths
# ============================================

SRC_DIR = ./src

# Source directories
CORE_DIR = $(SRC_DIR)/core
CORE_INCLUDE_DIR = $(CORE_DIR)/include

API_DIR = $(SRC_DIR)/api
API_INCLUDE_DIR = $(API_DIR)/include

CLI_DIR = $(SRC_DIR)/cli
CLI_INCLUDE_DIR = $(CLI_DIR)/include

SHARED_DIR = $(SRC_DIR)/shared

TESTS_DIR = tests
MOCKS_DIR = $(TESTS_DIR)/mocks

# Build directories
BUILD_DIR = $(SRC_DIR)/build
LIB_BUILD_DIR = $(BUILD_DIR)/lib
GUI_BUILD_DIR = $(BUILD_DIR)/gui
OBJ_DIR = $(BUILD_DIR)/obj
TESTS_BUILD_DIR = $(TESTS_DIR)/build
GCOV_DIR = $(TESTS_DIR)/gcov-reports

# ============================================
# Installation paths
# ============================================

PREFIX ?= /usr/local
BIN_DIR = $(PREFIX)/bin
LIB_INSTALL_DIR = $(PREFIX)/lib
INCLUDE_INSTALL_DIR = $(PREFIX)/include/tetris

# ============================================
# Compiler and flags
# ============================================

CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

# Release flags
CFLAGS_RELEASE = $(CFLAGS) -O2 -fPIC
LDFLAGS_RELEASE = -lncurses -lm

# Test and coverage flags
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
CFLAGS_TEST = $(CFLAGS) -O0 $(GCOV_FLAGS)
LDFLAGS_TEST = $(CHECK_LIBS)

# check libs — OS-specific additions
CHECK_LIBS = -lcheck -lm -lpthread
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	CHECK_LIBS += -lrt -lsubunit -pthread
else ifeq ($(UNAME_S),Darwin)
	CHECK_LIBS += -pthread
endif

# ============================================
# Source files
# ============================================

# Library source files
LIB_SRCS = \
	$(CORE_DIR)/field_control.c \
	$(CORE_DIR)/game_info.c \
	$(CORE_DIR)/score_file.c \
	$(CORE_DIR)/state_machine.c \
	$(CORE_DIR)/tetromino.c \
	$(CORE_DIR)/user_actions.c \
	$(API_DIR)/tetris_api.c

# GUI (client) source files
GUI_SRCS = \
    $(CLI_DIR)/cli.c \
    $(CLI_DIR)/main.c

# Test source files
TESTS_SRCS = \
	$(TESTS_DIR)/test_tetris_api.c \
	$(TESTS_DIR)/test_user_actions.c \
	$(TESTS_DIR)/test_state_machine.c \
	$(TESTS_DIR)/test_game_info.c \
	$(TESTS_DIR)/test_tetromino.c \
	$(TESTS_DIR)/test_field_control.c \
	$(TESTS_DIR)/test_score_file.c \
	$(MOCKS_DIR)/mock_cli.c \
	$(MOCKS_DIR)/mock_field_control_G_I.c \
	$(MOCKS_DIR)/mock_state_machine_T_A.c \
	$(MOCKS_DIR)/mock_game_info_S_M.c \
	$(MOCKS_DIR)/mock_state_machine_T_A.c \
	$(MOCKS_DIR)/mock_game_info_U_A.c \
	$(MOCKS_DIR)/mock_score_file.c \
	$(MOCKS_DIR)/mock_state_machine_T_A.c \
	$(MOCKS_DIR)/mock_state_machine_U_A.c \
	$(MOCKS_DIR)/mock_tetromino.c \
	$(MOCKS_DIR)/mock_user_actions.c

# Per-test source groups
TEST_T_A_SRCS = \
	$(TESTS_DIR)/test_tetris_api.c \
	$(MOCKS_DIR)/mock_cli.c \
	$(MOCKS_DIR)/mock_state_machine_T_A.c \
	$(MOCKS_DIR)/mock_game_info_T_A.c \
	$(MOCKS_DIR)/mock_user_actions.c

TEST_U_A_SRCS = \
	$(TESTS_DIR)/test_user_actions.c \
	$(MOCKS_DIR)/mock_state_machine_U_A.c \
	$(MOCKS_DIR)/mock_game_info_U_A.c

TEST_S_M_SRCS = \
	$(TESTS_DIR)/test_state_machine.c \
	$(MOCKS_DIR)/mock_game_info_S_M.c

TEST_G_I_SRCS = \
	$(TESTS_DIR)/test_game_info.c \
	$(MOCKS_DIR)/mock_field_control_G_I.c \
	$(MOCKS_DIR)/mock_score_file.c \
	$(MOCKS_DIR)/mock_tetromino.c

TEST_S_F_SRCS = \
	$(TESTS_DIR)/test_score_file.c

TEST_T_M_SRCS = \
	$(TESTS_DIR)/test_tetromino.c \
	$(MOCKS_DIR)/mock_field_control_T_M.c

TEST_F_C_SRCS = \
	$(TESTS_DIR)/test_field_control.c

# ============================================
# Headers
# ============================================

# Public headers
PUBLIC_HEADERS = \
	$(API_INCLUDE_DIR)/tetris_api.h \
	$(SHARED_DIR)/game_config.h

# Private headers
PRIVATE_HEADERS = \
	$(CORE_INCLUDE_DIR)/backend_defines.h \
	$(CORE_INCLUDE_DIR)/backend_objects.h \
	$(CORE_INCLUDE_DIR)/field_control.h \
	$(CORE_INCLUDE_DIR)/game_info.h \
	$(CORE_INCLUDE_DIR)/score_file.h \
	$(CORE_INCLUDE_DIR)/state_machine.h \
	$(CORE_INCLUDE_DIR)/tetromino.h \
	$(CORE_INCLUDE_DIR)/user_actions.h

# Test headers
TESTS_HEADER = \
	$(TESTS_DIR)/test_helpers.h

# ============================================
# Objects and targets
# ============================================

# Object files (release)
LIB_OBJS = $(LIB_SRCS:%.c=$(OBJ_DIR)/%.o)
GUI_OBJS = $(GUI_SRCS:%.c=$(OBJ_DIR)/%.o)

# Final targets
STATIC_LIB = $(LIB_BUILD_DIR)/$(LIB_NAME)
EXEC_PATH = $(SRC_DIR)/$(EXEC)

TESTS_ALL = tetris_api_test user_actions_test state_machine_test game_info_test score_file_test tetromino_test field_control_test

# ============================================
# Main targets
# ============================================

.PHONY: all lib gui release clean install uninstall dvi dist test tests clang-format clang-check format check

all: lib

# Build static library (release)
lib: $(STATIC_LIB)

$(STATIC_LIB): $(LIB_OBJS)
	@mkdir -p $(LIB_BUILD_DIR)
	ar rcs $@ $(LIB_OBJS)
	ranlib $@
	@echo "Static library built: $@"

# Release build
release: CFLAGS := $(CFLAGS_RELEASE)
release: LDFLAGS := $(LDFLAGS_RELEASE)
release: lib gui
	@echo "=== Release build complete ==="
	@echo "Library: $(STATIC_LIB)"
	@echo "EXEC: $(EXEC_PATH)"

# Build GUI (client) with library
gui: $(EXEC_PATH)

$(EXEC_PATH): $(GUI_OBJS) $(STATIC_LIB)
	@mkdir -p $(GUI_BUILD_DIR)
	$(CC) $(CFLAGS_RELEASE) -o $@ $(GUI_OBJS) -L$(LIB_BUILD_DIR) -ltetris $(LDFLAGS_RELEASE)
	@echo "GUI executable built: $@"

# Compile object files
$(OBJ_DIR)/%.o: %.c $(PUBLIC_HEADERS) $(PRIVATE_HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS_RELEASE) \
	-I$(CORE_INCLUDE_DIR) \
	-I$(API_INCLUDE_DIR) \
	-I$(CLI_INCLUDE_DIR) \
	-I$(SHARED_DIR) \
	-c $< -o $@

# ============================================
# Testing with coverage
# (each test is built with its required mocks)
# ============================================

test: tests

$(TESTS_BUILD_DIR):
	@mkdir -p $(TESTS_BUILD_DIR)

# Run all tests and generate combined HTML coverage report
tests: $(TESTS_BUILD_DIR) $(TESTS_ALL)
	@echo "All tests finished. Generating combined coverage report..."

	@mkdir -p $(GCOV_DIR)/combined
	lcov --capture --directory ./tests/build --output-file $(GCOV_DIR)/combined/coverage.info >/dev/null 2>&1 || true
	lcov -r $(GCOV_DIR)/combined/coverage.info "/usr/*" "*/tests/*" --output-file $(GCOV_DIR)/combined/coverage.info >/dev/null 2>&1 || true
	genhtml $(GCOV_DIR)/combined/coverage.info --output-directory $(GCOV_DIR)/combined/html >/dev/null 2>&1 || true
	@echo "Combined HTML coverage report: $(GCOV_DIR)/combined/html/index.html"
	@if command -v xdg-open >/dev/null 2>&1; then xdg-open $(GCOV_DIR)/combined/html/index.html >/dev/null 2>&1 || true; \
	 elif command -v open >/dev/null 2>&1; then open $(GCOV_DIR)/combined/html/index.html >/dev/null 2>&1 || true; \
	 else echo \"Open the coverage report at: $(GCOV_DIR)/combined/html/index.html\"; fi

tetris_api_test: $(TESTS_BUILD_DIR)
	@echo "Building tetris_api tests..."
	$(CC) $(CFLAGS_TEST) $(TEST_T_A_SRCS) $(LDFLAGS_TEST) -I$(CORE_INCLUDE_DIR) \
	-I$(API_INCLUDE_DIR) \
	-I$(CLI_INCLUDE_DIR) \
	-I$(SHARED_DIR) -o $(TESTS_BUILD_DIR)/test_tetris_api
	@echo "Running tetris_api tests..."
	$(TESTS_BUILD_DIR)/test_tetris_api || exit 1
	@echo "tetris_api tests done."


user_actions_test: $(TESTS_BUILD_DIR)
	@echo "Building user_actions tests..."
	$(CC) $(CFLAGS_TEST) $(TEST_U_A_SRCS) $(LDFLAGS_TEST) -I$(CORE_INCLUDE_DIR) \
	-I$(API_INCLUDE_DIR) \
	-I$(CLI_INCLUDE_DIR) \
	-I$(SHARED_DIR) -o $(TESTS_BUILD_DIR)/test_user_actions
	@echo "Running user_actions tests..."
	$(TESTS_BUILD_DIR)/test_user_actions || exit 1
	@echo "user_actions tests done."

state_machine_test: $(TESTS_BUILD_DIR)
	@echo "Building state_machine tests..."
	$(CC) $(CFLAGS_TEST) $(TEST_S_M_SRCS) $(LDFLAGS_TEST) -I$(CORE_INCLUDE_DIR) \
	-I$(API_INCLUDE_DIR) \
	-I$(CLI_INCLUDE_DIR) \
	-I$(SHARED_DIR) -o $(TESTS_BUILD_DIR)/test_state_machine
	@echo "Running state_machine tests..."
	$(TESTS_BUILD_DIR)/test_state_machine || exit 1
	@echo "state_machine tests done."

game_info_test: $(TESTS_BUILD_DIR)
	@echo "Building game_info tests..."
	$(CC) $(CFLAGS_TEST) $(TEST_G_I_SRCS) $(LDFLAGS_TEST) -I$(CORE_INCLUDE_DIR) \
	-I$(API_INCLUDE_DIR) \
	-I$(CLI_INCLUDE_DIR) \
	-I$(SHARED_DIR) -o $(TESTS_BUILD_DIR)/test_game_info
	@echo "Running game_info tests..."
	$(TESTS_BUILD_DIR)/test_game_info || exit 1
	@echo "game_info tests done."

score_file_test: $(TESTS_BUILD_DIR)
	@echo "Building score_file tests..."
	$(CC) $(CFLAGS_TEST) $(TEST_S_F_SRCS) $(LDFLAGS_TEST) -I$(CORE_INCLUDE_DIR) \
	-I$(API_INCLUDE_DIR) \
	-I$(CLI_INCLUDE_DIR) \
	-I$(SHARED_DIR) -o $(TESTS_BUILD_DIR)/test_score_file
	@echo "Running score_file tests..."
	$(TESTS_BUILD_DIR)/test_score_file || exit 1
	@echo "score_file tests done."

tetromino_test: $(TESTS_BUILD_DIR)
	@echo "Building tetromino tests..."
	$(CC) $(CFLAGS_TEST) $(TEST_T_M_SRCS) $(LDFLAGS_TEST) -I$(CORE_INCLUDE_DIR) \
	-I$(API_INCLUDE_DIR) \
	-I$(CLI_INCLUDE_DIR) \
	-I$(SHARED_DIR) -o $(TESTS_BUILD_DIR)/test_tetromino
	@echo "Running tetromino tests..."
	$(TESTS_BUILD_DIR)/test_tetromino || exit 1
	@echo "tetromino tests done."

field_control_test: $(TESTS_BUILD_DIR)
	@echo "Building field_control tests..."
	$(CC) $(CFLAGS_TEST) $(TEST_F_C_SRCS) $(LDFLAGS_TEST) -I$(CORE_INCLUDE_DIR) \
	-I$(API_INCLUDE_DIR) \
	-I$(CLI_INCLUDE_DIR) \
	-I$(SHARED_DIR) -o $(TESTS_BUILD_DIR)/test_field_control
	@echo "Running field_control tests..."
	$(TESTS_BUILD_DIR)/test_field_control || exit 1
	@echo "field_control tests done."

# ============================================
# Cleanup
# ============================================

clean:
	-rm *.gcno
	@echo "Project directory cleaned"
	rm -rf $(BUILD_DIR)
	@echo "Build directory cleaned"
	rm -rf $(TESTS_BUILD_DIR) $(GCOV_DIR) $(TESTS_DIR)/tests_assets/test_scorefile.txt
	@echo "Tests directory cleaned"
	-rm -f $(EXEC_PATH)
	@echo "Executable file deleted"

# ============================================
# Install / Uninstall
# ============================================

install: release
	@echo "Installing to $(PREFIX)..."
	@mkdir -p $(BIN_DIR) $(LIB_INSTALL_DIR) $(INCLUDE_INSTALL_DIR)

	install -m 0755 $(EXEC_PATH) $(BIN_DIR)/$(EXEC)
	install -m 0644 $(STATIC_LIB) $(LIB_INSTALL_DIR)/$(LIB_NAME)

	# Install public headers
	install -m 0644 $(PUBLIC_HEADERS) $(INCLUDE_INSTALL_DIR)/

	@echo "Install done."

uninstall:
	@echo "Uninstalling from $(PREFIX)..."
	-rm -f $(BIN_DIR)/$(EXEC)
	-rm -f $(LIB_INSTALL_DIR)/$(LIB_NAME)
	-rmdir --ignore-fail-on-non-empty $(INCLUDE_INSTALL_DIR) || true
	@echo "Uninstall done."

# ============================================
# Doxygen documentation
# ============================================

dvi:
	@echo "Generating Doxygen documentation..."
	@if ! command -v doxygen >/dev/null 2>&1; then \
	  echo "doxygen not found, please install it (e.g. apt install doxygen / brew install doxygen)"; \
	  exit 1; \
	fi
	@mkdir -p docs/doxygen
	@if [ ! -f Doxyfile ]; then \
	  echo "PROJECT_NAME = \"tetris\" > Doxyfile"; \
	  echo "OUTPUT_DIRECTORY = docs/doxygen" >> Doxyfile; \
	  echo "GENERATE_HTML = YES" >> Doxyfile; \
	  echo "INPUT = $(CORE_DIR) $(CORE_INCLUDE_DIR) $(API_DIR) $(CLI_DIR) $(SHARED_DIR)" >> Doxyfile; \
	  echo "RECURSIVE = YES" >> Doxyfile; \
	fi
	doxygen Doxyfile
	@echo "Doxygen documentation generated in docs/doxygen/html"

# ============================================
# Distribution
# ============================================

dist: clean
	@mkdir -p dist
	tar -czf dist/tetris-1.0.tar.gz --exclude='dist' --exclude='.git' --exclude='*.o' --exclude='a.out' --exclude='$(BUILD_DIR)/*' .
	@echo "Distribution package created: dist/tetris-1.0.tar.gz”

# ============================================
# Code style (check and format)
# ============================================

check: clang-check

clang-check:
	@echo "=== Checking code style ==="
	@for file in $(LIB_SRCS) $(GUI_SRCS) $(PUBLIC_HEADERS) $(PRIVATE_HEADERS) $(TESTS_SRCS) $(TESTS_HEADER); do \
			if [ -f "$$file" ]; then \
				clang-format -n -Werror "$$file" || exit 1; \
			fi; \
		done; \
		echo "Code style check passed"; \

format: clang-format

clang-format:
	@echo "=== Formatting code ==="
	@if command -v clang-format >/dev/null 2>&1; then \
		for file in $(LIB_SRCS) $(GUI_SRCS) $(PUBLIC_HEADERS) $(PRIVATE_HEADERS) $(TESTS_SRCS) $(TESTS_HEADER); do \
			if [ -f "$$file" ]; then \
				clang-format -i "$$file"; \
				echo "Formatted: $$file"; \
			fi; \
		done; \
		echo "Code formatting complete"; \
	fi
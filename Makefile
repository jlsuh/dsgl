CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Ivendor -Ivendor/unity -Itests -Iexamples -g -fsanitize=address,undefined

UNAME_S := $(shell uname -s)
RUN_ENV =

ifeq ($(UNAME_S),Darwin)
	RUN_ENV = MallocNanoZone=0
endif

ifeq ($(SRC),)
  ifneq ($(MAKECMDGOALS),clean)
  ifneq ($(MAKECMDGOALS),test)
  ifneq ($(MAKECMDGOALS),s)
    $(error SRC is not set. Usage: 'make run SRC=filename_without_extension' or 'make test' or 'make s')
  endif
  endif
  endif
endif

BIN_DIR = bin
BUILD_DIR = build
OUT_DIR = output

LIB_SRC = src/dsgl.c
LIB_OBJ = $(BUILD_DIR)/dsgl.o

UNITY_SRC = vendor/unity/unity.c
UNITY_OBJ = $(BUILD_DIR)/unity.o

TEST_SRC = tests/dsgl_tests.c
TEST_OBJ = $(BUILD_DIR)/dsgl_tests.o
TEST_BIN = $(BIN_DIR)/tests.out

ifneq ($(wildcard examples/to_png/$(SRC).c),)
	APP_SRC = examples/to_png/$(SRC).c
	APP_OBJ = $(BUILD_DIR)/$(SRC)_main.o
	ifneq ($(wildcard examples/$(SRC).c),)
		HELPER_SRC = examples/$(SRC).c
		HELPER_OBJ = $(BUILD_DIR)/$(SRC)_lib.o
	endif
else
	APP_SRC = examples/$(SRC).c
	APP_OBJ = $(BUILD_DIR)/$(SRC).o
	HELPER_OBJ = 
endif

TARGET = $(BIN_DIR)/$(SRC).out

.PHONY: all directories clean run test wasm s

all: directories $(TARGET)

directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BUILD_DIR)
	mkdir -p $(OUT_DIR)

$(TARGET): $(APP_OBJ) $(HELPER_OBJ) $(LIB_OBJ)
	$(CC) $(CFLAGS) $(APP_OBJ) $(HELPER_OBJ) $(LIB_OBJ) -o $@ -lm

$(BUILD_DIR)/%_main.o: examples/to_png/%.c include/dsgl.h | directories
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%_lib.o: examples/%.c include/dsgl.h | directories
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: examples/%.c include/dsgl.h | directories
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_OBJ): $(LIB_SRC) include/dsgl.h | directories
	$(CC) $(CFLAGS) -c $(LIB_SRC) -o $@

wasm:
	@chmod +x to_wasm.sh
	@./to_wasm.sh $(SRC)

test: $(TEST_BIN)
	$(RUN_ENV) ./$(TEST_BIN)

$(TEST_BIN): $(TEST_OBJ) $(LIB_OBJ) $(UNITY_OBJ)
	$(CC) $(CFLAGS) $(TEST_OBJ) $(LIB_OBJ) $(UNITY_OBJ) -o $@ -lm

$(TEST_OBJ): $(TEST_SRC) include/dsgl.h | directories
	$(CC) $(CFLAGS) -c $(TEST_SRC) -o $@

$(UNITY_OBJ): $(UNITY_SRC) | directories
	$(CC) $(CFLAGS) -c $(UNITY_SRC) -o $@

run: all
	$(RUN_ENV) ./$(TARGET)

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR) $(OUT_DIR) examples/to_wasm

s:
	bunx serve

CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -isystem vendor -g -fsanitize=address,undefined

UNAME_S := $(shell uname -s)

RUN_ENV =

ifeq ($(UNAME_S),Darwin)
	RUN_ENV = MallocNanoZone=0
endif

ifeq ($(SRC),)
  ifneq ($(MAKECMDGOALS),clean)
    $(error SRC is not set. Usage: 'make run SRC=file_name')
  endif
endif

APP_SRC = examples/$(SRC).c
LIB_SRC = src/dsgl.c
BIN_DIR = build/bin
OUT_DIR = output

TARGET = $(BIN_DIR)/$(SRC).out

.PHONY: all directories clean run

all: directories $(TARGET)

directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OUT_DIR)

$(TARGET): $(APP_SRC) $(LIB_SRC) include/dsgl.h | directories
	$(CC) $(CFLAGS) $(APP_SRC) $(LIB_SRC) -o $@ -lm

run: all
	$(RUN_ENV) ./$(TARGET)

clean:
	rm -rf build output

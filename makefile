CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

ifeq ($(SRC),)
  ifneq ($(MAKECMDGOALS),clean)
    $(error SRC is not set. Usage: 'make run SRC=file_name')
  endif
endif

LIB_SRC = src/canvas.c
APP_SRC = examples/$(SRC).c
BIN_DIR = build/bin
OUT_DIR = output

TARGET = $(BIN_DIR)/$(SRC)

.PHONY: all directories clean run

all: directories $(TARGET)

directories:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OUT_DIR)

$(TARGET): $(APP_SRC) $(LIB_SRC)
	$(CC) $(CFLAGS) $(LIB_SRC) $(APP_SRC) -o $(TARGET)

run: all
	./$(TARGET)

clean:
	rm -rf build output

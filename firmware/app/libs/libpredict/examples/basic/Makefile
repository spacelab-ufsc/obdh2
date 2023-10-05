TARGET=basic

ifndef BUILD_DIR
	BUILD_DIR=$(CURDIR)
endif

CC=gcc
FLAGS=-fpic -std=gnu99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -lm

.PHONY: all
all: $(BUILD_DIR)/main.o
	$(CC) $(FLAGS) $(BUILD_DIR)/main.o -o $(BUILD_DIR)/$(TARGET) -lpredict

$(BUILD_DIR)/main.o: main.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o $(BUILD_DIR)/$(TARGET)

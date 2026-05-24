BUILD_DIR ?= $(CURDIR)

TOOLCHAIN_PREFIX ?= 
CC_FLAGS_APPEND ?= 

CC := $(TOOLCHAIN_PREFIX)gcc
INC := ../include/
FLAGS := -fpic -std=gnu99 -Wall -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -lm -I$(INC) -O3

FLAGS += $(CC_FLAGS_APPEND)

.PHONY: all
all: $(BUILD_DIR)/julian_date.o $(BUILD_DIR)/moon.o $(BUILD_DIR)/observer.o $(BUILD_DIR)/orbit.o $(BUILD_DIR)/refraction.o $(BUILD_DIR)/sdp4.o $(BUILD_DIR)/sgp4.o $(BUILD_DIR)/sun.o $(BUILD_DIR)/unsorted.o 

$(BUILD_DIR)/julian_date.o: julian_date.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/moon.o: moon.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/observer.o: observer.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/orbit.o: orbit.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/refraction.o: refraction.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/sdp4.o: sdp4.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/sgp4.o: sgp4.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/sun.o: sun.c
	$(CC) $(FLAGS) -c $< -o $@

$(BUILD_DIR)/unsorted.o: unsorted.c
	$(CC) $(FLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(BUILD_DIR)/*.o

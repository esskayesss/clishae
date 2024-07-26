CC = clang
BASE_FLAGS = -Wall -Wextra -g -I./include
LIBS =

# Find all source files and test source files recursively
SRCS = $(shell find src -name "*.c")
SRCS := $(filter-out src/main.c, $(SRCS))
SRC_OBJS = $(SRCS:src/%.c=obj/%.o)

TEST_SRCS = $(shell find tests -name "*.c")
TEST_BINS = $(TEST_SRCS:tests/%.c=bin/%)

.PHONY: all clean test bin

all: $(SRC_OBJS)

clean:
	@rm -rf bin
	@rm -rf obj
	@echo "done cleaning"

test: $(TEST_BINS)
	@for dep in $^; do echo "running $$dep-"; ./$$dep; done

bin: $(SRC_OBJS)
	@mkdir -p bin
	$(CC) $(BASE_FLAGS) src/main.c $(SRC_OBJS) -o bin/clishae

$(SRC_OBJS): obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(BASE_FLAGS) -c $< -o $@

$(TEST_BINS): bin/%: tests/%.c $(SRC_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(BASE_FLAGS) $(SRC_OBJS) $< -o $@

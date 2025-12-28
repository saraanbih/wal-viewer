CC = gcc
CFLAGS = -Wall -Wextra -g -Iinclude
LIBS = -lncurses

SRCS = src/main.c src/ui.c src/wal_parser.c src/search.c src/utils.c
OBJS = $(SRCS:src/%.c=build/%.o)
TARGET = build/wal-viewer

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LIBS) -o $(TARGET)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build

run: $(TARGET)
	./$(TARGET) data/sample_wal.txt

.PHONY: all clean run
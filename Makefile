CC = g++
CCFLAGS = -O3 -fsanitize=address -Wall
DEBUG_FLAGS = -g

.PHONY: clean

debug_tux:
	$(CC) $(DEBUG_FLAGS) $(CCFLAGS) -o degub_tux ./src/*.cpp

build_tux:
	$(CC) $(CCFLAGS) -o tux ./src/*.cpp

clean:
	rm tux

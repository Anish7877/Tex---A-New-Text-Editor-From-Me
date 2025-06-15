CC = g++
CCFLAGS = -O3 -fsanitize=address -Wall -o

.PHONY: clean

tux:
	$(CC) $(CCFLAGS) tux ./src/*.cpp

clean:
	rm tux

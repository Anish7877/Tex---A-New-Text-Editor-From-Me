CC = g++
CCFLAGS = -O3 -fsanitize=address -Wall -o

.PHONY: clean

tex:
	$(CC) $(CCFLAGS) tex ./src/*.cpp

clean:
	rm tex

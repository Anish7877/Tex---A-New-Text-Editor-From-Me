CC = g++
CCFLAGS = -O3 -fsanitize=address -Wall -o

tex:
	$(CC) $(CCFLAGS) tex ./src/*.cpp
clean:
	rm tex

CC=g++ -std=c++14 -pthread

all: test

build: main.cpp ThreadPool.h
	$(CC) -c main.cpp
	$(CC) -o thread main.o

run: main.cpp ThreadPool.h
	$(CC) -c main.cpp
	$(CC) -o thread main.o
	./thread

test: main.cpp ThreadPool.h
	$(CC) -c main.cpp
	$(CC) -o thread main.o
	./thread test

clean:
	rm -rf *.o thread
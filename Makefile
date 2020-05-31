GCC = g++
FLAGS = -O3 -std=c++17

main: main.o
	${GCC} ${FLAGS} main.o -o main

main.o: main.cpp
	${GCC} ${FLAGS} -c main.cpp -o main.o
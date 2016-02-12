CC = gcc
OPTIONS = -Wall
EXE = tp1

all:  clean build

build:
	$(CC) $(OPTIONS) bank.c -o $(EXE)

clean:
	-rm tp1



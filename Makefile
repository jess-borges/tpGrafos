############################# Makefile ##########################
CC=gcc
CFLAGS=-W -Wall -ansi -pedantic
EXEC=grafo
OBJ=grafo.o io.o main.o
all: $(EXEC)
grafo: $(OBJ)
	$(CC) -o $@ $^ 
%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)
main.o: main.c grafo.h io.h
	$(CC) -o $@ -c $< $(CFLAGS)
.PHONY: clean mrproper
clean: 
	rm -rf *.o
mrproper: clean
	rm -rf $(EXEC)

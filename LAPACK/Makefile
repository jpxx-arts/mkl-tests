CC = gcc
SRC = ./src
OBJ = ./obj
INCLUDE = ./include
BIN = ./bin
EXE = out
LIBFLAGS = -lmkl_rt

all:
	$(CC) -c $(SRC)/alg_lin.c -o $(OBJ)/alg_lin.o
	$(CC) $(SRC)/main.c $(OBJ)/alg_lin.o -I $(INCLUDE)/ -o $(BIN)/$(EXE) $(LIBFLAGS)

clean:
	rm -rf $(BIN)/*

run:
	$(BIN)/$(EXE) $(ARGS)
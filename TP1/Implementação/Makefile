OBJ = ./obj
SRC = ./src
INC = ./include
APP = ./app
BIN = ./bin

all: libed myapps

libed:
		gcc -c $(SRC)/data.c -I $(INC) -o $(OBJ)/data.o
		gcc -c $(SRC)/index.c -I $(INC) -o $(OBJ)/index.o
		gcc -c $(SRC)/utilidades.c -I $(INC) -o $(OBJ)/utilidades.o
		gcc -c $(SRC)/own_utils.c -I $(INC) -o $(OBJ)/own_utils.o
		gcc -c $(SRC)/functionalities.c -I $(INC) -o $(OBJ)/functionalities.o

myapps:
		gcc $(APP)/main.c $(OBJ)/*.o -I $(INC) -o $(BIN)/main

run:
		./$(BIN)/main

clean:
		rm $(OBJ)/*
		rm $(BIN)/*
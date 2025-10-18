OBJ = ./obj
SRC = ./src
INC = ./include
APP = ./app
BIN = ./bin

all: libed myapps

libed:
		gcc -ggdb -c $(SRC)/data.c -I $(INC) -o $(OBJ)/data.o
		gcc -ggdb -c $(SRC)/index.c -I $(INC) -o $(OBJ)/index.o
		gcc -ggdb -c $(SRC)/utilidades.c -I $(INC) -o $(OBJ)/utilidades.o
		gcc -ggdb -c $(SRC)/own_utils.c -I $(INC) -o $(OBJ)/own_utils.o
		gcc -ggdb -c $(SRC)/functionalities.c -I $(INC) -o $(OBJ)/functionalities.o

myapps:
		gcc -ggdb $(APP)/main.c $(OBJ)/*.o -I $(INC) -o $(BIN)/main

run:
		./$(BIN)/main

clean:
		rm $(OBJ)/*
		rm $(BIN)/*
		rm ./*.bin

zip:

		zip -r code.zip $(SRC) $(APP) $(BIN) $(INC) $(OBJ) Makefile

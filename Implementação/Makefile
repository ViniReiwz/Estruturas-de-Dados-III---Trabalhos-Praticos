OBJ = ./obj
SRC = ./src
INC = ./include
APP = ./app
BIN = ./bin

CFLAGS = -ggdb -Wall -Wextra -g

all: libed myapps

libed:
		gcc $(CFLAGS) -c $(SRC)/data.c -I $(INC) -o $(OBJ)/data.o
		gcc $(CFLAGS) -c $(SRC)/index.c -I $(INC) -o $(OBJ)/index.o
		gcc $(CFLAGS) -c $(SRC)/utilidades.c -I $(INC) -o $(OBJ)/utilidades.o
		gcc $(CFLAGS) -c $(SRC)/own_utils.c -I $(INC) -o $(OBJ)/own_utils.o
		gcc $(CFLAGS) -c $(SRC)/follow.c -I $(INC) -o $(OBJ)/follow.o
		gcc $(CFLAGS) -c $(SRC)/functionalities.c -I $(INC) -o $(OBJ)/functionalities.o
		gcc $(CFLAGS) -c $(SRC)/graph.c -I $(INC) -o $(OBJ)/graph.o

myapps:
		gcc $(CFLAGS) $(APP)/main.c $(OBJ)/*.o -I $(INC) -o $(BIN)/main

run:
		./$(BIN)/main

clean:
		rm $(OBJ)/*
		rm $(BIN)/*
		rm ./*.bin

zip:
		zip -r code.zip $(SRC) $(APP) $(BIN) $(INC) $(OBJ) Makefile

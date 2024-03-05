NAME=hokm-engine
CC=gcc
OUTPUT_DIR=./bin
OUTPUT_FILE=$(OUTPUT_DIR)/$(NAME)
FILES=main.c libs/lobby/lobby.c libs/cards/card.c
FLAGS=-Ilibs -Iabstraction

clean:
	rm -rf $(OUTPUT_DIR)

build: clean
	mkdir ./bin
	$(CC) -o $(OUTPUT_FILE) $(FILES) $(FLAGS)
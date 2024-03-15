NAME=hokm-engine
CC=gcc
OUTPUT_DIR=./bin
OUTPUT_FILE=$(OUTPUT_DIR)/$(NAME)
FILES=main.c libs/lobby/lobby.c libs/cards/card.c libs/menu/menu.c libs/io/io.c game/menu/menu.c libs/thread/thread.c
FLAGS=-Ilibs -Iabstraction -lncurses

clean:
	rm -rf $(OUTPUT_DIR)

build: clean
	mkdir ./bin
	$(CC) -o $(OUTPUT_FILE) $(FILES) $(FLAGS)
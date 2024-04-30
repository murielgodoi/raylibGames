SRC=./*.c
INCLUDE=-I./include/ -L./lib
BIN=./ariel
EXTRA_FLAGS = -Wall -Werror -Wextra -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all:
	gcc $(SRC) -g -lm $(EXTRA_FLAGS) $(INCLUDE) -o $(BIN)

run:
	$(BIN)

config:
	export LD_LIBRARY_PATH=./lib/
	sudo ldconfig

clean:
	rm $(BIN)

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./$(BIN)
SRC=./*.c
INCLUDE=-I./include/ -L./lib
INCLUDE_WIN=-I./includewin/ -L./libwin
BIN=./hello
BIN_WIN=./$(EXTRA_FLAGS).exe
EXTRA_FLAGS = -Wall -Werror -Wextra -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
EXTRA_FLAGS_WIN = -Wall -Werror -Wextra -std=c99 -Wno-missing-braces -lraylib -lm -lopengl32 -lgdi32 -lwinmm

all:
	gcc $(SRC) -g -lm $(EXTRA_FLAGS) $(INCLUDE) -o $(BIN)


win:
	gcc $(SRC) -g -lm $(EXTRA_FLAGS_WIN) $(INCLUDE_WIN) -o $(BIN_WIN)

run:
	$(BIN)

runwin:
	$(BIN_WIN)

clean:
	rm $(BIN)

valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./$(BIN)
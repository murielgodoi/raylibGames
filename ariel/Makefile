# Verifica o sistema operacional
ifdef OS
  OS := $(strip $(OS))
else
  OS := $(strip $(shell uname))
endif

BINNAME = ariel

ifeq ($(OS),Windows_NT)
	INCLUDE = -I./include/ -L./libwin
	EXTRA_FLAGS = -Wall -Werror -Wextra -std=c99 -Wno-missing-braces -lraylib -lm -lopengl32 -lgdi32 -lwinmm
	BIN =.\$(BINNAME).exe
	RM = del /Q /F
else
	INCLUDE=-I./include/ -L./lib
	EXTRA_FLAGS = -Wall -Werror -Wextra -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
	BIN =./$(BINNAME)
	RM = rm -f
endif

SRC=./*.c

all:
	gcc $(SRC) -g -lm $(EXTRA_FLAGS) $(INCLUDE) -o $(BIN)

web:
	# emcc $(SRC) -Os -Wall -g -s USE_GLFW=3 -DPLATFORM_WEB -I./include/ -L./libweb -o $(BINNAME).html
	# emcc ./*.c -g -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB -I./include/ -L./libweb -lraylib -s ERROR_ON_UNDEFINED_SYMBOLS=0 -o ./$(BINNAME).html

run:
	$(BIN)

clean:
	$(RM) $(BIN)


valgrind:
	valgrind --tool=memcheck --leak-check=full --track-origins=yes --show-leak-kinds=all --show-reachable=yes ./$(BIN)
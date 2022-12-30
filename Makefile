C   :=gcc
CXX :=g++

EXE :=bin/space_invader

LIBS:=-ldl -lm -lSDL2 -lSDL2_mixer -lGLESv2

SRC_C   :=src/platform.c
SRC_CXX :=src/main.cpp

SANITIZE  :=-Wl,-fsanitize=address,-fsanitize=bounds,-fsanitize=enum,-fsanitize=signed-integer-overflow,-fsanitize=null,-fsanitize=signed-integer-overflow,-fsanitize=return,-fsanitize=integer-divide-by-zero,-fsanitize=unreachable,-fsanitize=alignment
FLAGS_C   :=-DDEBUG -O0 -g -fno-exceptions -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -Wno-unused-variable -Wno-unused-but-set-variable $(SANITIZE)
FLAGS_CXX :=$(FLAGS_C)

OBJ_C   :=$(patsubst %.c,   %.o, $(SRC_C))
OBJ_CXX :=$(patsubst %.cpp, %.o, $(SRC_CXX))

%.o: %.c %.h
	@mkdir -p bin
	@echo -e "Compiling $<"
	@$(C) -c -o $@ $< $(FLAGS_C)

%.o: %.cpp %.hpp %.h
	@mkdir -p bin
	@echo -e "Compiling $<"
	@$(CXX) -c -o $@ $< $(FLAGS_CXX)

$(EXE): $(OBJ_C) $(OBJ_CXX)
	@mkdir -p bin
	@$(CXX) -o $(EXE) $(OBJ_C) $(OBJ_CXX) $(LIBS)
	@echo -e "Done"

build: $(EXE) $(OBJ_C) $(OBJ_CXX)

.PHONY: run build

run:
	@./$(EXE)

clean:
	@echo -e "Deleting *.o   ..."
	@rm *.o -fr
	@rm src/*.o -fr
	@echo -e "Deleting bin/* ..."
	@rm bin/* -fr

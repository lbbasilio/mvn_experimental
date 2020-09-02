BIN = bin
SRC = src
INC = inc

FLAGS = -g
VPATH = src:./$(src)
INCLUDE = -I"./$(INC)"

SOURCES = $(wildcard $(SRC)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(BIN)/%.o, $(SOURCES))
TARGET = main.exe

$(TARGET): $(OBJECTS)
	gcc $(FLAGS) $^ -o $@

$(BIN)/%.o: %.c
	gcc $(FLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm $(BIN)/*.o
	rm $(TARGET)

.PHONY: clean

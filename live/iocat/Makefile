CC = gcc
CFLAGS = -Werror -Wall -Wextra -std=c99 -pedantic -fsanitize=address
LDFLAGS = -lasan

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
BIN = iocat

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

debug: CFLAGS += -g
debug: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(BIN)

.PHONY: clean

clean:
	$(RM) *.o
	$(RM) $(BIN)

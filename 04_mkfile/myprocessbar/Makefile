BIN=myprocessbar.exe
CC=gcc
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
LKFLAGS=-o
FLAGS=-c -std=c99
RM=rm -f

$(BIN):$(OBJ)
	@$(CC) $(LKFLAGS) $@ $^
	@echo "linking... $^ to $@"

%.o:%.c
	@$(CC) $(FLAGS) $<
	@echo "compling... $< to $@"

.PHONY:clean
clean:
	$(RM) $(BIN) $(OBJ)

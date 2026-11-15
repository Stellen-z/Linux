SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
CC=gcc
FLAGS=-c 
LKFLAGS= -o 
BIN=processbar
RM=rm -rf

$(BIN):$(OBJ)
	@$(CC) $(LKFLAGS) $@ $^
	@echo "Linking... $^ to $@" 

%.o:%.c
	@$(CC) $(FLAGS) $<
	@echo "Compling... $< to $@"

.PHONY:clean
clean:
	@$(RM) $(OBJ) $(BIN)
	@echo "Clean completed."

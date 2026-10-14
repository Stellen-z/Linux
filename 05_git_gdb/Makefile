BIN = mycmd.exe
SRC = mycmd.c
OBJ = mycmd.o
CC = gcc
FLAGS = -std=c99 -c -g
LKFLAGS =-o
RM = rm -rf

$(BIN):$(OBJ)
	@$(CC) $(LKFLAGS) $@ $^
	@echo "linking... $^ to $@"

%.o:%.c
	@$(CC) $(FLAGS) $<
	@echo "compling... $< to $@"

.PHONY:clean
clean:
	$(RM) $(OBJ) $(BIN)




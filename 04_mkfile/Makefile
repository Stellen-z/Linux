BIN=myproc.exe
CC=gcc
RM=rm -f
SRC=myproc.c
OBJ=myproc.o
LKFLAGS=-o
FLAGS=-c

$(BIN):$(OBJ)
	@$(CC) $(LKFLAGS) $@ $^
	@echo "linking... $^ to $@"
%.o:%.c
	@$(CC) $(FLAGS) $<
	@echo "compling... $< to $@"

.PHONY:clean
clean:
	$(RM) $(OBJ) $(BIN)

.PHONY:test
test:
	@echo $(BIN)
	@echo $(CC)
	@echo $(RM)
	@echo $(SRC)
	@echo $(OBJ)
	@echo $(LKFLAGS)

#code:code.c
#	gcc code.c -o code


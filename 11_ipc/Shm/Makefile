.PHONY:all
all: server client
	
server: server.o
	@echo "Linking $@..."
	@g++ -o $@ $^

client: client.o
	@echo "Linking $@..."
	@g++ -o $@ $^

%.o: %.cc
	@echo "Compling... $< -> $@"
	@g++ -c $<

.PHONY:clean
clean:
	@echo "Cleaning up..."
	@rm -rf *.o server client
	@echo "All clean"

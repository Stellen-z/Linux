libmyc.so:mystdio.o mystrlen.o
	gcc -shared -o $@ $^ 

%.o:%.c
	gcc -fPIC -c $<

.PHONY:clean
clean:
	rm -rf *.o libmyc.so 

.PHONY:output
output:libmyc.so
	mkdir -p lib/include
	mkdir -p lib/mylib
	cp -f *.h lib/include
	cp -f *.so lib/mylib
	tar -czvf libmyc.tgz lib/


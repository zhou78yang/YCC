VPATH = lexer:common:parser:compiler:test
OBJS = token.o scanner.o

ycc: main.cc $(OBJS)
	clang++ -std=c++11 -o ycc main.cc $(OBJS)
%.o: %.cc
	clang++ -std=c++11 -c $< -o $@

.PHONY: clean
clean:
	-rm *.o ycc

VPATH = lexer:common:parser:compiler:vm:test
OBJS = token.o scanner.o error.o symbols.o symbol_table.o parser.o depth_vistor.o

ycc: main.cc $(OBJS)
	clang++ -std=c++11 -o ycc main.cc $(OBJS)
%.o: %.cc
	clang++ -std=c++11 -c $< -o $@

.PHONY: clean
clean:
	-rm *.o ycc

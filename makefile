VPATH = lexer:common:parser:compiler:vm:test
OBJS = token.o scanner.o error.o symbols.o symbol_table.o parser.o depth_vistor.o
DPATH = /bin/ycc

ycc: main.cc $(OBJS)
	clang++ -std=c++11 -o $(DPATH) main.cc $(OBJS); rm *.o
%.o: %.cc
	clang++ -std=c++11 -c $< -o $@

.PHONY: clean, uninstall
clean:
	-rm *.o ycc
uninstall:
	-rm $(DPATH)

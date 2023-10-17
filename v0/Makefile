CFLAGS = -I. -g

mybc: main.o lexer.o parser.o
	$(CC) -o $@ $^

clean:
	$(RM) *.o

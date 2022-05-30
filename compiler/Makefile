pipi: lib/stack.o lib/error.o lib/str.o
	cc -o csirke *.c lib/*.o

string.o: lib/str.c lib/str.h
	cc -c lib/str.c -o lib/str.o

error.o: lib/error.c lib/error.h
	cc -c lib/error.c -o lib/error.o

stack.o: lib/stack.c lib/stack.h
	cc -c lib/stack.c -o lib/stack.o
	

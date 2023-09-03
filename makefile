CC = gcc
C_FLAGS = -Wall -Wpedantic

main: main.o Symbols.o diagram.o
	$(CC) $(C_FLAGS) -o main main.o Symbols.o diagram.o -lm

main.o: main.c Symbols.h diagram.h
	$(CC) $(C_FLAGS) -c main.c

Symbols.o: Symbols.c Symbols.h macros.h
	$(CC) $(C_FLAGS) -c Symbols.c

diagram.o: diagram.c diagram.h Symbols.h
	$(CC) $(C_FLAGS) -c diagram.c

clean:
	$(RM) main *.o *~

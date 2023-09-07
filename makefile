CC = gcc
C_FLAGS = -Wall -Wpedantic

main: main.o Symbols.o diagram.o filehandling.o
	$(CC) $(C_FLAGS) -o main main.o Symbols.o diagram.o filehandling.o -lm

main.o: main.c filehandling.h diagram.h
	$(CC) $(C_FLAGS) -c main.c

Symbols.o: Symbols.c Symbols.h macros.h filehandling.h
	$(CC) $(C_FLAGS) -c Symbols.c

diagram.o: diagram.c diagram.h Symbols.h
	$(CC) $(C_FLAGS) -c diagram.c

filehandling.o: filehandling.c filehandling.h
	$(CC) $(C_FLAGS) -c filehandling.c

clean:
	$(RM) main *.o *~

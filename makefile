IDIR=inc
ODIR=obj
LDIR=src

TARGET=main

CC=gcc
CFLAGS=-I$(IDIR) -Wall -Wpedantic
LIBS=-lm

_DEPS = macros.h Symbols.h diagram.h filehandling.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o Symbols.o diagram.o filehandling.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(LDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(IDIR)/*~

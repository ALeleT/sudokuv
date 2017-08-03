CC=gcc
CFLAGS=-c -Wall -Werror
LDFLAGS=-lncurses -lm
SOURCES=main.c game_loop.c sudoku.c graph.c 
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sudoku

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o 

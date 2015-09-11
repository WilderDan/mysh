CC=gcc
CFLAGS=-c -Wall -g

HEADERS=$(wildcard include/*.h)
SOURCES=$(wildcard src/*.c)
OBJECTS=$(notdir $(SOURCES:.c=.o) )

EXECUTABLE=mysh

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)  
	$(CC) $(OBJECTS) -o $(EXECUTABLE) 

%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm $(OBJECTS) $(EXECUTABLE)

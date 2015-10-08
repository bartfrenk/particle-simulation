CC = g++

INCLUDE = $(shell pkg-config gtkmm-3.0 --cflags)
LIBS = $(shell pkg-config gtkmm-3.0 --libs)

OBJECTS = client.o
HEADERS = $(shell ls *.h)
OPTIONS =-Wall -pedantic

debug: OPTIONS += -DDEBUG -g
debug: test

.PHONY: all clean test client debug
all: client, test

%.o: %.cpp
	$(CC) -c $< $(OPTIONS)

client: $(OBJECTS)
	$(CC) -o $@.out $^ $(OPTIONS) $(LIBS)

test: test.o utils.o
	$(CC) -o $@.out $^ $(OPTIONS)

clean:
	rm -f *.o *.out

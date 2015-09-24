CFLAGS = $(shell pkg-config gtkmm-3.0 --cflags)
LIBS = $(shell pkg-config gtkmm-3.0 --libs)
OBJECTS = client.o
OPTIONS =-Wall -pedantic

.PHONY: all clean
all: client, test

%.o: %.cpp
	g++ -c $< $(OPTIONS)

client: $(OBJECTS)
	g++ -o $@.out $^ $(OPTIONS) $(LIBS)

test: test.o
	g++ -o $@.out $^ $(OPTIONS)

clean:
	rm -f *.o *.out

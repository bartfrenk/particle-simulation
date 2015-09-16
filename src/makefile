CFLAGS = $(shell pkg-config gtkmm-3.0 --cflags)
LIBS = $(shell pkg-config gtkmm-3.0 --libs)
OBJECTS = canvas.o client.o
OPTIONS =-Wall -pedantic

.PHONY: all clean
all: client

%.o: %.cpp
	g++ -c $< $(OPTIONS) $(CFLAGS)

client: $(OBJECTS)
	g++ -o $@.out $^ $(OPTIONS) $(LIBS)

clean:
	rm -f *.o *.out

CPP=g++
CFLAGS=-Wall
DEPS = 
LDFLAGS=	

%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CFLAGS)

all: example

example: example.o StatsDLynx.o
	g++ $(LDFLAGS) -o example example.o StatsDLynx.o $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o example


CC=g++
CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
DEPS = 
OBJ = main.o

all: run clean

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

run: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o
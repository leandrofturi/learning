CC=g++
CFLAGS = -lm -w `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv libswresample libavformat libavcodec libswscale libavutil`
DEPS = 
OBJ = ff2cv_split.o

all: run clean

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LIBS)

run: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f *.o
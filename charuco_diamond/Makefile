CC = g++ -std=c++11
CFLAGS = -Wall -g
LDFLAGS = -lm -pthread

# opencv 
CFLAGS += `pkg-config --cflags opencv`
LDFLAGS += `pkg-config --libs opencv`

# cuda
CFLAGS += -I/usr/local/cuda/include/
LDFLAGS += -L/usr/local/cuda/lib64 -lcuda -lcudart -lcublas -lcurand

# zed
CFLAGS += -I/usr/local/zed/include
LDFLAGS += -L/usr/local/zed/lib -lsl_zed

# deps
DEPS = $(wildcard *.h) $(wildcard *.hpp)
OBJS = create_diamond

all: $(OBJS) clean

$(OBJS): %: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o

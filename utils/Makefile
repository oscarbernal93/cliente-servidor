CC	= g++ -std=c++11 
#LDFLAGS = -lpthread -lzmqpp -lzmq

all: int_size

int_size: int_size.cc
	$(CC) -o int_size int_size.cc

int_size: version.cc
	$(CC) -o version version.cc

clean:
	rm -f int_size version
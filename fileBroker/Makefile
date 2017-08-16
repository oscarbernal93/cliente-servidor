CC	= g++ -std=c++11 -Wall -g
LDFLAGS = -lpthread -lzmqpp -lzmq
LOCAL_FLAGS = -I ~/zmq/include -L ~/zmq/lib


all: server client broker

server: server.cc
	$(CC) -o server server.cc $(LDFLAGS) $(LOCAL_FLAGS)

client: client.cc
	$(CC) -o client client.cc $(LDFLAGS) $(LOCAL_FLAGS)

broker: broker.cc
	$(CC) -o broker broker.cc $(LDFLAGS) $(LOCAL_FLAGS)

clean:
	rm -f client server broker


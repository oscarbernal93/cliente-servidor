CC= g++ -std=c++11
LIBS=-lzmq -lzmqpp -lsfml-audio 

all: client server

client:
	$(CC) client.cc -o client $(LIBS)

server:
	$(CC) server.cc -o server $(LIBS)

clean:
	rm server client

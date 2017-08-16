CC= g++ -std=c++11
#CC= clang++ -stdlib=libstdc++ -std=c++11
LIBS=-lzmq -lzmqpp

all: client server

client:
	$(CC) client.cc -o client.o $(LIBS)

server:
	$(CC) server.cc -o server.o $(LIBS)

clean:
	rm server client



CC= g++ -std=c++11 -I/usr/include
#CC= clang++ -stdlib=libstdc++ -std=c++11 -I/usr/include/
LIBS=-lzmq -lzmqpp -lsfml-audio


all: player server
libs: hermes apolo

player: player.cc
	$(CC) player.cc -o player $(LIBS) hermes.o apolo.o

server: server.cc
	$(CC) server.cc -o server $(LIBS) hermes.o apolo.o

hermes: hermes.cc
	$(CC) -c hermes.cc -o hermes.o
	
apolo: apolo.cc
	$(CC) -c apolo.cc -o apolo.o
	
clean:
	rm player server hermes.o apolo.o



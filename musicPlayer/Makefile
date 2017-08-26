CC= g++ -std=c++11 -I/usr/include
#CC= clang++ -stdlib=libstdc++ -std=c++11 -I/usr/include/
LIBS=-lzmq -lzmqpp -lsfml-audio


all: hermes apolo player

player: player.cc
	$(CC) player.cc -o player $(LIBS) hermes.o apolo.o

hermes: hermes.cc
	$(CC) -c hermes.cc -o hermes.o
	
apolo: apolo.cc
	$(CC) -c apolo.cc -o apolo.o
	
clean:
	rm player hermes.o apolo.o



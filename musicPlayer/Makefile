CC= g++ -std=c++11 -I/usr/include
#CC= clang++ -stdlib=libstdc++ -std=c++11 -I/usr/include/
LIBS=-lzmq -lzmqpp -lsfml-audio

all: play

play:
	$(CC) player.cc -o player $(LIBS)

clean:
	rm player



CC= g++ -std=c++11 -I/usr/include
#CC= clang++ -stdlib=libstdc++ -std=c++11 -I/usr/include/
LIBS=-lzmq -lzmqpp -lsfml-audio

all: record play

record:
	$(CC) record.cc -o record $(LIBS)

play:
	$(CC) play.cc -o play $(LIBS)

clean:
	rm record play my_record.ogg



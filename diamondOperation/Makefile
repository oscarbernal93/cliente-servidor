#sort -k2,2n -k3,3n -o USA-road-d.NY.sort.gr USA-road-d.NY.gr 
CC= g++ -std=c++11
LIBS=-lpthread

all: tpoolsm

simple:
	$(CC) simple.cc -o simple $(LIBS)

async:
	$(CC) async.cc -o async $(LIBS)

tpool:
	$(CC) tpool.cc -o tpool $(LIBS)

tpoolsm:  tpoolsm.cc
	$(CC) tpoolsm.cc -o tpoolsm $(LIBS)

clean:
	rm simple async tpool tpoolsm

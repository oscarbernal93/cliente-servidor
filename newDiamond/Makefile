CC= g++ -std=c++11 -I/usr/include
#LIBS=-lzmq -lzmqpp -lsfml-audio


all: lib diamond

diamond: diamond.cc
	$(CC) sparse_matrix.o safe_queue.o thread_pool.o diamond.cc -o diamond 

lib: sparse_matrix.cc thread_pool.cc safe_queue.cc
	$(CC) -c -o sparse_matrix.o sparse_matrix.cc
	$(CC) -c -o safe_queue.o safe_queue.cc
	$(CC) -c -o thread_pool.o thread_pool.cc

clean:
	rm sparce_matrix.o thread_pool.o safe_queue.o diamond



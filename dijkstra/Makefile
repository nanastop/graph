INCLUDE_DIR = ../
LIBRARY_DIR = ./
UTIL_PARENT = ../../

CC = gcc
CFLAGS = -g -O3 -Wall  
LDGLAGS = 
LIBS = -lpthread 

CFLAGS += -I$(INCLUDE_DIR) -I$(UTIL_PARENT)

all : test_dijkstra 

test_dijkstra : binary_heap.o dijkstra.o test_dijkstra.o adjlist.o util.o
	$(CC) $(LDFLAGS) binary_heap.o  dijkstra.o test_dijkstra.o adjlist.o util.o \
			  		  -o test_dijkstra -L$(LIBRARY_DIR) $(LIBS)

adjlist.o : ../graph/adjlist.c
	$(CC) $(CFLAGS) -c ../graph/adjlist.c

binary_heap.o : ../binary_heap/binary_heap.c
	$(CC) $(CFLAGS) -c ../binary_heap/binary_heap.c

util.o : $(UTIL_PARENT)/util/util.c
	$(CC) $(CFLAGS) -c $(UTIL_PARENT)/util/util.c

%.o : %.c 
	$(CC) $(CFLAGS) -c $<


clean :
	rm -f test_dijkstra *.o

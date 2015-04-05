INCLUDE_DIR = ../
LIBRARY_DIR = ./
UTIL_PARENT = ../../

CC = gcc
CFLAGS = -O3 -Wall -DPROFILE 
LDGLAGS = 
LIBS = -lpthread 

CFLAGS += -I$(INCLUDE_DIR) -I$(UTIL_PARENT)

all : test_kruskal test_mt_kruskal

test_kruskal : kruskal.o test_kruskal.o edgelist.o adjlist.o union_find.o util.o
	$(CC) $(LDFLAGS)  kruskal.o test_kruskal.o edgelist.o \
					  adjlist.o union_find.o util.o \
					  -o test_kruskal -L$(LIBRARY_DIR) $(LIBS)

test_mt_kruskal : kruskal.o mt_kruskal.o test_mt_kruskal.o edgelist.o adjlist.o union_find.o util.o processor_map.o
	$(CC) $(LDFLAGS)  kruskal.o mt_kruskal.o test_mt_kruskal.o edgelist.o \
					  adjlist.o union_find.o util.o processor_map.o \
					  -o test_mt_kruskal -L$(LIBRARY_DIR) $(LIBS)

edgelist.o : ../graph/edgelist.c
	$(CC) $(CFLAGS) -c ../graph/edgelist.c 

adjlist.o : ../graph/adjlist.c
	$(CC) $(CFLAGS) -c ../graph/adjlist.c

util.o : $(UTIL_PARENT)/util/util.c
	$(CC) $(CFLAGS) -c $(UTIL_PARENT)/util/util.c

processor_map.o : $(UTIL_PARENT)/util/processor_map.c
	$(CC) $(CFLAGS) -c $(UTIL_PARENT)/util/processor_map.c

union_find.o : ../disjoint_sets/union_find.c
	$(CC) $(CFLAGS) -c ../disjoint_sets/union_find.c

%.o : %.c 
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f test_kruskal test_mt_kruskal *.o

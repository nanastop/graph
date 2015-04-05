INCLUDE_DIR = ../ 
LIBRARY_DIR = ./
UTIL_PARENT = ../../

CC = gcc
CFLAGS = -O3 -Wall 
LDGLAGS = 
LIBS = -lpthread

CFLAGS += -I$(INCLUDE_DIR) -I$(UTIL_PARENT)

all : test_adjlist test_edgelist

test_adjlist : util.o adjlist.o test_adjlist.o
	$(CC) $(LDFLAGS) util.o adjlist.o test_adjlist.o -o test_adjlist -L$(LIBRARY_DIR) $(LIBS)

test_edgelist : util.o edgelist.o adjlist.o test_edgelist.o
	$(CC) $(LDFLAGS) util.o edgelist.o adjlist.o test_edgelist.o -o test_edgelist -L$(LIBRARY_DIR) $(LIBS)

util.o : $(UTIL_PARENT)/util/util.c
	$(CC) $(CFLAGS) -c $(UTIL_PARENT)/util/util.c

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f test_adjlist test_edgelist *.o

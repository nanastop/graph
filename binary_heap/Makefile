INCLUDE_DIR = ./ 
LIBRARY_DIR = ./

CC = gcc
CFLAGS = -O3 -Wall 
LDGLAGS = 
LIBS = 

CFLAGS += -I$(INCLUDE_DIR)

OBJS = binary_heap.o test_bheap.o

all : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o test_bheap -L$(LIBRARY_DIR) $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f test_bheap $(OBJS)

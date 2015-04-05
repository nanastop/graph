INCLUDE_DIR = ./ 
LIBRARY_DIR = ./

CC = gcc
CFLAGS = -O3 -Wall 
LDGLAGS = 
LIBS = 

CFLAGS += -I$(INCLUDE_DIR)

OBJS = union_find.o test_union_find.o

all : $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o test_union_find -L$(LIBRARY_DIR) $(LIBS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean :
	rm -f test_union_find $(OBJS)

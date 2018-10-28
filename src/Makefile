CC = gcc
CFLAGS = -Wall -g
LIBRARIES = -lm
PROG = cache
CACHE_C = cache.c

all:
	$(CC) $(CFLAGS) $(CACHE_C) main.c -o $(PROG) $(LIBRARIES)

clean :
	rm $(PROG) *.o *.s

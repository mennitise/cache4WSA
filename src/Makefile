CC = gcc
CFLAGS = -Wall -g
LIBRARIES = -lm
PROG = cache
MAIN_MEMORY_C = main_memory.c
CACHE_C = cache.c

all:
	$(CC) $(CFLAGS) $(MAIN_MEMORY_C) $(CACHE_C) main.c -o $(PROG) $(LIBRARIES)

clean :
	rm $(PROG) *.o *.s

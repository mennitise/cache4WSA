#ifndef MAIN_MEMORY_H
#define MAIN_MEMORY_H

typedef struct main_memory main_memory_t;

extern main_memory_t* MAIN_MEMORY;

/* 	The init() function initialize the blocks of the main memory
	as invalid and the rate of misses to 0. */
void init_main_memory();

/* 	The destroy() function destroys the Main Memory*/
void destroy_main_memory();

#endif // MAIN_MEMORY_H

#ifndef CACHE_H
#define CACHE_H

typedef struct cache cache_t;

extern cache_t* CACHE;

/* 	The init() function initialize the blocks of the cache
	as invalid and the rate of misses to 0. */
void init();

/* 	The find_set(int address) function returns the cache
	set that to which the address address maps.  */
int find_set(int address);

/* 	The find_lru(int setnum) function returns the least
	recently used block within a set (or one of them if 
	there is more than one), using the corresponding field
	in the metadata of the blocks in the set. */
int find_lru(int setnum);

/* 	The is_dirty(int way, int blocknum) function returns
 	the state of the D bit of the corresponding block. */
int is_dirty(int way, int setnum);

/* 	The read_block(int blocknum) function reads the
 	block blocknum from memory and stores it in the
 	corresponding place in the cache. */
void read_block(int blocknum);

/* 	The write_block function (int way, int setnum) 
	writes the data contained in the setnum block of
	the way. */
void write_block(int way, int setnum);

/* 	The read_byte(address) function returns the
 	value corresponding to the address position. */
int read_byte(int address);

/* 	The write_byte(int address, char value) function
 	writes the value value to the correct position of
 	the block corresponding to the address. */
int write_byte(int address, char value);

/* 	The get_miss_rate() function returns the
	percentage of misses since the cache was
	initialized. */
int get_miss_rate();

/* 	The destroy() function destroys the Cache*/
void destroy();

#endif // CACHE_H

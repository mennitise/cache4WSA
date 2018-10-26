#ifndef CACHE_H
#define CACHE_H

typedef struct cache cache_t;

extern cache_t* CACHE;

void init();

int find_set(int address);

int find_lru(int setnum);

int is_dirty(int way, int setnum);

void read_block(int blocknum);

void write_block(int way, int setnum);

int read_byte(int address);

int write_byte(int address, char value);

int get_miss_rate();

void destroy();

#endif // CACHE_H

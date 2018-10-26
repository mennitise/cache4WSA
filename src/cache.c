#include <stdio.h>
#include <stdlib.h>

#include "cache.h"

const int CACHE_SIZE = 4 * 1024; // 4KB
const int BLOCK_SIZE = 64; // 64B

const int BITS_TAG = 6;
const int BITS_INDEX = 4;
const int BITS_OFFSET = 6;

typedef struct cache{
	
	int size;
	int* data;
	size_t misses;
	size_t hits;
	
}cache_t;

/*
struct cache_set{
	
};

cache_set* create_cache_set(){

}
*/

void init(){
	printf("Initialize Cache...\n");
	
	CACHE = malloc(sizeof(cache_t));
	if (!CACHE){
		printf("ERROR: Can't Initialize Cache\n");
		abort();
	}

	CACHE->size = 64 * 1024;
	CACHE->hits = 0;
	CACHE->misses = 0;

	CACHE->data = malloc(sizeof(int));
	if (!CACHE->data){
		printf("ERROR: Can't Initialize Cache Data\n");
		abort();
	}
}

int find_set(int address){
	return 0;
}

int find_lru(int setnum){
	return 0;
}

int is_dirty(int way, int setnum){
	return 0;
}

void read_block(int blocknum){

}

void write_block(int way, int setnum){

}

int read_byte(int address){
	return 0;
}

int write_byte(int address, char value){
	return 0;
}

int get_miss_rate(){
	if ((CACHE->misses + CACHE->hits) == 0) return 0;
	return CACHE->misses / ( CACHE->misses + CACHE->hits );
}

void destroy(){
	if (CACHE) free(CACHE->data);
	free(CACHE);
	puts("Cache destroyed!");
}
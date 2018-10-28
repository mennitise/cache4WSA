#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cache.h"

#define WAYS 			4
#define CACHE_SIZE 		4 * 1024 // 4KB
#define BLOCK_SIZE 		64 // 64B
#define BLOCKS 			( 4 * 1024 ) / 64

#define BITS_TAG 			6
#define BITS_TAG_INIT		0
#define BITS_TAG_END		5

#define BITS_INDEX			4
#define BITS_INDEX_INIT		6
#define BITS_INDEX_END		9

#define BITS_OFFSET 		6
#define BITS_OFFSET_INIT	10
#define BITS_OFFSET_END		15

#define BITS_ADDRESS 		16

/*
	 __________________
	| tag  |idx |offset|
	|______|____|______|
	|      |    |      |
	|------|----|------|
	|______|____|______|

*/

char* int_to_binary(int n){
	int c, d, count;
	char *pointer;

	count = 0;
	pointer = (char*)malloc(BITS_ADDRESS);
	if (!pointer){
		printf("ERROR: Can't Initialize blocks from cache\n");
		abort();
	}

	for (c = BITS_ADDRESS-1 ; c >= 0 ; c--){
		d = n >> c;

		if (d & 1)
			*(pointer+count) = 1 + '0';
		else
			*(pointer+count) = 0 + '0';

		count++;
	}
	*(pointer+count) = '\0';

	return  pointer;
}

char* get_tag(char* address){
	char* tag = (char*)malloc(BITS_TAG);
	if (!tag) return NULL;
	int count = 0;
	for (int i = BITS_TAG_INIT; i <= BITS_TAG_END; ++i){
		*(tag + count) = address[i];
		count++;
	}
	return tag;
}

char* get_index(char* address){
	char* index = (char*)malloc(BITS_INDEX);
	if (!index) return NULL;
	int count = 0;
	for (int i = BITS_INDEX_INIT; i <= BITS_INDEX_END; ++i){
		*(index + count) = address[i];
		count++;
	}
	return index;
}

char* get_offset(char* address){
	char* offset = (char*)malloc(BITS_OFFSET);
	if (!offset) return NULL;
	int count = 0;
	for (int i = BITS_OFFSET_INIT; i <= BITS_OFFSET_END; ++i){
		*(offset + count) = address[i];
		count++;
	}
	return offset;
}

/* -----------------BLOCK DEFINITION------------------- */

typedef struct block{
	time_t lastUpdate;
	int valid;
	int dirty;
	char* data;
}block_t;

block_t* init_block(){

	block_t* block = malloc(sizeof(block_t));
	if (!block){
		printf("ERROR: Can't Initialize blocks from cache\n");
		abort();
	}
	
	block->data = malloc(BLOCK_SIZE * sizeof(char*));
	if (!block->data){
		printf("ERROR: Can't Initialize data blocks from cache\n");
		abort();
	}
	
	block->lastUpdate = time(NULL);
	block->valid = 0;
	block->dirty = 0;

	return block;
}

void destroy_block(block_t* block){
	if(block) free(block->data);
	free(block);
}

/* ---------------------------------------------------- */

/* ----------------- SET DEFINITION ------------------- */

typedef struct cache_set{
	block_t* blocks[BLOCKS];
	size_t blocks_amount;
}cache_set_t;

cache_set_t* init_cache_set(){

	cache_set_t* cache_set = malloc(sizeof(cache_set_t));
	if (!cache_set){
		printf("ERROR: Can't Initialize sets from cache\n");
		abort();
	}

	cache_set->blocks_amount = BLOCKS;

	for (int i = 0; i < cache_set->blocks_amount; ++i){
		cache_set->blocks[i] = init_block();
	}

	return cache_set;
}

void destroy_cache_set(cache_set_t* cache_set){
	if (cache_set){
		for (int i = 0; i < cache_set->blocks_amount; ++i){
			destroy_block(cache_set->blocks[i]);
		}
	}
	cache_set->blocks_amount = 0;
	free(cache_set);
}

/* ---------------------------------------------------- */

/* ---------------- CACHE DEFINITION ------------------ */

typedef struct cache{
	int size;
	cache_set_t* sets[WAYS];
	size_t ways;
	size_t misses;
	size_t hits;
	
}cache_t;

void init(){
	printf("Initialize Cache...\n");
	
	CACHE = malloc(sizeof(cache_t));
	if (!CACHE){
		printf("ERROR: Can't Initialize Cache\n");
		abort();
	}

	CACHE->ways = WAYS;
	CACHE->size = 64 * 1024;
	CACHE->hits = 0;
	CACHE->misses = 0;

	for (int i = 0; i < CACHE->ways; ++i){
		CACHE->sets[i] = init_cache_set();
	}
}

void destroy(){
	if (CACHE){
		for (int i = 0; i < CACHE->ways; ++i){
			destroy_cache_set(CACHE->sets[i]);
		}
	}
	free(CACHE);
	puts("Cache destroyed!");
}

/* ---------------------------------------------------- */

int find_set(int address){
	char* bin = int_to_binary(address);

	char* tag = get_tag(bin_address);
	char* index = get_index(bin_address);
	char* offset = get_offset(bin_address);

	if (!tag || !index || !offset){
		printf("ERROR: Don't have space for initialize variables\n");
		abort();
	}

	printf("Address:%s\n Tag:%s Index:%s Offset:%s\n", bin_address, tag, index, offset);

	free(tag);
	free(index);
	free(offset);

	free(bin);
	return 0;
}

int find_lru(int setnum){
	if (!CACHE){
		printf("ERROR: The Cache isn't initialized\n");
		return 0;
	}

	int way_lru_block = 0;
	for (int i = 0; i < CACHE->ways; ++i){
		if (CACHE->sets[i]->blocks[setnum-1]->lastUpdate > CACHE->sets[way_lru_block]->blocks[setnum-1]->lastUpdate) {
			way_lru_block = i;
		}
	}
	return way_lru_block;
}

int is_dirty(int way, int setnum){
	if(!CACHE){
		printf("ERROR: The Cache isn't initialized\n");
		return 0;
	}
	
	if(CACHE->ways < way){
		printf("ERROR: The Cache Set especified don't exists\n");
		return 0;
	}else if(!CACHE->sets[way-1]){
		printf("ERROR: The Cache Set especified isn't initialized\n");
		return 0;
	}

	if(CACHE->sets[way-1]->blocks_amount < setnum){
		printf("ERROR: The Block especified in the Cache Set don't exists\n");
		return 0;
	}else if(!CACHE->sets[way-1]->blocks[setnum-1]){
		printf("ERROR: This Block especified in the Cache Set isn't initialized\n");
		return 0;
	}

	return CACHE->sets[way-1]->blocks[setnum-1]->dirty;
}

void read_block(int blocknum){

}

void write_block(int way, int setnum){

}

int read_byte(int address){
	char* bin_address = int_to_binary(address);

	char* tag = get_tag(bin_address);
	char* index = get_index(bin_address);
	char* offset = get_offset(bin_address);

	if (!tag || !index || !offset){
		printf("ERROR: Don't have space for initialize variables\n");
		abort();
	}

	printf("Address:%s\n Tag:%s Index:%s Offset:%s\n", bin_address, tag, index, offset);

	free(tag);
	free(index);
	free(offset);

	free(bin_address);
	return 0;
}

int write_byte(int address, char value){
	char* bin = int_to_binary(address);

	char* tag = get_tag(bin_address);
	char* index = get_index(bin_address);
	char* offset = get_offset(bin_address);

	if (!tag || !index || !offset){
		printf("ERROR: Don't have space for initialize variables\n");
		abort();
	}

	printf("Address:%s\n Tag:%s Index:%s Offset:%s\n", bin_address, tag, index, offset);

	free(tag);
	free(index);
	free(offset);

	free(bin);
	return 0;
}

int get_miss_rate(){
	if ((CACHE->misses + CACHE->hits) == 0) return 0;
	return CACHE->misses / ( CACHE->misses + CACHE->hits );
}

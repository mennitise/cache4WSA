#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "main_memory.h"
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

/* -------------------- UTILITIES --------------------- */

char* int_to_binary(int n, size_t bits){
	int c, d, count;
	char *pointer;

	count = 0;
	pointer = (char*)malloc(bits + 1);
	if (!pointer){
		printf("ERROR: Can't Initialize blocks from cache\n");
		abort();
	}

	for (c = bits - 1; c >= 0; c--){
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

int binary_to_int(char* bin, size_t bits){
	int result = 0;
	int count = bits - 1;
	for (int i = 0; i < bits; ++i){
		if (bin[i] == '1'){
			result += pow(2, count);
		}
		count--;
	}
	return result;
}

char* get_from_address(char* address, int len, int begin, int end){
	char* aux = (char*)malloc(len + 1);
	if (!aux) return NULL;
	int count = 0;
	for (int i = begin; i <= end; ++i){
		*(aux + count) = address[i];
		count++;
	}
	return aux;
}

char* get_tag(char* address){
	return get_from_address(address, BITS_TAG, BITS_TAG_INIT, BITS_TAG_END);
}

char* get_index(char* address){
	return get_from_address(address, BITS_INDEX, BITS_INDEX_INIT, BITS_INDEX_END);
}

char* get_offset(char* address){
	return get_from_address(address, BITS_OFFSET, BITS_OFFSET_INIT, BITS_OFFSET_END);
}

/* -----------------BLOCK DEFINITION------------------- */

typedef struct block{
	time_t lastUpdate;
	int valid;
	int dirty;
	int tag;
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
	cache_set_t* ways[WAYS];
	size_t amount_ways;
	int misses;
	int hits;
	
}cache_t;

void init(){
	printf("Initialize Cache...\n");
	
	CACHE = malloc(sizeof(cache_t));
	if (!CACHE){
		printf("ERROR: Can't Initialize Cache\n");
		abort();
	}

	CACHE->amount_ways = WAYS;
	CACHE->size = 64 * 1024;
	CACHE->hits = 0;
	CACHE->misses = 0;

	for (int i = 0; i < CACHE->amount_ways; ++i){
		CACHE->ways[i] = init_cache_set();
	}
}

void destroy(){
	if (CACHE){
		for (int i = 0; i < CACHE->amount_ways; ++i){
			destroy_cache_set(CACHE->ways[i]);
		}
	}
	free(CACHE);
	puts("Cache destroyed!");
}

/* ---------------------------------------------------- */

int find_set(int address){
	char* bin_address = int_to_binary(address, BITS_ADDRESS);

	char* tag = get_tag(bin_address);
	char* index = get_index(bin_address);
	char* offset = get_offset(bin_address);

	int founded = -1;

	if (!tag || !index || !offset){
		printf("ERROR: Don't have space for initialize variables\n");
		abort();
	}

	for (int i = 0; i < CACHE->amount_ways; ++i){
		if ( (CACHE->ways[i]->blocks[binary_to_int(index, BITS_INDEX)]->valid == 1) &&
			 (CACHE->ways[i]->blocks[binary_to_int(index, BITS_INDEX)]->tag == binary_to_int(tag, BITS_TAG)) 
		){
			founded = i;
			break;
		}
	}

	free(tag);
	free(index);
	free(offset);

	free(bin_address);
	return founded;
}

int find_lru(int setnum){
	if (!CACHE){
		printf("ERROR: The Cache isn't initialized\n");
		return 0;
	}

	int way_lru_block = 0;
	for (int i = 0; i < CACHE->amount_ways; ++i){
		if (CACHE->ways[i]->blocks[setnum]->lastUpdate > CACHE->ways[way_lru_block]->blocks[setnum]->lastUpdate) {
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
	
	if(CACHE->amount_ways < way){
		printf("ERROR: The Cache Set especified don't exists\n");
		return 0;
	}else if(!CACHE->ways[way-1]){
		printf("ERROR: The Cache Set especified isn't initialized\n");
		return 0;
	}

	if(CACHE->ways[way-1]->blocks_amount < setnum){
		printf("ERROR: The Block especified in the Cache Set don't exists\n");
		return 0;
	}else if(!CACHE->ways[way-1]->blocks[setnum-1]){
		printf("ERROR: This Block especified in the Cache Set isn't initialized\n");
		return 0;
	}

	return CACHE->ways[way-1]->blocks[setnum-1]->dirty;
}

void read_block(int blocknum){

    if(MAIN_MEMORY->blocks_amount < blocknum){
		printf("ERROR: The Main Memory block especified doesn't exists\n");
		return ;
	}else if(!MAIN_MEMORY->blocks[blocknum-1]){
		printf("ERROR: The Main Memory data especified isn't initialized\n");
		return ;
	}

    // OBTENGO EL TAG E INDEX DEL BLOCKNUM. SON 10 BITS LOS QUE IDENTIFICA A UN BLOQUE DE LA MEMORIA PRINCIPAL
	char* bin_blocknum = int_to_binary(blocknum, BITS_TAG + BITS_INDEX);
    char* tag = get_tag(bin_blocknum);
    char* index = get_index(bin_blocknum);

    int memory_changed = 0;

    //LEO LOS DATOS DE MEMORIA PRINCIPAL
    char* data_in_memory = malloc(BLOCK_SIZE * sizeof(char*));
    strcpy(data_in_memory, MAIN_MEMORY->blocks[binary_to_int(bin_blocknum, BITS_TAG + BITS_INDEX)]->data);

    // VER SI HAY ALGUN BLOQUE LIBRE EN CACHE
    for (int i = 0; i < CACHE->amount_ways; ++i){
        if (CACHE->ways[i]->blocks[binary_to_int(index, BITS_INDEX)]->valid == 0){
            printf("ENCUENTRA BLOQUE LIBRE EN CACHE (read_block)\n");
            strcpy(CACHE->ways[i]->blocks[binary_to_int(index, BITS_INDEX)]->data, data_in_memory);
            CACHE->ways[i]->blocks[binary_to_int(index, BITS_INDEX)]->dirty = 0; //Quedaria igual que en memoria principal
            CACHE->ways[i]->blocks[binary_to_int(index, BITS_INDEX)]->valid = 1;
            CACHE->ways[i]->blocks[binary_to_int(index, BITS_INDEX)]->lastUpdate = time(NULL);
            CACHE->ways[i]->blocks[binary_to_int(index, BITS_INDEX)]->tag = binary_to_int(tag, BITS_TAG);
            memory_changed = 1;
            break;
        }
    }

    // SI NO SE ENCONTRO BLOQUE LIBRE HAY QUE AGARRAR EL BLOQUE LRU
    if (memory_changed != 1) {
        int way_lru = find_lru(binary_to_int(index, BITS_INDEX));
        // ME FIJO SI EL BIT DIRTY ESTA EN 1 PARA GUARDAR LO QUE TIENE EN MEMORIA
        if (CACHE->ways[way_lru]->blocks[binary_to_int(index, BITS_INDEX)]->dirty == 1){
            strcpy(MAIN_MEMORY->blocks[binary_to_int(bin_blocknum, BITS_TAG + BITS_INDEX)]->data, CACHE->ways[way_lru]->blocks[binary_to_int(index, BITS_INDEX)]->data);
        }
        strcpy(CACHE->ways[way_lru]->blocks[binary_to_int(index, BITS_INDEX)]->data, data_in_memory);
        CACHE->ways[way_lru]->blocks[binary_to_int(index, BITS_INDEX)]->dirty = 0; //Quedaria igual que en memoria principal
        CACHE->ways[way_lru]->blocks[binary_to_int(index, BITS_INDEX)]->valid = 1;
        CACHE->ways[way_lru]->blocks[binary_to_int(index, BITS_INDEX)]->lastUpdate = time(NULL);
        CACHE->ways[way_lru]->blocks[binary_to_int(index, BITS_INDEX)]->tag = binary_to_int(tag, BITS_TAG);
    }
}

void write_block(int way, int setnum){

    if(!CACHE){
        printf("ERROR: The Cache isn't initialized\n");
        return ;
    }

    if(CACHE->amount_ways < way){
        printf("ERROR: The Cache Set especified don't exists\n");
        return ;
    }else if(!CACHE->ways[way-1]){
        printf("ERROR: The Cache Set especified isn't initialized\n");
        return ;
    }

    if(CACHE->ways[way-1]->blocks_amount < setnum){
        printf("ERROR: The Block especified in the Cache Set don't exists\n");
        return ;
    }else if(!CACHE->ways[way-1]->blocks[setnum-1]){
        printf("ERROR: This Block especified in the Cache Set isn't initialized\n");
        return ;
    }

    // BUSQUEDA DEL BLOQUE EN CACHE
    if (CACHE->ways[way-1]->blocks[setnum-1]->valid == 1){
        printf("ENCUENTRA EN CACHE\n");
        char* data = CACHE->ways[way-1]->blocks[setnum-1]->data;
        int tag = CACHE->ways[way-1]->blocks[setnum-1]->tag;
        char* bin_tag = int_to_binary(tag, BITS_TAG);
        char* bin_index = int_to_binary(setnum, BITS_INDEX);

        // Obtengo el numero de bloque de la memoria principal
        char* bin_blocknum;
        bin_blocknum = malloc(strlen(bin_tag)+strlen(bin_index)+1);
        strcpy(bin_blocknum, bin_tag);
        strcat(bin_blocknum, bin_index);

        int blocknum = binary_to_int(bin_blocknum, BITS_TAG + BITS_INDEX);

        // Guardo una copia de los datos en el bloque de la memoria principal
        strcpy(MAIN_MEMORY->blocks[blocknum-1]->data, data);

        free(bin_blocknum);
    }
    else{
        printf("NO SE PUDO ENCONTRAR EL BLOQUE EN CACHE\n");
    }
}

char read_byte(int address){
	char* bin_address = int_to_binary(address, BITS_ADDRESS);

	char* tag = get_tag(bin_address);
	char* index = get_index(bin_address);
	char* offset = get_offset(bin_address);

	if (!tag || !index || !offset){
		printf("ERROR: Don't have space for initialize variables\n");
		abort();
	}

	printf("	Address:%s\n 	Tag:%d Index:%d Offset:%d\n", bin_address, binary_to_int(tag, BITS_TAG), binary_to_int(index, BITS_INDEX), binary_to_int(offset, BITS_OFFSET));
	
	char value;
	int set;

	// BUSCA EL VALOR EN CACHE PARA RETORNARLO
	set = find_set(address);
	if (set != -1){
		printf("Read byte: Encuentra el set en cache - %d\n",set);
		CACHE->hits++;
		value = *(CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->data + binary_to_int(offset, BITS_OFFSET));
	} else {
		printf("Read byte: No encuentra el set en cache\n");
		CACHE->misses++;

		// LEER DE MEMORIA EL BLOQUE
		read_block(binary_to_int(bin_address, BITS_TAG + BITS_INDEX));
		set = find_set(address);
		if (set != 1){
			value = *(CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->data + binary_to_int(offset, BITS_OFFSET));
		} else {
			printf("ERROR: Reading block from main memory\n");
			abort();
		}
	}

	free(tag);
	free(index);
	free(offset);

	free(bin_address);
	return value;
}

int write_byte(int address, char value){
	char* bin_address = int_to_binary(address, BITS_ADDRESS);
	char* tag = get_tag(bin_address);
	char* index = get_index(bin_address);
	char* offset = get_offset(bin_address);

	if (!tag || !index || !offset){
		printf("ERROR: Don't have space for initialize variables\n");
		abort();
	}

	printf("	Address:%s\n 	Tag:%d Index:%d Offset:%d\n", bin_address, binary_to_int(tag, BITS_TAG), binary_to_int(index, BITS_INDEX), binary_to_int(offset, BITS_OFFSET));

	// BUSCAR SI EL BLOQUE SE ENCUENTRA EN CACHE PARA ESCRIBIR EL NUEVO VALOR
	int set = find_set(address);
	if (set != -1){
		printf("Write byte: Encuentra en cache el set: %d\n",set);
		CACHE->hits++;

		*(CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->data + binary_to_int(offset, BITS_OFFSET)) = value;
		CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->dirty = 1;
		CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->valid = 1;
		CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->lastUpdate = time(NULL);
		CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->tag = binary_to_int(tag, BITS_TAG);
	} else {
		// NO SE ENCUENTRA EL BLOQUE EN CACHE, LO TRAE DE MEMORIA
		printf("Write byte: No encuentra en cache el set. Trae de memoria el set.\n");
		CACHE->misses++;
		
		// LEER DE MEMORIA EL BLOQUE
		read_block(binary_to_int(bin_address, BITS_TAG + BITS_INDEX));
		
		// Una vez que lo trae lo vuelve a buscar
		set = find_set(address);
		if (set != -1){
			printf("Write byte: Luego de traer de memoria encuentra en cache el set:%d\n",set);
			*(CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->data + binary_to_int(offset, BITS_OFFSET)) = value;
			CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->dirty = 1;
			CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->valid = 1;
			CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->lastUpdate = time(NULL);
			CACHE->ways[set]->blocks[binary_to_int(index, BITS_INDEX)]->tag = binary_to_int(tag, BITS_TAG);
		} else {
			printf("ERROR: Reading block from main memory\n");
			abort();
		}
	}

	free(tag);
	free(index);
	free(offset);

	free(bin_address);
	return 0;
}

int get_miss_rate(){
	printf("%d - %d\n", CACHE->misses, CACHE->hits);
	if ((CACHE->misses + CACHE->hits) == 0) return 0;
	return CACHE->misses / ( CACHE->misses + CACHE->hits );
}

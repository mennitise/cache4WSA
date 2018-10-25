#include "cache.h"


const int CACHE_SIZE = 4 * 1024; // 4KB
const int BLOCK_SIZE = 64; // 64B

const int BITS_TAG = 6;
const int BITS_INDEX = 4;
const int BITS_OFFSET = 6;

void init(){

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
	return 0;
}